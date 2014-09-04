% Calculate productivity for 033-010-01-010 on Malmö-Göteborg with GCW 70t
% Tractor engine - D16, Electric motors used - M01, Buffer size - B01

%% WHAT CHANGES EVERY 5 YEARS
% Battery mass - hence mPayload, hence Revenue
% Battery price
% Fuel price
% Electricity price
% Driver hourly wages

%% -----------------------------------------------------------------------------------------------------------------------------------

clear all
close all
clc

%% Currency conversion

USDtoEUR = 0.76;

%% Simulation outputs

C = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsWithPredictiveControl/33/C.mat');
B(1) = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsWithPredictiveControl/33/U0B.mat');
B(2) = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsWithPredictiveControl/33/U1B.mat');
B(3) = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsWithPredictiveControl/33/U2B.mat');
B(4) = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsWithPredictiveControl/33/U3B.mat');

dMission = C.positionOverMission(end)/1000;    % km
tMission = size(C.positionOverMission,2)/3600; % hours
vFuel = C.fuelConsumptionOverMission(end);     % litres 

%% Get Parameters over years

ParameterEvolution;

%% Combination and mission data

sizeBattery = [batterySizes(2) batterySizes(2) batterySizes(2)];    % SIZE OF BATTERY 

%% ------CHANGE BELOW DATA TO CHANGE CONFIGURATION ----------------------------------------------------------------------
nUnits = 4;
nEAxles = [1 1 1];
nEUnits = size(nEAxles,2);

%% ------CHANGE BELOW DATA FOR N_FIRST_OWNER AND TRIPS ------------------------------------------------------------------

nFirstOwner=5;
nMissionDaily = 2;
nMissionAnnual = nMissionDaily*365;

%% Productivity evolution over years 2015-2030 in 5 year intervals
% YEARS = [2015 2020 2025 2030];

for nYear=1:4     
    %% --- Mission revenues----------------------------------------------------------------------------------------------

    revUnitFreight = 0.06*USDtoEUR; % [12] CHANGE IF REVENUE PER UNIT FREIGHT CHANGES

    mTractor = 9000;
    mSemiTrailer1 = 7000;
    mSemiTrailer2 = 7000;
    mDolly = 3000;
    
    %% --- CHANGE THESE TO OBSERVE EFFECTS OF GCW-------------------------------------------------------------------------
    
    tractorAxleLoads = [7094, 15340/2*[1 1]];   
    firstSemiTrailerAxleLoads = 22100/3*[1 1 1];    
    dollyAxleLoads =    10500/2*[1 1];    
    secondSemiTrailerAxleLoads = 13480/3*[1 1 1];   
    
    GVW = mTractor+mSemiTrailer1+mDolly+mSemiTrailer2;
    GCW = sum(tractorAxleLoads)+sum(firstSemiTrailerAxleLoads)+sum(dollyAxleLoads)+sum(secondSemiTrailerAxleLoads);
    
    mBattery(nYear,:) = [batteryMass(2,nYear) batteryMass(2,nYear) batteryMass(2,nYear)];    % MASS OF BATTERY ON EACH TRAILING UNIT IN FIRST YEAR
    mMotor = [43 43 43];
    mEAxle(nYear,:) = mBattery(nYear,:)+mMotor;
    deltaMAxle(nYear) = sum(mEAxle(nYear,:));

    mPayloadGross = GCW-GVW;
    mPayloadNet(nYear) = mPayloadGross - deltaMAxle(nYear);    % INCREASES as deltaMAxle decreases every 5 years
    
    revMission(nYear) = revUnitFreight*(mPayloadNet(nYear)/1000)*dMission;
    revAnnual(nYear) = revMission(nYear) * nMissionAnnual;
    rN(nYear) = revAnnual(nYear)*nFirstOwner;

    %% --- Fixed Costs-----------------------------------------------------------------------------------------------------

    pTractorBase = 130000*USDtoEUR;
    deltaPowertrain = 15000;    % D16-D11
    pTractorNet = pTractorBase + deltaPowertrain;    
    pDolly = 40000*USDtoEUR;
    pST1 = 70000*USDtoEUR;
    pST2 = 70000*USDtoEUR;
    
    cFixedConv = pTractorNet+pDolly+pST1+pST2;
    
    pEMotor = 30000;
    pEAxle = pEMotor*nEAxles;   % EUR
    pBattery(nYear,:) = batteryPrice(2,nYear)*ones(1,nUnits-1);

    cFixedElec(nYear,:) = pBattery(nYear,:)+pEAxle;

    cFixed(nYear) = cFixedConv+sum(cFixedElec(nYear,:));

    %% --- Operational costs------------------------------------------------------------------------------------------------

    %% --- CHANGE BELOW IF RATIOS OF OTHER COSTS TO DRIVER COSTS CHANGES ---------------------------------------------------
    rMntDriver = 6/35;
    rTyreDriver = 7/35;
    rTollDriver = 14/35;

    for j=2:nUnits
        deltaSOC = (B(1,j).stateOfBufferOverMission(1))-(B(1,j).stateOfBufferOverMission(end));
        chargeConsumedUnit(j) = deltaSOC*sizeBattery(1,j-1);
    end
    eRecharge = sum(chargeConsumedUnit)*3.6/3.6e6;  %kWh

    cDriver(nYear) = tMission*driverPrice(nYear);
    cFuel(nYear) = vFuel*fuelPrice(nYear);
    cMnt(nYear) = cDriver(nYear)*rMntDriver;
    cTyres(nYear) = cDriver(nYear)*rTyreDriver;
    cTolls(nYear) = cDriver(nYear)*rTollDriver;
    cElec(nYear) = eRecharge*elecPrice(nYear);

    cVariableMission(nYear) = cDriver(nYear)+cFuel(nYear)+cMnt(nYear)+cTyres(nYear)+cTolls(nYear)+cElec(nYear);
    cVariableN(nYear) = cVariableMission(nYear)*nFirstOwner*nMissionAnnual;

    %% Productivity

    P(nYear) = rN(nYear)/(cFixed(nYear)+cVariableN(nYear));
end

P