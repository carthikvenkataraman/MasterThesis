clear all;
close all;
clc

%% Mission route data

missionRouteData = csvread('/home/karthik/Documents/GitHubRepos/MasterThesis/lib/data/MissionData.csv');
targetSpeed=missionRouteData(:,1)';              % #1 m/s
% save('MissionData.mat', 'targetSpeed', '-append');
longitudinalPosition=missionRouteData(:,2)';     % #2 m
% save('MissionData.mat', 'longitudinalPosition', '-append');
roadGradientInRadians=missionRouteData(:,3)';    % #3 rad
% save('MissionData.mat', 'roadGradientInRadians', '-append');
elevation(1)=0;
for i=2:size(roadGradientInRadians,2)
    elevation(i)=(longitudinalPosition(i)-longitudinalPosition(i-1))*tan(roadGradientInRadians(i-1))+elevation(i-1); % (m)
end

% for i=1:size(roadGradientInRadians,2)
%     if(roadGradientInRadians(i)<0 && roadGradientInRadians(i)>0.013)
%         SoCReference(i) = 0.5;
%     elseif(roadGradientInRadians(i)>0 && roadGradientInRadians(i)<0.013)
%         SoCReference(i) = 0.5;
%     elseif(roadGradientInRadians(i)>0.013 && roadGradientInRadians(i)<0.026)
%         SoCReference(i) = 0.4;
%     elseif(roadGradientInRadians(i)>0.026 && roadGradientInRadians(i)<0.039)
%         SoCReference(i) = 0.3;
%     elseif(roadGradientInRadians(i)>0.039 && roadGradientInRadians(i)<0.052)
%         SoCReference(i) = 0.2;
%     else
%         SoCReference(i) = 0.1;
%     end
% end


% %% Mission payload data
%
% payloadDensity = 2000;                  % #4 kg/m3
% save('MissionData.mat', 'payloadDensity', '-append');
% revenuePerTonPayload = 3000;            % #5 Euro / ton
% save('MissionData.mat', 'revenuePerTonPayload', '-append');
% ladenCGHeight = 1.8;                    % #6 m
% save('MissionData.mat', 'ladenCGHeight', '-append');
%
% %% Mission cost data
%
% baseTractorCost = 90000;     % #7 Euro
% save('MissionData.mat', 'baseTractorCost', '-append');
% semiTrailerCost = 20000;     % #8 Euro
% save('MissionData.mat', 'semiTrailerCost', '-append');
% dollyCost = 15000;           % #9 Euro
% save('MissionData.mat', 'dollyCost', '-append');
%
% motorCosts = [2000, 3000, 5000];    % #10 Euro
% save('MissionData.mat', 'motorCosts', '-append');
% powerElectronicsCosts = [2000, 3000, 5000]; % #11 Euro
% save('MissionData.mat', 'powerElectronicsCosts', '-append');
% bufferCosts = [5000, 6000, 8000];   % #12 Euro
% save('MissionData.mat', 'bufferCosts', '-append');
% mechanicalDriveCosts = [1500, 3000, 5000];  % #13 Euro
% save('MissionData.mat', 'mechanicalDriveCosts', '-append');
%
% driverHourlyRates = 500;            % #14 Euro
% save('MissionData.mat', 'driverHourlyRates', '-append');
% fuelCosts = 3;                      % #15 Euro / litre
% save('MissionData.mat', 'fuelCosts', '-append');
% electricityRates = 0.2;               % #16 Euro / kWh
% save('MissionData.mat', 'electricityRates', '-append');
% annuitPercentage = 10;  % #17
% save('MissionData.mat', 'annuitPercentage', '-append');
%
% %% Combination loading details
%
% grossUnitWeight = [9000,26000,3000,14000];  % #18 kg
% save('MissionData.mat', 'grossUnitWeight', '-append');
% % ALWAYS CHANGE INDIVIDUAL UNIT AXLE LOADS WHEN YOU CHANGE EACH GVW /
% % GCW!!!
% tractorAxleLoads = [7094, 15340/2, 15340/2];    % #19 kg
% save('MissionData.mat', 'tractorAxleLoads', '-append');
% firstSemiTrailerAxleLoads = 22100/3*[1 1 1];    % #20 kg
% save('MissionData.mat', 'firstSemiTrailerAxleLoads', '-append');
% dollyAxleLoads = 10500/2*[1 1];    % #21 kg
% save('MissionData.mat', 'dollyAxleLoads', '-append');
% secondSemiTrailerAxleLoads = 13480/3*[1 1 1];    % #22 kg
% save('MissionData.mat', 'secondSemiTrailerAxleLoads', '-append');
% grossCombinationWeight = sum(tractorAxleLoads)+sum(firstSemiTrailerAxleLoads)+sum(dollyAxleLoads)+sum(secondSemiTrailerAxleLoads); % #23 kg
% save('MissionData.mat', 'grossCombinationWeight', '-append');
% payloadEachUnit = [0,24000,0,10000];    % #24 kg
% save('MissionData.mat', 'payloadEachUnit', '-append');
%
% %% Tractor, tire and air parameters
%
% frontalArea = 9.7;  % #25 m2
% save('MissionData.mat', 'frontalArea', '-append');
% aerodynamicDragCoefficient = 0.6;   % #26
% save('MissionData.mat', 'aerodynamicDragCoefficient', '-append');
% densityOfAir = 1.184; % #27 kg/m3
% save('MissionData.mat', 'densityOfAir', '-append');
% rollingResistanceCoefficient = 0.005;   % #28
% save('MissionData.mat', 'rollingResistanceCoefficient', '-append');
% wheelRadiusEachUnit = 0.502*[1 1 1 1];    % #29 m
% save('MissionData.mat', 'wheelRadiusEachUnit', '-append');
% coefficientOfFrictionEachUnit = [0.77 0.77 0.77 0.77];       % #30
% save('MissionData.mat', 'coefficientOfFrictionEachUnit', '-append');
% gravitationalAcceleration = 9.81;       % #31 m/s^2
% save('MissionData.mat', 'gravitationalAcceleration', '-append');
%
% %% Powertrain inertia parameters
%
% tireInertia = 4*35; % 31 kgm^2
% save('MissionData.mat', 'tireInertia', '-append');
% axleInertia = 2.14; % 32 kgm^2
% save('MissionData.mat', 'axleInertia', '-append');
% propShaftInertia = 2; % 33 kgm^2
% save('MissionData.mat', 'propShaftInertia', '-append');
% clutchInertia = 3.8; % 34 kgm^2
% save('MissionData.mat', 'clutchInertia', '-append');
% clear missionRouteData;
%
% save('MissionData.mat');