clear all;
close all;
clc

delete('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat');
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat');

%% Currency conversion

USDtoEUR = 0.76;

%% Mission route data

missionRouteData = csvread('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData.csv');
% missionRouteData(1:10,3)=0;
% missionRouteData(11:end,3)=0.025;

targetSpeed=missionRouteData(:,1)';              % #1 m/s
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'targetSpeed', '-append');
longitudinalPosition=missionRouteData(:,2)';     % #2 m
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'longitudinalPosition', '-append');
roadGradientInRadians=missionRouteData(:,3)';    % #3 rad
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'roadGradientInRadians', '-append');
elevation(1)=0;
for i=2:size(roadGradientInRadians,2)
    elevation(i)=(longitudinalPosition(i)-longitudinalPosition(i-1))*tan(roadGradientInRadians(i-1))+elevation(i-1); % (m)
end

%% Mission payload data

revenuePerTonPerKM = 0.0935*USDtoEUR;            % #4 Euro / ton-km
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'revenuePerTonPerKM', '-append');

%% Mission cost data

baseTractorCost = 130000*USDtoEUR*(1.03^15);     
semiTrailerCost = 70000*USDtoEUR*(1.03^15);     
dollyCost = 40000*USDtoEUR*(1.03^15);          
unitCosts = [baseTractorCost, semiTrailerCost, dollyCost, semiTrailerCost];  % #5 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'unitCosts', '-append');
powertrainPremium = [10000, 15000];
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'powertrainPremium', '-append');

motorCosts = [30000, 30000, 30000];    % #6 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'motorCosts', '-append');
batteryCosts = [1853, 18532, 33357];   % #7 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'batteryCosts', '-append');

driverHourlyRates = 35.9477;            % #8 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'driverHourlyRates', '-append');
fuelCosts = 2.2871;                      % #9 Euro / litre
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'fuelCosts', '-append');
electricityRates = 0.3272;               % #10 Euro / kWh
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'electricityRates', '-append');

%% Combination loading details

kerbUnitWeight = [9000, 7000, 3000, 4000];  % #11 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'kerbUnitWeight', '-append');
tractorAxleLoads = [7094, 15340/2*[1 1]];    % #12 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'tractorAxleLoads', '-append');
firstSemiTrailerAxleLoads = 22100/3*[1 1 1];    % #13 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'firstSemiTrailerAxleLoads', '-append');
dollyAxleLoads =    10500/2*[1 1];    % #14 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'dollyAxleLoads', '-append');
secondSemiTrailerAxleLoads = 13480/3*[1 1 1];    % #15 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'secondSemiTrailerAxleLoads', '-append');
grossCombinationWeight = sum(tractorAxleLoads)+sum(firstSemiTrailerAxleLoads)+sum(dollyAxleLoads)+sum(secondSemiTrailerAxleLoads); % #16 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'grossCombinationWeight', '-append');

%% Tractor, tire and air parameters

frontalArea = 9.7;  % #17 m2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'frontalArea', '-append');
aerodynamicDragCoefficient = 0.6;   % #18
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'aerodynamicDragCoefficient', '-append');
densityOfAir = 1.184; % #19 kg/m3
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'densityOfAir', '-append');
rollingResistanceCoefficient = 0.005;   % #20
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'rollingResistanceCoefficient', '-append');
wheelRadiusEachUnit = 0.502*[1 1 1 1];    % #21 m
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'wheelRadiusEachUnit', '-append');
coefficientOfFrictionEachUnit = [0.77 0.77 0.77 0.77];       % #22
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'coefficientOfFrictionEachUnit', '-append');
gravitationalAcceleration = 9.81;       % #23 m/s^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'gravitationalAcceleration', '-append');

%% Powertrain inertia parameters

tireInertia = 4*35; % 24 kgm^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'tireInertia', '-append');
axleInertia = 2.14; % 25 kgm^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'axleInertia', '-append');
propShaftInertia = 2; % 26 kgm^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'propShaftInertia', '-append');
clutchInertia = 3.8; % 27 kgm^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'clutchInertia', '-append');
clear missionRouteData;

zeta = InitReferenceSoC(targetSpeed, longitudinalPosition, roadGradientInRadians);
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'zeta', '-append'); % 28

%% Motor and battery masses

motorMasses = [43 43 43];
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'motorMasses', '-append');  % 29
batteryMasses = [129 405 709];
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'batteryMasses', '-append');    % 30

%% More mission data
nMissionDaily = 2;
nMissionAnnual = nMissionDaily*365;
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'nMissionAnnual', '-append');    % 31
nFirstOwner=5;
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'nFirstOwner', '-append');    % 32

rMntDriver = 6/35;
rTyreDriver = 7/35;
rTollDriver = 14/35;
otherCostRatios = [rMntDriver, rTyreDriver, rTollDriver];
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData70.mat', 'otherCostRatios', '-append');    % 33

close all