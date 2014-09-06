clear all;
% close all;
clc

delete('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat');
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat');

%% Currency conversion

USDtoEUR = 0.76;

%% Mission route data

missionRouteData = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70B.mat');
% missionRouteData(1:10,3)=0;
% missionRouteData(11:end,3)=0.025;

targetSpeed=[missionRouteData.targetSpeed missionRouteData.targetSpeed];              % #1 m/s
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'targetSpeed', '-append');
longitudinalPosition=missionRouteData.longitudinalPosition;
for i=1:size(missionRouteData.longitudinalPosition,2)-1
    longitudinalPosition(i+size(missionRouteData.longitudinalPosition,2))=...
        missionRouteData.longitudinalPosition(end)+missionRouteData.longitudinalPosition(i+1);     % #2 m
end
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'longitudinalPosition', '-append');
roadGradientInRadians=[missionRouteData.roadGradientInRadians missionRouteData.roadGradientInRadians];    % #3 rad
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'roadGradientInRadians', '-append');
elevation(1)=0;
for i=2:size(roadGradientInRadians,2)-1
    elevation(i)=(longitudinalPosition(i)-longitudinalPosition(i-1))*tan(roadGradientInRadians(i-1))+elevation(i-1); % (m)
end

%% Mission payload data

revenuePerTonPerKM = 0.06*USDtoEUR;            % #4 Euro / ton-km
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'revenuePerTonPerKM', '-append');

%% Mission cost data

baseTractorCost = 130000*USDtoEUR;     
semiTrailerCost = 70000*USDtoEUR;     
dollyCost = 40000*USDtoEUR;          
unitCosts = [baseTractorCost, semiTrailerCost, dollyCost, semiTrailerCost];  % #5 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'unitCosts', '-append');
powertrainPremium = [10000, 15000];
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'powertrainPremium', '-append');

motorCosts = [30000, 30000, 30000];    % #6 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'motorCosts', '-append');
batteryCosts = [1657, 16570, 29820];   % #7 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'batteryCosts', '-append');

driverHourlyRates = 24;            % #8 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'driverHourlyRates', '-append');
fuelCosts = 1.468;                      % #9 Euro / litre
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'fuelCosts', '-append');
electricityRates = 0.21;               % #10 Euro / kWh
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'electricityRates', '-append');

%% Combination loading details

kerbUnitWeight = [9000, 7000, 3000, 4000];  % #11 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'kerbUnitWeight', '-append');
tractorAxleLoads = [7094, 15340/2*[1 1]];    % #12 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'tractorAxleLoads', '-append');
firstSemiTrailerAxleLoads = 22100/3*[1 1 1];    % #13 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'firstSemiTrailerAxleLoads', '-append');
dollyAxleLoads =    10500/2*[1 1];    % #14 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'dollyAxleLoads', '-append');
secondSemiTrailerAxleLoads = 13480/3*[1 1 1];    % #15 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'secondSemiTrailerAxleLoads', '-append');
grossCombinationWeight = sum(tractorAxleLoads)+sum(firstSemiTrailerAxleLoads)+sum(dollyAxleLoads)+sum(secondSemiTrailerAxleLoads); % #16 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'grossCombinationWeight', '-append');

%% Tractor, tire and air parameters

frontalArea = 9.7;  % #17 m2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'frontalArea', '-append');
aerodynamicDragCoefficient = 0.6;   % #18
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'aerodynamicDragCoefficient', '-append');
densityOfAir = 1.184; % #19 kg/m3
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'densityOfAir', '-append');
rollingResistanceCoefficient = 0.005;   % #20
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'rollingResistanceCoefficient', '-append');
wheelRadiusEachUnit = 0.502*[1 1 1 1];    % #21 m
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'wheelRadiusEachUnit', '-append');
coefficientOfFrictionEachUnit = [0.77 0.77 0.77 0.77];       % #22
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'coefficientOfFrictionEachUnit', '-append');
gravitationalAcceleration = 9.81;       % #23 m/s^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'gravitationalAcceleration', '-append');

%% Powertrain inertia parameters

tireInertia = 4*35; % 24 kgm^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'tireInertia', '-append');
axleInertia = 2.14; % 25 kgm^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'axleInertia', '-append');
propShaftInertia = 2; % 26 kgm^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'propShaftInertia', '-append');
clutchInertia = 3.8; % 27 kgm^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'clutchInertia', '-append');
clear missionRouteData;

zeta = InitReferenceSoCAlt(targetSpeed, longitudinalPosition, roadGradientInRadians);
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'zeta', '-append'); % 28

%% Motor and battery masses

motorMasses = [43 43 43];
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'motorMasses', '-append');  % 29
batteryMasses = [167 524 918];
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'batteryMasses', '-append');    % 30

%% More mission data
nMissionDaily = 2;
nMissionAnnual = nMissionDaily*365;
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'nMissionAnnual', '-append');    % 31
nFirstOwner=5;
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'nFirstOwner', '-append');    % 32

rMntDriver = 6/35;
rTyreDriver = 7/35;
rTollDriver = 14/35;
otherCostRatios = [rMntDriver, rTyreDriver, rTollDriver];
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70C.mat', 'otherCostRatios', '-append');    % 33

close all