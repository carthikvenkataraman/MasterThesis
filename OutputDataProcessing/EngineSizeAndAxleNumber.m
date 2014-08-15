clear all
close all
clc

% Influence of axle number and engine size
fileNumber=[3,15,51,87,178;...   % D11 engine
            7,19,55,91,179;...   % D13 engine
            11,23,59,95,180];    % D16 engine
engineSize=[11,13,16];

%% Load simulation Output files

for i=1:size(fileNumber,1)   % Engine size - D11, D13 or D16
    for j=1:size(fileNumber,2)   % Number of axles
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

figure('name', 'Mission time vs engine size (GCW=70t)');
plot(missionTime/3600)
legend('011-000-00-000', '011-010-00-000', '011-010-01-000', '011-010-01-010','011-111-11-111','Location','Best');
title('Mission time vs engine size (GCW=70t)');
xlabel('Engine size'), ylabel('Mission time (hours)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'D11','D13','D16'});
saveas(gcf,'Plots/EngineSizeAndNoOFAxles/MissionTimeVsAxleNumberAndEngineSize.pdf');

%% Plot fuel consumed

figure('name', 'Fuel consumption vs engine size (GCW=70t)');
plot(fuelConsumed)
legend('011-000-00-000', '011-010-00-000', '011-010-01-000','011-010-01-001','011-111-11-111','Location','Best');
title('Fuel consumption vs engine size (GCW=70t)');
xlabel('Engine size'), ylabel('Fuel consumption (litres)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'D11','D13','D16'});
saveas(gcf,'Plots/EngineSizeAndNoOFAxles/FuelConsumptionVsAxleNumberAndEngineSize.pdf');

%% Percentage reductions in fuel consumption

for i=1:size(fileNumber,1)   % Engine size - D11, D13 or D16
    reference = fuelConsumed(i,1)
    for j=2:size(fileNumber,2)   % Number of axles    
        fuelConsumed(i,j)
        reduction(i,j-1)= -(fuelConsumed(i,j)-reference)*100/reference
    end
end

figure('name', 'Fuel consumption reduction vs engine size (GCW=70t)');
plot(reduction')
legend('D11','D13','D16','Location','NorthEast');
title('Fuel consumption reduction (in %) vs engine size (GCW=70t)');
xlabel('Vehicle Combination'), ylabel('Reduction in fuel consumption compared to pure ICE combination (%)');
set(gca,'XTick',[1 2 3 4]);
set(gca,'XTickLabel',{'011-010-00-000', '011-010-01-000','011-010-01-001','011-111-11-111'});
saveas(gcf,'Plots/EngineSizeAndNoOFAxles/FuelConsumptionReductionVsAxleNumberAndEngineSize.pdf');

%% Plot startability

figure('name', 'Combination Startability vs engine size (GCW=70t)');
plot(combinationStartability)
legend('011-000-00-000', '011-010-00-000', '011-010-01-000','011-010-01-001','011-111-11-111','Location','NorthEast');
title('Combination Startability vs engine size (GCW=70t)');
xlabel('Engine size'), ylabel('Combination Startability (degrees)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'D11','D13','D16'});
saveas(gcf,'Plots/EngineSizeAndNoOFAxles/CombinationStartabilityVsAxleNumberAndEngineSize.pdf');

%% Plot mission speed

for i=1:size(fileNumber,1)
    figureName=strcat('MissionSpeedVsPosition(constantGCW=70t,engine-D',int2str(engineSize(i)),')');
    figure('name', figureName);
    for j=1:size(fileNumber,2)
        plot(C(i,j).positionOverMission, C(i,j).speedOverMission);
        hold all
    end
    title(figureName);
    legend('011-000-00-000', '011-010-00-000', '011-010-01-000','011-010-01-001','011-111-11-111','Location','Southwest');
    xlabel('Position over mission (m)');
    ylabel('Speed over mission (m/s)');
    saveas(gcf,strcat('Plots/EngineSizeAndNoOFAxles/',figureName,'.pdf'));
end

figureName=strcat('MissionSpeedVsPosition(constantGCW=70t)'); % = ', int2str(gcw(i)), 't)');
figure('name', figureName);
for i=1:size(fileNumber,1)
    subplot(size(fileNumber,1),1,i);
    for j=1:size(fileNumber,2)
        plot(C(i,j).positionOverMission, C(i,j).speedOverMission);
        hold all
    end
    title(strcat(figureName, ' engine - D',int2str(engineSize(i)), ')'));
    legend('011-000-00-000', '011-010-00-000', '011-010-01-000','011-010-01-001','011-111-11-111','Location','Southwest');
    xlabel('Position over mission (m)');
    ylabel('Speed over mission (m/s)');
    saveas(gcf,strcat('Plots/EngineSizeAndNoOFAxles/',figureName,'.pdf'));
end

% Clear unnecessary variables from workspace
clearvars i j path fullPath fileNameNumber fileName extension;