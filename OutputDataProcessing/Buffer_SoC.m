% close all
clear all
clc

% Effect of buffer SoC limit on mission performance

% Constant combination - 011-010-01-010, D13, GCW 70t
% Buffer SoC limits affects fuel consumption and charge consumption

fileNumber = [175,176,177];
bufferSize=['SoC limit 1', 'SoC limit 2', 'SoC limit 3'];

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
        fileName='/U0B';
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        B(i,j) = load(fullPath);
    end
end

for i=1:size(fileNumber,1)
    for j=1:size(fileNumber,2)  % Buffer size
        path='Output/';
        fileNameNumber = int2str(fileNumber(i,j));
        fileName='/U1B';
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        B1(i,j) = load(fullPath);
    end
end

for i=1:size(fileNumber,1)
    for j=1:size(fileNumber,2)  % Buffer size
        path='Output/';
        fileNameNumber = int2str(fileNumber(i,j));
        fileName='/U2B';
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        B2(i,j) = load(fullPath);
    end
end

for i=1:size(fileNumber,1)
    for j=1:size(fileNumber,2)  % Buffer size
        path='Output/';
        fileNameNumber = int2str(fileNumber(i,j));
        fileName='/U3B';
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        B3(i,j) = load(fullPath);
    end
end

for i=1:size(fileNumber,1)
    for j=1:size(fileNumber,2)  % Buffer size
        missionTime(i,j) = size(C(i,j).positionOverMission,2);
        fuelConsumed(i,j) = 600-B(i,j).bufferLevelOverMission(end);
        chargeOverMission = C(i,j).chargeConsumptionOverMission;
        chargeConsumed(i,j) = sum(chargeOverMission);
        combinationStartability(i,j) = C(i,j).combinationStartability;
        powerLimitedMode(i,j) = C(i,j).powerLimitCount;
        tractionLimitedMode(i,j) = C(i,j).tractionLimitCount;
    end
end

%% Plot mission time

% figure('name', 'Mission time vs buffer SoC Limit (D13, GCW=70t)');
% plot(missionTime/3600, '-*')
% title('Mission time vs buffer SoC limit (D13, GCW=70t)');
% xlabel('Buffer size'), ylabel('Mission time (hours)');
% set(gca,'XTick',[1 2 3]);
% set(gca,'XTickLabel',{'SoC limit 0.5', 'SoC limit 0.3', 'SoC limit 0.1'});
% saveas(gcf, 'Plots/Effect_of_Buffer_SoC_limit/Mission_time_vs_buffer_SoC.pdf');

%% Plot fuel consumed

% figure('name', 'Fuel consumption vs buffer SoC Limit (D13, GCW=70t)');
% plot(fuelConsumed, '-*')
% title('Fuel consumption vs buffer SoC limit (D13, GCW=70t)');
% xlabel('Buffer size'), ylabel('Fuel consumption (litres)');
% set(gca,'XTick',[1 2 3]);
% set(gca,'XTickLabel',{'SoC limit 0.5', 'SoC limit 0.3', 'SoC limit 0.1'});
% saveas(gcf,'Plots/Effect_of_Buffer_SoC_limit/Fuel_consumption_vs_buffer_size_(with_electric_propulsion).pdf');

%% Plot charge consumed

% figure('name', 'Charge consumption vs buffer SoC Limit (D13, GCW=70t)');
% plot(chargeConsumed, '-*')
% title('Charge consumption vs buffer SoC Limit (D13, GCW=70t)');
% xlabel('Buffer size'), ylabel('Charge consumption (Coulombs)');
% set(gca,'XTick',[1 2 3]);
% set(gca,'XTickLabel',{'SoC limit 0.5', 'SoC limit 0.3', 'SoC limit 0.1'});
% saveas(gcf,'Plots/Effect_of_Buffer_SoC_limit/Fuel consumption_vs_buffer_SoC_Limit(with_electric_propulsion).pdf');

%% Plot startability

% figure('name', 'Combination Startability vs buffer SoC Limit (D13, GCW=70t)');
% plot(combinationStartability, '-*')
% title('Combination Startability vs buffer SoC Limit (D13, GCW=70t)');
% xlabel('Buffer size'), ylabel('Combination Startability (degrees)');
% set(gca,'XTick',[1 2 3]);
% set(gca,'XTickLabel',{'SoC limit 0.5', 'SoC limit 0.3', 'SoC limit 0.1'});
% saveas(gcf,'Plots/Effect_of_Buffer_SoC_limit/Combination_startability_vs_buffer_SoC_Limit(with_electric_propulsion).pdf');

%% Plot mission speed

for i=1:size(fileNumber,1)
    figureName='Mission speed vs position (011-010-01-010, D13, GCW 70t)';
    figure('name', figureName);
    for j=1:size(fileNumber,2)
        plot(C(i,j).positionOverMission, C(i,j).speedOverMission);
        hold all
    end
    title(figureName);
    legend('SoC limit 0.5', 'SoC limit 0.3', 'SoC limit 0.1','Location','Best');
    xlabel('Position over mission (m)');
    ylabel('Speed over mission (m/s)');
    %     saveas(gcf,strcat('Plots/Effect_of_Buffer_SoC_limit/',figureName,'.pdf'));
end

%% Plot charge consumption over mission
for i=1:size(fileNumber,1)
    figureName='Charge consumption vs position (011-010-01-010, D13, GCW 70t)';
    figure('name', figureName);
    for j=1:size(fileNumber,2)
        C(i,j).positionOverMission(1)=[];
        plot(C(i,j).positionOverMission, C(i,j).chargeConsumptionOverMission);
        hold all
    end
    title(figureName);
    legend('SoC limit 0.5', 'SoC limit 0.3', 'SoC limit 0.1','Location','Best');
    xlabel('Position over mission (m)');
    ylabel('Charge consumption over mission (m/s)');
    %     saveas(gcf,strcat('Plots/Effect_of_Buffer_SoC_limit/',figureName,'.pdf'));
end

%% Plot achieved tractive force over mission
for i=1:size(fileNumber,1)
    figureName='Achieved tractive force vs position (011-010-01-010, D13, GCW 70t)';
    figure('name', figureName);
    for j=1:size(fileNumber,2)
        plot(C(i,j).positionOverMission, C(i,j).achievedTractiveForceOverMission);
        hold all
    end
    title(figureName);
    legend('SoC limit 0.5', 'SoC limit 0.3', 'SoC limit 0.1','Location','Best');
    xlabel('Position over mission (m)');
    ylabel('Achieved Tractive Force over mission (N)');
    %     saveas(gcf,strcat('Plots/Effect_of_Buffer_SoC_limit/',figureName,'.pdf'));
end

%% Plot operating mode over mission
for i=1:size(fileNumber,1)
    figureName='Achieved tractive force vs position (011-010-01-010, D13, GCW 70t)';
    figure('name', figureName);
    for j=1:size(fileNumber,2)
        plot(C(i,j).positionOverMission, C(i,j).operatingModeOverMission);
        hold all
    end
    title(figureName);
    legend('SoC limit 0.5', 'SoC limit 0.3', 'SoC limit 0.1','Location','Best');
    xlabel('Position over mission (m)');
    ylabel('operating mode over mission');
    saveas(gcf,strcat('Plots/Effect_of_Buffer_SoC_limit/',figureName,'.pdf'));
    axis([0 3e5 -8 8]);
end

%% Plot SoC over mission
for i=1:size(fileNumber,1)
    figureName='Achieved Buffer 1 SoC vs position (011-010-01-010, D13, GCW 70t)';
    figure('name', figureName);
    for j=1:size(fileNumber,2)
        plot(B1(i,j).stateOfBufferOverMission);
        hold all
    end
    title(figureName);
    legend('SoC limit 0.5', 'SoC limit 0.3', 'SoC limit 0.1','Location','Best');
    xlabel('Position over mission (m)');
    ylabel('Buffer 1 SoC over mission');
    %     saveas(gcf,strcat('Plots/Effect_of_Buffer_SoC_limit/',figureName,'.pdf'));
    
    figureName='Achieved Buffer 2 SoC vs position (011-010-01-010, D13, GCW 70t)';
    figure('name', figureName);
    for j=1:size(fileNumber,2)
        plot(B2(i,j).stateOfBufferOverMission);
        hold all
    end
    title(figureName);
    legend('SoC limit 0.5', 'SoC limit 0.3', 'SoC limit 0.1','Location','Best');
    xlabel('Position over mission (m)');
    ylabel('Buffer 2 SoC over mission');
    
    figureName='Achieved Buffer 3 SoC vs position (011-010-01-010, D13, GCW 70t)';
    figure('name', figureName);
    for j=1:size(fileNumber,2)
        plot(B3(i,j).stateOfBufferOverMission);
        hold all
    end
    title(figureName);
    legend('SoC limit 0.5', 'SoC limit 0.3', 'SoC limit 0.1','Location','Best');
    xlabel('Position over mission (m)');
    ylabel('Buffer 3 SoC over mission');
end

%% Clear unnecessary variables from workspace
clearvars i j path fullPath fileNameNumber fileName extension;
