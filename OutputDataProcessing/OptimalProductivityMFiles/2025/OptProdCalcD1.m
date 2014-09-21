clear all
close all
clc

% Combination - 010\000\011\  001\001\000\  100\001\00\  100\001\000\

%% Year & missionData.grossCombinationWeight

year = 2025;
gcw = 80;
gcwCode = 'D';
fileCode = 1;

filePath = strcat('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OptimalCombinationsOutputFilesRevised/',int2str(year),'/',gcwCode,int2str(fileCode),'/');
missionDataFile = strcat('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/MissionDataFiles/', int2str(year),'/MissionData',int2str(gcw),'.mat');
outputFilePath = strcat('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OptimalProductivityOutputs/',int2str(year),'/',gcwCode,int2str(fileCode),'.mat');

%% Simulation outputs

C = load(strcat(filePath,'C.mat'));
B(1) = load(strcat(filePath,'U0B.mat'));
B(2) = load(strcat(filePath,'U1B.mat'));
% B(3) = load(strcat(filePath,'U2B.mat'));
% B(4) = load(strcat(filePath,'U3B.mat'));

dMission = C.positionOverMission(end)/1000;    % km
tMission = size(C.positionOverMission,2)/3600; % hours
vFuel = C.fuelConsumptionOverMission(end);     % litres 

%% Use productivity parameters from Mission Data file

missionData = load(missionDataFile);

%% Combination and mission data

batterySizes = [5,50,90];    % kWh
sizeBattery = [batterySizes(1) batterySizes(1) batterySizes(1)];    % SIZE OF BATTERY 

%% ------CHANGE BELOW DATA TO CHANGE CONFIGURATION ----------------------------------------------------------------------
nUnits = 4;
nEAxles = [3 0 0];      % For motor calculation
battIndex = [1 0 0];    % For battery calculation
nEUnits = size(nEAxles,2);

%% ------CHANGE BELOW DATA FOR N_FIRST_OWNER AND TRIPS ------------------------------------------------------------------

nFirstOwner=5;
nMissionDaily = 2;
nMissionAnnual = nMissionDaily*365;

%% Productivity calculcation

%% --- Mission revenues----------------------------------------------------------------------------------------------

revUnitFreight = missionData.revenuePerTonPerKM; % [12] CHANGE IF REVENUE PER UNIT FREIGHT CHANGES

mBattery = missionData.batteryMasses(1)*battIndex;
mMotor = 43*nEAxles;
mEAxle = mBattery+mMotor;
deltaMAxle = sum(mEAxle);
GVW=sum(missionData.kerbUnitWeight);

mPayloadGross = missionData.grossCombinationWeight-GVW;
mPayloadNet = mPayloadGross - deltaMAxle;    

revMission = revUnitFreight*(mPayloadNet/1000)*dMission;
revAnnual = revMission * nMissionAnnual;
rN = revAnnual*nFirstOwner;

%% --- Fixed Costs-----------------------------------------------------------------------------------------------------

% Base combination
cFixedConv = sum(missionData.unitCosts);

% Electrification
pEMotor = missionData.motorCosts(1);
pEAxle = pEMotor*nEAxles;   
pBattery = missionData.batteryCosts(1)*battIndex;
cFixedElec = pBattery+pEAxle;

cFixed = cFixedConv+sum(cFixedElec);

%% --- Operational costs------------------------------------------------------------------------------------------------

%% --- CHANGE BELOW IF RATIOS OF OTHER COSTS TO DRIVER COSTS CHANGES ---------------------------------------------------
rMntDriver = 6/35;
rTyreDriver = 7/35;
rTollDriver = 14/35;

if(any(nEAxles)==1)
    for j=2:3
        if(nEAxles(j-1)~=0)
            deltaSOC = (B(1,j-1).stateOfBufferOverMission(1))-(B(1,j-1).stateOfBufferOverMission(end));
            if(deltaSOC<1)
                deltaSOC=0;
            end
            chargeConsumedUnit(j-1) = deltaSOC*sizeBattery(1,j-1);
        else
            chargeConsumedUnit(j-1) = 0;
        end
    end
    chargeConsumedUnitTotal = sum(chargeConsumedUnit);
else
    chargeConsumedUnitTotal=0;
end
eRecharge = chargeConsumedUnitTotal*3.6/3.6e6;  %kWh

cDriver = tMission*missionData.driverHourlyRates;
cFuel = vFuel*missionData.fuelCosts;
cMnt = cDriver*rMntDriver;
cTyres = cDriver*rTyreDriver;
cTolls = cDriver*rTollDriver;
cElec = eRecharge*missionData.electricityRates;

cVariableMission = cDriver+cFuel+cMnt+cTyres+cTolls+cElec;
cVariableN = cVariableMission*nFirstOwner*nMissionAnnual;

%% Productivity

P = rN/(cFixed+cVariableN);

missionData.grossCombinationWeight
GVW=sum(missionData.kerbUnitWeight)
sum(mBattery)
sum(mMotor)
deltaMAxle
mPayloadNet
missionData.revenuePerTonPerKM
dMission
revMission
revAnnual
rN
cFixedConv
sum(pBattery)
cFixedElec
cFixed
cFuel
eRecharge
cElec
tMission
missionData.driverHourlyRates
cDriver
cVariableMission
cVariableN
P

save(outputFilePath);