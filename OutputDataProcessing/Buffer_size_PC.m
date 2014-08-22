close all
clear all
clc

%% Effect of buffer size on mission performance
% D13, GCW 70t, Buffer size affects fuel consumption and charge consumption

combinationType = ['011-010-00-000', '011-010-00-010', '011-010-01-010','011-111-11-111'];
bufferSize=['Battery 1', 'Battery 2', 'Battery 3'];

fileNumber = [58 59 60; % 011-010-00-000
    61 62 63; % 011-010-00-010
    64 65 66; % 011-010-01-010
    67 68 69]';% 011-111-11-111

batteryVoltage = 2.4;   % Volts

%% Load simulation output files
for i=1:size(fileNumber,1)
    for j=1:size(fileNumber,2)   % Buffer size
        path='OutputsWithPredictiveControl/';
        fileNameNumber = int2str(fileNumber(i,j));
        fileName='/C';
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        C(i,j) = load(fullPath);
    end
end

for i=1:size(fileNumber,1)
    for j=1:size(fileNumber,2)  % Buffer size
        path='OutputsWithPredictiveControl/';
        fileNameNumber = int2str(fileNumber(i,j));
        fileName='/U0B';
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        B0(i,j) = load(fullPath);
    end
end

for i=1:size(fileNumber,1)
    for j=1:size(fileNumber,2)  % Buffer size
        missionTime(i,j) = size(C(i,j).positionOverMission,2);
        fuelConsumed(i,j) = 600-B0(i,j).bufferLevelOverMission(end);
        chargeOverMission = C(i,j).chargeConsumptionOverMission;
        chargeConsumed(i,j) = sum(chargeOverMission);
        electricalEnergy(i,j) = chargeConsumed(i,j)*batteryVoltage/(3.6e6); %kWh
        combinationStartability(i,j) = C(i,j).combinationStartability;
    end
end

%% Plot mission time

figure('name', 'Mission time vs buffer size (D13, GCW=70t)');
plot(missionTime/3600, '-*')
title('Mission time vs buffer size (D13, GCW=70t)');
xlabel('Buffer size'), ylabel('Mission time (hours)');
legend('011-010-00-000', '011-010-00-010', '011-010-01-010','011-111-11-111','Location', 'Best');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'Battery 1', 'Battery 2', 'Battery 3'});
saveas(gcf, 'PlotsWithPredictiveControl/BufferSize/Mission time vs buffer size (D13, GCW=70t).pdf');

%% Plot fuel consumed

figure('name', 'Fuel consumption vs buffer size (D13, GCW=70t)');
plot(fuelConsumed, '-*')
title('Fuel consumption vs buffer size (D13, GCW=70t)');
xlabel('Buffer size'), ylabel('Fuel consumption (litres)');
legend('011-010-00-000', '011-010-00-010', '011-010-01-010','011-111-11-111','Location', 'Best');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'Battery 1', 'Battery 2', 'Battery 3'});
saveas(gcf,'PlotsWithPredictiveControl/BufferSize/Fuel consumption vs buffer size (with electric propulsion).pdf');

%% Plot charge consumed

figure('name', 'Charge consumption vs buffer size (D13, GCW=70t)');
plot(chargeConsumed, '-*')
title('Charge consumption vs buffer size (D13, GCW=70t)');
xlabel('Buffer size'), ylabel('Charge consumption (Coulombs)');
legend('011-010-00-000', '011-010-00-010', '011-010-01-010','011-111-11-111','Location', 'Best');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'Battery 1', 'Battery 2', 'Battery 3'});
saveas(gcf,'PlotsWithPredictiveControl/BufferSize/Fuel consumption vs buffer size (with electric propulsion).pdf');

%% Plot charge consumed

figure('name', 'Electrical energy consumed vs buffer size (D13, GCW=70t)');
plot(electricalEnergy, '-*');
title('Electrical energy consumed (in kWh) vs buffer size (D13, GCW=70t)');
xlabel('Buffer size'), ylabel('Electrical energy consumed (kWh)');
legend('011-010-00-000', '011-010-00-010', '011-010-01-010','011-111-11-111','Location', 'Best');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'Battery 1', 'Battery 2', 'Battery 3'});
saveas(gcf,'PlotsWithPredictiveControl/BufferSize/Electrical energy consumed vs buffer size (with electric propulsion).pdf');

%% Plot startability

figure('name', 'Combination Startability vs buffer size (D13, GCW=70t)');
plot(combinationStartability, '-*')
title('Combination Startability vs buffer size (D13, GCW=70t)');
xlabel('Buffer size'), ylabel('Combination Startability (degrees)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'Battery 1', 'Battery 2', 'Battery 3'});
saveas(gcf,'PlotsWithPredictiveControl/BufferSize/Combination startability vs buffer size (with electric propulsion).pdf');

%% Plot mission speed

fileNumber = [58 59 60; % 011-010-00-000
              61 62 63; % 011-010-00-010
              64 65 66; % 011-010-01-010
              67 68 69];% 011-111-11-111

for i=1:size(fileNumber,1)  % No of combinations
    figureName=strcat('Mission speed vs position (D13, GCW 70t) ',combinationType(i));
    figure('name', figureName);
    for j=1:size(fileNumber,2) % No. of battery sizes
        plot(C(j,i).positionOverMission, C(j,i).speedOverMission);
        hold all
    end
    title(figureName);
    legend('Battery 1', 'Battery 2', 'Battery 3','Location','Best');
    xlabel('Position over mission (m)');
    ylabel('Speed over mission (m/s)');
    saveas(gcf,strcat('PlotsWithPredictiveControl/BufferSize/',figureName,'.pdf'));
end

%% Clear unnecessary variables from workspace
clearvars i j path fullPath fileNameNumber fileName extension;
