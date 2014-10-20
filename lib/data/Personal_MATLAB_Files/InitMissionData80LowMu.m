clear all;
% close all;
clc

delete('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat');
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat');

%% Currency conversion

USDtoEUR = 0.76;

%% Mission route data

missionRouteData = csvread('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData.csv');

targetSpeed=missionRouteData(:,1)';              % #1 m/s
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'targetSpeed', '-append');
longitudinalPosition=missionRouteData(:,2)';     % #2 m
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'longitudinalPosition', '-append');
roadGradientInRadians=missionRouteData(:,3)';    % #3 rad
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'roadGradientInRadians', '-append');
elevation(1)=0;
for i=2:size(roadGradientInRadians,2)
    elevation(i)=(longitudinalPosition(i)-longitudinalPosition(i-1))*tan(roadGradientInRadians(i-1))+elevation(i-1); % (m)
end

%% Mission payload data

revenuePerTonPerKM = 0.0529*USDtoEUR;            % #4 Euro / ton-km
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'revenuePerTonPerKM', '-append');

%% Mission cost data

baseTractorCost = 98800*USDtoEUR*(1.03^15);     
semiTrailerCost = 53200*USDtoEUR*(1.03^15);     
dollyCost = 30400*USDtoEUR*(1.03^15);          
unitCosts = [baseTractorCost, semiTrailerCost, dollyCost, semiTrailerCost];  % #5 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'unitCosts', '-append');
powertrainPremium = [10000, 15000];
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'powertrainPremium', '-append');

motorCosts = [30000, 30000, 30000];    % #6 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'motorCosts', '-append');
batteryCosts = [4000, 40000, 72000];   % #7 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'batteryCosts', '-append');

driverHourlyRates = 24;            % #8 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'driverHourlyRates', '-append');
fuelCosts = 1.468;                      % #9 Euro / litre
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'fuelCosts', '-append');
electricityRates = 0.21;               % #10 Euro / kWh
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'electricityRates', '-append');

%% Combination loading details

kerbUnitWeight = [9000, 7000, 3000, 4000];  % #11 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'kerbUnitWeight', '-append');
tractorAxleLoads = [7094, 15340/2*[1 1]];    % #12 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'tractorAxleLoads', '-append');
firstSemiTrailerAxleLoads = 22400/3*[1 1 1];    % #13 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'firstSemiTrailerAxleLoads', '-append');
dollyAxleLoads =    12500/2*[1 1];    % #14 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'dollyAxleLoads', '-append');
secondSemiTrailerAxleLoads = 22400/3*[1 1 1];    % #15 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'secondSemiTrailerAxleLoads', '-append');
grossCombinationWeight = sum(tractorAxleLoads)+sum(firstSemiTrailerAxleLoads)+sum(dollyAxleLoads)+sum(secondSemiTrailerAxleLoads); % #16 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'grossCombinationWeight', '-append');

%% Tractor, tire and air parameters

frontalArea = 9.7;  % #17 m2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'frontalArea', '-append');
aerodynamicDragCoefficient = 0.6;   % #18
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'aerodynamicDragCoefficient', '-append');
densityOfAir = 1.184; % #19 kg/m3
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'densityOfAir', '-append');
rollingResistanceCoefficient = 0.005;   % #20
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'rollingResistanceCoefficient', '-append');
wheelRadiusEachUnit = 0.502*[1 1 1 1];    % #21 m
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'wheelRadiusEachUnit', '-append');
coefficientOfFrictionEachUnit = [0.3 0.3 0.3 0.3];       % #22
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'coefficientOfFrictionEachUnit', '-append');
gravitationalAcceleration = 9.81;       % #23 m/s^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'gravitationalAcceleration', '-append');

%% Powertrain inertia parameters

tireInertia = 4*35; % 24 kgm^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'tireInertia', '-append');
axleInertia = 2.14; % 25 kgm^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'axleInertia', '-append');
propShaftInertia = 2; % 26 kgm^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'propShaftInertia', '-append');
clutchInertia = 3.8; % 27 kgm^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'clutchInertia', '-append');
clear missionRouteData;

zeta = InitReferenceSoC(targetSpeed, longitudinalPosition, roadGradientInRadians);
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'zeta', '-append'); % 28

%% Motor and battery masses

motorMasses = [43 43 43];
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'motorMasses', '-append');  % 29
batteryMasses = [129 405 709];
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'batteryMasses', '-append');    % 30

%% More mission data
nMissionDaily = 2;
nMissionAnnual = nMissionDaily*365;
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'nMissionAnnual', '-append');    % 31
nFirstOwner=5;
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'nFirstOwner', '-append');    % 32

rMntDriver = 6/35;
rTyreDriver = 7/35;
rTollDriver = 14/35;
otherCostRatios = [rMntDriver, rTyreDriver, rTollDriver];
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData80LowMu.mat', 'otherCostRatios', '-append');    % 33

close all