% Calculate productivity for 011-010-01-010 on Malmö-Göteborg with GCW 70t
% Tractor engine - D16, Electric motors used - M01, Buffer size - B01

clear all
close all
clc

%% Currency conversion

USDtoEUR = 0.76;

%% Simulation outputs

C = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsWithPredictiveControl/11/C.mat');
B(1) = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsWithPredictiveControl/11/U0B.mat');
%B(2) = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsWithPredictiveControl/11/U1B.mat');
%B(3) = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsWithPredictiveControl/11/U2B.mat');
%B(4) = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsWithPredictiveControl/11/U3B.mat');

%% Combination and mission data

nUnits = 4;
nEAxles = [0 0 0];
nEUnits = size(nEAxles,2);

mBattery = [0 0 0]; % Battery B01 on all units
mMotor = [0 0 0];
mEAxle = mBattery+mMotor;
deltaMAxle = sum(mEAxle);

nFirstOwner=5;

dMission = C.positionOverMission(end)/1000;    % km
nMissionAnnual = 2*365;

sizeBattery = [0 0 0];

%% Mission revenues

revUnitFreight = 0.06*USDtoEUR; % [12]

mTractor = 9000;
mSemiTrailer1 = 7000;
mSemiTrailer2 = 7000;
mDolly = 3000;

tractorAxleLoads = [7094, 15340/2*[1 1]];   
firstSemiTrailerAxleLoads = 22100/3*[1 1 1];    
dollyAxleLoads =    10500/2*[1 1];    
secondSemiTrailerAxleLoads = 13480/3*[1 1 1];   

GVW = mTractor+mSemiTrailer1+mDolly+mSemiTrailer2;
GCW = sum(tractorAxleLoads)+sum(firstSemiTrailerAxleLoads)+sum(dollyAxleLoads)+sum(secondSemiTrailerAxleLoads);

mPayloadGross = GCW-GVW;
mPayloadNet = mPayloadGross - deltaMAxle;

revMission = revUnitFreight*(mPayloadNet/1000)*dMission;
revAnnual = revMission * nMissionAnnual;
rN = revAnnual*nFirstOwner;

%% Fixed Costs

pTractorBase = 130000*USDtoEUR;
deltaPowertrain = 15000;    % D16-D11
pDolly = 40000*USDtoEUR;
pST1 = 70000*USDtoEUR;
pST2 = 70000*USDtoEUR;
pEAxle = [30000 30000 30000];   % One eAxle per unit, EUR

pTractorNet = pTractorBase + deltaPowertrain;
cFixedConv = pTractorNet+pDolly+pST1+pST2;

pBatterykWh = 436*USDtoEUR;
pBattery = sizeBattery*pBatterykWh;

cFixedElec = pBattery+pEAxle;

cFixed = cFixedConv+sum(cFixedElec);

%% Operational costs

rMntDriver = 6/35;
rTyreDriver = 7/35;
rTollDriver = 14/35;

cDriverHour = 24;   % EUR
cFuelLitre = 1.468; % EUR
pElecKWh = 0.21;    % EUR

tMission = size(C.positionOverMission,2)/3600; % hours
vFuel = C.fuelConsumptionOverMission(end);

% for i=2:nUnits
%     startState = B(1,i).stateOfBufferOverMission(1);
%     endState = B(1,i).stateOfBufferOverMission(end);
%     chargeConsumedUnit(i) = (startState - endState)*sizeBattery(1,i-1);
% end
% eRecharge = sum(chargeConsumedUnit)*3.6/3.6e6;  %kWh
eRecharge = 0;

cDriver = tMission*cDriverHour;
cFuel = vFuel*cFuelLitre;
cMnt = cDriver*rMntDriver;
cTyres = cDriver*rTyreDriver;
cTolls = cDriver*rTollDriver;
cElec = eRecharge*pElecKWh;

cVariableMission = cDriver+cFuel+cMnt+cTyres+cTolls+cElec;
cVariableN = cVariableMission*nFirstOwner*nMissionAnnual;

%% Productivity

P = rN/(cFixed+cVariableN);