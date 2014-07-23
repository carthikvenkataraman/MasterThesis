close all
clear all
clc

% Effect of buffer size on mission performance

% Constant combination - 011-010-01-010, D13, GCW 70t
% Buffer size affects fuel consumption and charge consumption

fileNumber = [172,173,174];
bufferSize=['Battery 1', 'Battery 2', 'Battery 3'];

%% Load simulation output files
for i=1:size(fileNumber,1)
    for j=1:size(fileNumber,2)   % Buffer size 
        path='Output/';
        fileNameNumber = int2str(fileNumber(i,j));
        fileName='/C';
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        C(i,j) = load(fullPath);
    end
end

for i=1:size(fileNumber,1)
    for j=1:size(fileNumber,2)  % Buffer size 
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
    for j=1:size(fileNumber,2)  % Buffer size 
        missionTime(i,j) = size(C(i,j).positionOverMission,2);
        fuelConsumed(i,j) = 600-B0(i,j).bufferLevelOverMission(end);
        chargeOverMission = C(i,j).chargeConsumptionOverMission;
        chargeConsumed(i,j) = sum(chargeOverMission);
        combinationStartability(i,j) = C(i,j).combinationStartability;
    end
end

%% Plot mission time

figure('name', 'Mission time vs buffer size (D13, GCW=70t)');
plot(missionTime/3600, '-*')
title('Mission time vs buffer size (D13, GCW=70t)');
xlabel('Buffer size'), ylabel('Mission time (hours)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'Battery 1', 'Battery 2', 'Battery 3'});
saveas(gcf, 'Plots/Effect of buffer size/Mission time vs buffer size (D13, GCW=70t).pdf');

%% Plot fuel consumed

figure('name', 'Fuel consumption vs buffer size (D13, GCW=70t)');
plot(fuelConsumed, '-*')
title('Fuel consumption vs buffer size (D13, GCW=70t)');
xlabel('Buffer size'), ylabel('Fuel consumption (litres)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'Battery 1', 'Battery 2', 'Battery 3'});
saveas(gcf,'Plots/Effect of buffer size/Fuel consumption vs buffer size (with electric propulsion).pdf');

%% Plot charge consumed

figure('name', 'Charge consumption vs buffer size (D13, GCW=70t)');
plot(chargeConsumed, '-*')
title('Charge consumption vs buffer size (D13, GCW=70t)');
xlabel('Buffer size'), ylabel('Fuel consumption (litres)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'Battery 1', 'Battery 2', 'Battery 3'});
saveas(gcf,'Plots/Effect of buffer size/Fuel consumption vs buffer size (with electric propulsion).pdf');

%% Plot startability

figure('name', 'Combination Startability vs buffer size (D13, GCW=70t)');
plot(combinationStartability, '-*')
title('Combination Startability vs buffer size (D13, GCW=70t)');
xlabel('Buffer size'), ylabel('Combination Startability (degrees)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'Battery 1', 'Battery 2', 'Battery 3'});
saveas(gcf,'Plots/Effect of buffer size/Combination startability vs buffer size (with electric propulsion).pdf');

%% Plot mission speed

for i=1:size(fileNumber,1)
    figureName='Mission speed vs position (011-010-01-010, D13, GCW 70t)';
    figure('name', figureName);
    for j=1:size(fileNumber,2)
        plot(C(i,j).positionOverMission, C(i,j).speedOverMission);
        hold all
    end
    title(figureName);
    legend('Battery 1', 'Battery 2', 'Battery 3','Location','Best');
    xlabel('Position over mission (m)');
    ylabel('Speed over mission (m/s)');
    saveas(gcf,strcat('Plots/Effect of buffer size/',figureName,'.pdf'));
end

%% Clear unnecessary variables from workspace
clearvars i j path fullPath fileNameNumber fileName extension;
