close all
clear all
clc

% Effect of engine downsizing on mission performance

% Constant combination - 011-010-01-010, D13, GCW 70t
% Engine downsizing affects overall productivity, fuel consumption and
% charge consumption

fileNumber = [87,91,95];
engineSize=['D11', 'D13', 'D16'];

%% Load simulation output files
for i=1:size(fileNumber,1)
    for j=1:size(fileNumber,2)   % Engine size - D11, D13 or D16
        path='Output/';
        fileNameNumber = int2str(fileNumber(i,j));
        fileName='/C';
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        C(i,j) = load(fullPath);
    end
end

for i=1:size(fileNumber,1)
    for j=1:size(fileNumber,2)  % Engine size - D11, D13 or D16
        path='Output/';
        fileNameNumber = int2str(fileNumber(i,j));
        for k=1:4
            switch(k)
                case 1
                    fileName='/U0B';
                    extension='.mat';
                    fullPath=strcat(path, fileNameNumber,fileName, extension);
                    B0(i,j) = load(fullPath);
                case 2
                    fileName='/U1B';
                    extension='.mat';
                    fullPath=strcat(path, fileNameNumber,fileName, extension);
                    B1(i,j) = load(fullPath);
                case 3
                    fileName='/U2B';
                    extension='.mat';
                    fullPath=strcat(path, fileNameNumber,fileName, extension);
                    B2(i,j) = load(fullPath);
                case 4
                    fileName='/U3B';
                    extension='.mat';
                    fullPath=strcat(path, fileNameNumber,fileName, extension);
                    B3(i,j) = load(fullPath);
            end
        end
    end
end

for i=1:size(fileNumber,1)
    for j=1:size(fileNumber,2)  % Engine size - D11, D13 or D16
        missionTime(i,j) = size(C(i,j).positionOverMission,2);
        fuelConsumed(i,j) = 600-B0(i,j).bufferLevelOverMission(end);
        chargeOverMission = C(i,j).chargeConsumptionOverMission;
        chargeConsumed(i,j) = sum(chargeOverMission);
        combinationStartability(i,j) = C(i,j).combinationStartability;
    end
end

%% Plot mission time

figure('name', 'Mission time vs engine size (with electric propulsion) (GCW=70t)');
plot(missionTime/3600, '-*')
title('Mission time vs engine size (with electric propulsion) (GCW=70t)');
xlabel('Engine size'), ylabel('Engine size');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'D11', 'D13', 'D16'});
saveas(gcf, 'Plots/Engine downsizing/Mission time vs engine size (with electric propulsion) (GCW=70t).pdf');

%% Plot fuel consumed

figure('name', 'Fuel consumption vs engine size (with electric propulsion) (GCW=70t)');
plot(fuelConsumed, '-*')
title('Fuel consumption vs engine size (with electric propulsion) (GCW=70t)');
xlabel('Engine size'), ylabel('Fuel consumption (litres)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'D11', 'D13', 'D16'});
saveas(gcf,'Plots/Engine downsizing/Fuel consumption vs engine size (with electric propulsion).pdf');

%% Plot charge consumed

figure('name', 'Charge consumption vs engine size (with electric propulsion) (GCW=70t)');
plot(chargeConsumed, '-*')
title('Charge consumption vs engine size (with electric propulsion) (GCW=70t)');
xlabel('Engine size'), ylabel('Fuel consumption (litres)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'D11', 'D13', 'D16'});
saveas(gcf,'Plots/Engine downsizing/Fuel consumption vs engine size (with electric propulsion).pdf');

%% Plot startability

figure('name', 'Combination Startability vs engine size (with electric propulsion) (GCW=70t)');
plot(combinationStartability, '-*')
title('Combination Startability vs engine size (with electric propulsion) (GCW=70t)');
xlabel('Engine size'), ylabel('Combination Startability (degrees)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'D11', 'D13', 'D16'});
saveas(gcf,'Plots/Engine downsizing/Combination startability vs engine size (with electric propulsion).pdf');

%% Plot mission speed

for i=1:size(fileNumber,1)
    figureName='Mission speed vs position (011-010-01-010, D13, GCW 70t)';
    figure('name', figureName);
    for j=1:size(fileNumber,2)
        plot(C(i,j).positionOverMission, C(i,j).speedOverMission);
        hold all
    end
    title(figureName);
    legend('D11', 'D13', 'D16','Location','Best');
    xlabel('Position over mission (m)');
    ylabel('Speed over mission (m/s)');
    saveas(gcf,strcat('Plots/Engine downsizing/',figureName,'.pdf'));
end

%% Clear unnecessary variables from workspace
clearvars i j path fullPath fileNameNumber fileName extension;
