clear all
close all
clc

% Influence of axle load - constant GCW, different engines
% Each column corresponds to a different axle being propelled
% Column 1 - one axle on unit 1 - axle load 7366 kg
% Column 2 - one axle on unit 2 - axle load 5250 kg
% Column 3 - one axle on unit 3 - axle load 4493 kg

fileNumber=[15,27,39;...    % D11
            19,31,43;...    % D13
            23,35,47];      % D16
engineSize=[11,13,16];
battery=load('data/Battery02.mat');

%% Load simulation output files
for i=1:size(fileNumber,1)   % Engine size - D11, D13 or D16
    for j=1:size(fileNumber,2)   % Axle load (which unit's single axle is propelled)
        path='Output/';
        fileNameNumber = int2str(fileNumber(i,j));
        fileName='/C';
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        C(i,j) = load(fullPath);
    end
end

for i=1:size(fileNumber,1)   % Engine size - D11, D13 or D16
    for j=1:size(fileNumber,2)   % Number of axles
        path='Output/';
        fileNameNumber = int2str(fileNumber(i,j));
        fileName='/U0B';
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        B(i,j) = load(fullPath);
    end
end

for i=1:size(fileNumber,1)   % Engine size - D11, D13 or D16
    for j=1:size(fileNumber,2)   % Number of axles
        path='Output/';
        fileNameNumber = int2str(fileNumber(i,j));
        fileName=strcat('/U',int2str(j), 'B');
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        EB(i,j) = load(fullPath);
    end
end

for i=1:size(fileNumber,1)
    for j=1:size(fileNumber,2)
        missionTime(i,j) = size(C(i,j).positionOverMission,2);
        fuelConsumed(i,j) = 600-B(i,j).bufferLevelOverMission(end);
        chargeConsumed(i,j)= battery.bufferData(1) - EB(i,j).bufferLevelOverMission(end);
        combinationStartability(i,j) = C(i,j).combinationStartability;
    end
end

%% Plot mission time

figure('name', 'Mission time vs axle load (GCW=70t)');
plot(missionTime/3600)
legend('Axle load 7.4t', 'Axle load 5.25t', 'Axle load 4.5t','Location','Best');
title('Mission time vs axle load (GCW=70t)');
xlabel('Engine size'), ylabel('Mission time (hours)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'D11','D13','D16'});
saveas(gcf, 'Plots/Effect of axle load/Mission time vs axle load and engine size.pdf');

%% Plot fuel consumed

figure('name', 'Fuel consumption vs axle load (GCW=70t)');
plot(fuelConsumed)
legend('Axle load 7.4t', 'Axle load 5.25t', 'Axle load 4.5t','Location','Best');
title('Fuel consumption vs axle load (GCW=70t)');
xlabel('Engine size'), ylabel('Fuel consumption (litres)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'D11','D13','D16'});
saveas(gcf,'Plots/Effect of axle load/Fuel consumption vs axle load and engine size.pdf');

%% Plot startability

figure('name', 'Combination Startability vs axle load (GCW=70t)');
plot(combinationStartability)
legend('Axle load 7.4t', 'Axle load 5.25t', 'Axle load 4.5t','Location','Best');
title('Combination Startability vs axle load (GCW=70t)');
xlabel('Engine size'), ylabel('Combination Startability (degrees)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'D11','D13','D16'});
saveas(gcf,'Plots/Effect of axle load/Combination startability vs axle load and engine size.pdf');

%% Plot mission speed

for i=1:size(fileNumber,1)
    figureName=strcat('Mission speed vs position (constant GCW = 70t, engine - D',int2str(engineSize(i)),')');
    figure('name', figureName);
    for j=1:size(fileNumber,2)
        plot(C(i,j).positionOverMission, C(i,j).speedOverMission);
        hold all
    end
    title(figureName);
    legend('Axle load 7.4t', 'Axle load 5.25t', 'Axle load 4.5t','Location','Best');
    xlabel('Position over mission (m)');
    ylabel('Speed over mission (m/s)');
    saveas(gcf,strcat('Plots/Effect of axle load/',figureName,'.pdf'));
end

figureName=strcat('Mission speed vs position (constant GCW = 70t)'); % = ', int2str(gcw(i)), 't)');
figure('name', figureName);
for i=1:size(fileNumber,1)
    subplot(size(fileNumber,1),1,i);
    for j=1:size(fileNumber,2)
        plot(C(i,j).positionOverMission, C(i,j).speedOverMission);
        hold all
    end
    title(strcat(figureName, ' engine - D',int2str(engineSize(i)), ')'));
    legend('Axle load 7.4t', 'Axle load 5.25t', 'Axle load 4.5t','Location','Best');
    xlabel('Position over mission (m)');
    ylabel('Speed over mission (m/s)');
    saveas(gcf,strcat('Plots/Effect of axle load/',figureName,'.pdf'));
end

%% Clear unnecessary variables from workspace
clearvars i j path fullPath fileNameNumber fileName extension;
