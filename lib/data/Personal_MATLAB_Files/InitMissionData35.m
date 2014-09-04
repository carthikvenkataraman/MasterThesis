clear all;
close all;
clc

%% Mission route data

missionRouteData = csvread('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData.csv');
% missionRouteData(1:10,3)=0;
% missionRouteData(11:end,3)=0.025;

targetSpeed=missionRouteData(:,1)';              % #1 m/s
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'targetSpeed', '-append');
longitudinalPosition=missionRouteData(:,2)';     % #2 m
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'longitudinalPosition', '-append');
roadGradientInRadians=missionRouteData(:,3)';    % #3 rad
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'roadGradientInRadians', '-append');
% elevation(1)=0;
% for i=2:size(roadGradientInRadians,2)
%     elevation(i)=(longitudinalPosition(i)-longitudinalPosition(i-1))*tan(roadGradientInRadians(i-1))+elevation(i-1); % (m)
% end

%% Mission payload data

payloadDensity = 2000;                  % #4 kg/m3
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'payloadDensity', '-append');
revenuePerTonPayload = 3000;            % #5 Euro / ton
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'revenuePerTonPayload', '-append');
ladenCGHeight = 1.8;                    % #6 m
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'ladenCGHeight', '-append');

%% Mission cost data

baseTractorCost = 90000;     % #7 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'baseTractorCost', '-append');
semiTrailerCost = 20000;     % #8 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'semiTrailerCost', '-append');
dollyCost = 15000;           % #9 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'dollyCost', '-append');

motorCosts = [2000, 3000, 5000];    % #10 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'motorCosts', '-append');
powerElectronicsCosts = [2000, 3000, 5000]; % #11 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'powerElectronicsCosts', '-append');
bufferCosts = [5000, 6000, 8000];   % #12 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'bufferCosts', '-append');
mechanicalDriveCosts = [1500, 3000, 5000];  % #13 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'mechanicalDriveCosts', '-append');

driverHourlyRates = 500;            % #14 Euro
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'driverHourlyRates', '-append');
fuelCosts = 3;                      % #15 Euro / litre
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'fuelCosts', '-append');
electricityRates = 0.2;               % #16 Euro / kWh
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'electricityRates', '-append');
annuitPercentage = 10;  % #17
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'annuitPercentage', '-append');

%% Combination loading details

grossUnitWeight = [9000,26000];%,3000,14000];  % #18 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'grossUnitWeight', '-append');
% ALWAYS CHANGE INDIVIDUAL UNIT AXLE LOADS WHEN YOU CHANGE EACH GVW /
% GCW!!!
tractorAxleLoads = [7094, 15340/2*[1 1]];    % #19 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'tractorAxleLoads', '-append');
firstSemiTrailerAxleLoads = 22100/3*[1 1 1];    % #20 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'firstSemiTrailerAxleLoads', '-append');
dollyAxleLoads =    10500/2*[1 1];    % #21 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'dollyAxleLoads', '-append');
secondSemiTrailerAxleLoads = 13480/3*[1 1 1];    % #22 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'secondSemiTrailerAxleLoads', '-append');
grossCombinationWeight = sum(tractorAxleLoads)+sum(firstSemiTrailerAxleLoads);%+sum(dollyAxleLoads)+sum(secondSemiTrailerAxleLoads); % #23 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'grossCombinationWeight', '-append');
payloadEachUnit = [0,24000];%,0,10000];    % #24 kg
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'payloadEachUnit', '-append');

%% Tractor, tire and air parameters

frontalArea = 9.7;  % #25 m2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'frontalArea', '-append');
aerodynamicDragCoefficient = 0.6;   % #26
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'aerodynamicDragCoefficient', '-append');
densityOfAir = 1.184; % #27 kg/m3
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'densityOfAir', '-append');
rollingResistanceCoefficient = 0.005;   % #28
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'rollingResistanceCoefficient', '-append');
wheelRadiusEachUnit = 0.502*[1 1];% 1 1];    % #29 m
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'wheelRadiusEachUnit', '-append');
coefficientOfFrictionEachUnit = [0.77 0.77];% 0.77 0.77];       % #30
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'coefficientOfFrictionEachUnit', '-append');
gravitationalAcceleration = 9.81;       % #31 m/s^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'gravitationalAcceleration', '-append');

%% Powertrain inertia parameters

tireInertia = 4*35; % 32 kgm^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'tireInertia', '-append');
axleInertia = 2.14; % 33 kgm^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'axleInertia', '-append');
propShaftInertia = 2; % 34 kgm^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'propShaftInertia', '-append');
clutchInertia = 3.8; % 35 kgm^2
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'clutchInertia', '-append');
clear missionRouteData;

zeta = InitReferenceSoC(targetSpeed, longitudinalPosition, roadGradientInRadians);
save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat', 'zeta', '-append'); % 36

save('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData35.mat');