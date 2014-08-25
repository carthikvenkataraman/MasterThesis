clear all
close all
clc

% Influence of axle number and engine size
axleConfiguration=['C011-010-00-000','C011-010-01-000','C011-010-01-010','C011-111-11-111'];
chargeControl = ['Step-assigned charge control','Full predictive control'];
fileNumber=[1,4;...   % 011-010-00-000
            2,5;...   % 011-010-01-000
            3,6;...   % 011-010-01-010
            4,8];     % 011-111-11-111

%% Load simulation OutputsWithPredictiveControl files

for i=1:size(fileNumber,1)   % Engine size - D11, D13 or D16
    for j=1:size(fileNumber,2)   % Number of axles
        path='OutputsWithStepSocLimits/';
        fileNameNumber = int2str(fileNumber(i,j));
        fileName='/C';
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        C(i,j) = load(fullPath);
    end
end

for i=1:size(fileNumber,1)   % Engine size - D11, D13 or D16
    for j=1:size(fileNumber,2)   % Number of axles
        path='OutputsWithStepSocLimits/';
        fileNameNumber = int2str(fileNumber(i,j));
        fileName='/U0B';
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        B(i,j) = load(fullPath);
    end
end

for i=1:size(fileNumber,1)
    for j=1:size(fileNumber,2) 
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

figure('name', 'Mission time vs charge control strategy (D13, GCW=70t)');
plot(missionTime'/3600)
legend('011-010-00-000', '011-010-01-000', '011-010-01-010','011-111-11-111','Location','Best');
title('Mission time vs charge control strategy (D13, GCW=70t)');
xlabel('Charge Control Strategy'), ylabel('Mission time (hours)');
set(gca,'XTick',[1 2]);
set(gca,'XTickLabel',{'Step-assigned charge control','Full predictive control'});
saveas(gcf,'PlotsWithStepControl/MissionTime.pdf');

%% Plot fuel consumed

figure('name', 'Fuel consumption vs charge control strategy (D13, GCW=70t)');
plot(fuelConsumed')
legend('011-010-00-000', '011-010-01-000','011-010-01-001','011-111-11-111','Location','Best');
title('Fuel consumption vs charge control strategy (D13, GCW=70t)');
xlabel('Charge Control Strategy'), ylabel('Fuel consumption (litres)');
set(gca,'XTick',[1 2]);
set(gca,'XTickLabel',{'Step-assigned charge control','Full predictive control'});
saveas(gcf,'PlotsWithStepControl/FuelConsumption.pdf');

%% Plot mission speed

for i=1:size(fileNumber,1)
    figureName=strcat('MissionSpeedVsPosition (70t,D13) Configuration ',int2str(i));
    figure('name', figureName);
    for j=1:size(fileNumber,2)
        plot(C(i,j).positionOverMission, C(i,j).speedOverMission);
        hold all
    end
    title(figureName);
    legend('Step-assigned charge control','Full predictive control','Location','Southwest');
    xlabel('Position over mission (m)');
    ylabel('Speed over mission (m/s)');
    saveas(gcf,strcat('PlotsWithStepControl/',figureName,'.pdf'));
end

%% Plot charge consumption over mission

for i=1:size(fileNumber,1)
    figureName=strcat('Charge Consumption Vs Position (70t,D13) Configuration  ',int2str(i));
    figure('name', figureName);
    for j=1:size(fileNumber,2)
        plot(C(i,j).positionOverMission, C(i,j).chargeConsumptionOverMission*2.4/3.6e6);
        hold all
    end
    title(figureName);
    legend('Step-assigned charge control','Full predictive control','Location','Southwest');
    xlabel('Position over mission (m)');
    ylabel('Electrical energy consumed over mission (kWh)');
    saveas(gcf,strcat('PlotsWithStepControl/',figureName,'.pdf'));
end