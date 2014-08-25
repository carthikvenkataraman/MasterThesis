clear all
close all
clc

load('MissionData70.mat');
elevation(1)=0;
for i=1:size(longitudinalPosition,2)-1
    delta_x = longitudinalPosition(i+1)-longitudinalPosition(i);
    elevation(i+1)=elevation(i)+delta_x*tan(roadGradientInRadians(i));
end

maxElevation = max(elevation);
normalisedElevation = elevation/max(elevation);

figure('name', 'Normalised elevation and Buffer SoC vs longitudinal position');
plot(longitudinalPosition, normalisedElevation);
xlabel('Longitudinal Position (m)');
ylabel('Normalised Elevation (m/m)');
title('Normalised elevation and Buffer SoC vs longitudinal position');
hold on;

load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/Output/139/C.mat');
load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/Output/139/U1B.mat');

plot(positionOverMission, stateOfBufferOverMission,'r');
ylabel('State of Charge of Buffer on First Semitrailer & Normalised Elevation (m/m)');
legend('Normalised Elevation','State of Charge of Buffer on First Semitrailer');

saveas(gcf, '/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/Plots/SoCVsElevation.pdf');