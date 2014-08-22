clear all
close all
clc

%% Combination used - 011-010-01-010, Machine01, Buffer01

% Influence of GCW and engine size
engineSize=[11,13,16];
GCW=[50,60,70,80];
fileNumber=[79,80,31,81;...   % D11 engine
            82,83,32,84;...   % D13 engine
            85,86,33,87];    % D16 engine
        
%% Load simulation OutputsWithPredictiveControl files

for i=1:size(fileNumber,1)   % Engine size - D11, D13 or D16
    for j=1:size(fileNumber,2)   % Number of axles
        path='OutputsWithPredictiveControl/';
        fileNameNumber = int2str(fileNumber(i,j));
        fileName='/C';
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        C(i,j) = load(fullPath);
    end
end

for i=1:size(fileNumber,1)   % Engine size - D11, D13 or D16
    for j=1:size(fileNumber,2)   % Number of axles
        path='OutputsWithPredictiveControl/';
        fileNameNumber = int2str(fileNumber(i,j));
        fileName='/U0B';
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        B(i,j) = load(fullPath);
    end
end

for i=1:size(fileNumber,1)  % Loop over engine sizes
    for j=1:size(fileNumber,2)  % Loop over GCWs
        missionTime(i,j) = size(C(i,j).positionOverMission,2);
        fuelConsumed(i,j) = 600-B(i,j).bufferLevelOverMission(end);
        combinationStartability(i,j) = C(i,j).combinationStartability;
        mode1(i,j) = sum(C(i,j).operatingModeOverMission==1);
        mode2(i,j) = sum(C(i,j).operatingModeOverMission==2);
        mode3(i,j) = sum(C(i,j).operatingModeOverMission==3);
        mode4(i,j) = sum(C(i,j).operatingModeOverMission==4);
        modeM1(i,j) = sum(C(i,j).operatingModeOverMission==-1);
        modeM2(i,j) = sum(C(i,j).operatingModeOverMission==-2);
        mode0(i,j) = sum(C(i,j).operatingModeOverMission==0);
        missionProductivity(i,j) = C(i,j).missionProductivity;
        chargeConsumed = C(i,j).chargeConsumptionOverMission;
        electricalEnergyConsumed(i,j) = sum(chargeConsumed)*2.4/3.6e6; % kWh
    end
end

%% Plot mission time

figure('name', 'Mission time vs engine size & GCW (011-010-01-010)');
plot(missionTime/3600)
legend('50t','60t', '70t', '80t','Location','Best');
title('Mission time vs engine size & GCW (011-010-01-010)');
xlabel('Engine size'), ylabel('Mission time (hours)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'D11','D13','D16'});
saveas(gcf,'PlotsWithPredictiveControl/EngineSizeAndGCW/MissionTimeVsGCWAndEngineSize.pdf');

%% Plot fuel consumed

figure('name', 'Fuel consumption vs engine size & GCW (011-010-01-010)');
plot(fuelConsumed)
legend('50t','60t', '70t', '80t','Location','Best');
title('Fuel consumption vs engine size & GCW (011-010-01-010)');
xlabel('Engine size'), ylabel('Fuel consumption (litres)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'D11','D13','D16'});
saveas(gcf,'PlotsWithPredictiveControl/EngineSizeAndGCW/FuelConsumptionVsGCWAndEngineSize.pdf');

%% Plot startability

figure('name', 'Combination Startability vs engine size & GCW (011-010-01-010)');
plot(combinationStartability)
legend('50t','60t', '70t', '80t','Location','NorthEast');
title('Combination Startability vs engine size & GCW (011-010-01-010)');
xlabel('Engine size'), ylabel('Combination Startability (degrees)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'D11','D13','D16'});
saveas(gcf,'PlotsWithPredictiveControl/EngineSizeAndGCW/CombinationStartabilityVsGCWAndEngineSize.pdf');

%% Plot mission speed

for i=1:size(fileNumber,1)
    figureName=strcat('MissionSpeedVsPosition(constantGCW=70t,engine-D',int2str(engineSize(i)),')');
    figure('name', figureName);
    for j=1:size(fileNumber,2)
        plot(C(i,j).positionOverMission, C(i,j).speedOverMission);
        hold all
    end
    title(figureName);
    legend('50t','60t', '70t', '80t','Location','Southwest');
    xlabel('Position over mission (m)');
    ylabel('Speed over mission (m/s)');
    saveas(gcf,strcat('PlotsWithPredictiveControl/EngineSizeAndGCW/',figureName,'.pdf'));
end

figureName=strcat('MissionSpeedVsPosition(constantGCW=70t)'); % = ', int2str(gcw(i)), 't)');
figure('name', figureName);
for i=1:size(fileNumber,1)
    subplot(size(fileNumber,1),1,i);
    for j=1:size(fileNumber,2)
        plot(C(i,j).positionOverMission, C(i,j).speedOverMission);
        hold all;
    end
    title(strcat(figureName, ' engine - D',int2str(engineSize(i)), ')'));
    legend('50t','60t', '70t', '80t','Location','Southwest');
    xlabel('Position over mission (m)');
    ylabel('Speed over mission (m/s)');
    saveas(gcf,strcat('PlotsWithPredictiveControl/EngineSizeAndGCW/',figureName,'.pdf'));
end

% Clear unnecessary variables from workspace
clearvars i j path fullPath fileNameNumber fileName extension;