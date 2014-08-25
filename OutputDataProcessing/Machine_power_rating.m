close all
clear all
clc

% Effect of machine power rating on mission performance

% Constant combination - 011-010-01-010, D13, GCW 70t
% Power rating of machines changed in each round of simulations

fileNumber = [169,170,171];
machinePowerRating=['120kW, 230Nm', '173kW, 411Nm', '180kW, 420Nm'];
openCircuitVoltage = 2.4;   % Volts

%% Load simulation output files
for i=1:size(fileNumber,1)
    for j=1:size(fileNumber,2)   % Machine power rating
        path='Output/';
        fileNameNumber = int2str(fileNumber(i,j));
        fileName='/C';
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        C(i,j) = load(fullPath);
    end
end

for i=1:size(fileNumber,1)
    for j=1:size(fileNumber,2)   % Machine power rating
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

figure('name', 'Mission time vs machine power rating (GCW=70t)');
plot(missionTime/3600, '-*')
title('Mission time vs machine power rating (GCW=70t)');
xlabel('Machine rating'), ylabel('Mission time (hours)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'120kW, 230Nm', '173kW, 411Nm', '180kW, 420Nm'});
% saveas(gcf, 'Plots/Effect of machine power rating/Mission time vs machine power rating.pdf');

%% Plot fuel consumed

figure('name', 'Fuel consumption vs machine power rating (GCW=70t)');
plot(fuelConsumed, '-*')
title('Fuel consumption vs machine power rating (GCW=70t)');
xlabel('Machine rating'), ylabel('Fuel consumption (litres)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'120kW, 230Nm', '173kW, 411Nm', '180kW, 420Nm'});
% saveas(gcf,'Plots/Effect of machine power rating/Fuel consumption vs machine power rating.pdf');

%% Plot electrical energy consumed

figure('name', 'Electrical energy consumption vs machine power rating (GCW=70t)');
plot(electricalEnergyConsumed, '-*')
title('Electrical energy consumption vs machine power rating (GCW=70t)');
xlabel('Machine rating'), ylabel('Electrical energy consumption (kWh)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'120kW, 230Nm', '173kW, 411Nm', '180kW, 420Nm'});
saveas(gcf,'Plots/Effect of machine power rating/Electrical_energy_consumption_vs_machine_power_rating.pdf');

%% Plot startability

figure('name', 'Combination Startability vs machine power rating (GCW=70t)');
plot(combinationStartability, '-*')
title('Combination Startability vs machine power rating (GCW=70t)');
xlabel('Machine rating'), ylabel('Combination Startability (degrees)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'120kW, 230Nm', '173kW, 411Nm', '180kW, 420Nm'});
% saveas(gcf,'Plots/Effect of machine power rating/Combination startability vs machine power rating.pdf');

%% Plot mission speed

for i=1:size(fileNumber,1)
    figureName='Mission speed vs position (011-010-01-010, D13, GCW 70t)';
    figure('name', figureName);
    for j=1:size(fileNumber,2)
        plot(C(i,j).positionOverMission, C(i,j).speedOverMission);
        hold all
    end
    title(figureName);
    legend('120kW, 230Nm', '173kW, 411Nm', '180kW, 420Nm','Location','Best');
    xlabel('Position over mission (m)');
    ylabel('Speed over mission (m/s)');
    % saveas(gcf,strcat('Plots/Effect of machine power rating/',figureName,'.pdf'));
end

%% Clear unnecessary variables from workspace
clearvars i j path fullPath fileNameNumber fileName extension;
