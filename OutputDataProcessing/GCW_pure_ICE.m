clear all
close all
clc

% Influence of GCW - pure ICE combinations
gcw=[50,60,70,80];

%% Load simulation output files
for i=0:2   % Engine size - D11, D13 or D16
    for j=1:4   % GCW - 50, 60, 70 or 80 t
        path='Pure ICE/';
        fileNameNumber = int2str(4*i+j);
        fileName='/C';
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        pure_ICE_C(i+1,j) = load(fullPath);
    end
end

for i=0:2
    for j=1:4
        path='Pure ICE/';
        fileNameNumber = int2str(4*i+j);
        fileName='/U0B';
        extension='.mat';
        fullPath=strcat(path, fileNameNumber,fileName, extension);
        pure_ICE_B(i+1,j) = load(fullPath);
    end
end

for i=1:3
    for j=1:4
        missionTime(i,j) = size(pure_ICE_C(i,j).positionOverMission,2);
        fuelConsumed(i,j) = 600-pure_ICE_B(i,j).bufferLevelOverMission(end);
        combinationStartability(i,j) = pure_ICE_C(i,j).combinationStartability;
    end
end

%% Plot mission time

figure('name', 'Mission time vs engine size');
plot(missionTime/3600)
legend('50t', '60t', '70t', '80t','Location','Best');
title('Mission time vs engine size');
xlabel('Engine size'), ylabel('Mission time (hours)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'D11','D13','D16'});
saveas(gcf,'Plots/Mission time vs GCW and engine size.pdf');

%% Plot fuel consumed

figure('name', 'Fuel consumption vs engine size');
plot(fuelConsumed)
legend('50t', '60t', '70t', '80t','Location','Best');
title('Fuel consumption vs engine size');
xlabel('Engine size'), ylabel('Fuel consumption (litres)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'D11','D13','D16'});
saveas(gcf,'Plots/Fuel consumption vs GCW and engine size.pdf');

%% Plot startability

figure('name', 'Combination Startability vs engine size');
plot(combinationStartability)
legend('50t', '60t', '70t', '80t','Location','Best');
title('Combination Startability vs engine size');
xlabel('Engine size'), ylabel('Combination Startability (degrees)');
set(gca,'XTick',[1 2 3]);
set(gca,'XTickLabel',{'D11','D13','D16'});
saveas(gcf,'Plots/Combination startability vs GCW and engine size.pdf');

%% Plot mission speed

for i=1:4
    figureName=strcat('Mission speed vs position (constant GCW = ', int2str(gcw(i)), 't)');
    figure('name', figureName);
    for j=1:3
        plot(pure_ICE_C(j,i).positionOverMission, pure_ICE_C(j,i).speedOverMission);
        hold all
    end
    title(figureName);
    legend('D11', 'D13', 'D16','Location','SouthWest');
    xlabel('Position over mission (m)');
    ylabel('Speed over mission (m)');
    saveas(gcf,strcat('Plots/',figureName,'.pdf'));
end

figureName=strcat('Mission speed vs position'); % = ', int2str(gcw(i)), 't)');
figure('name', figureName);
for i=1:4
    subplot(2,2,i);
    for j=1:3
        plot(pure_ICE_C(j,i).positionOverMission, pure_ICE_C(j,i).speedOverMission);
        hold all
    end
    title(strcat(figureName, ' (GCW = ',int2str(gcw(i)), 't)'));
    legend('D11', 'D13', 'D16','Location','SouthWest');
    xlabel('Position over mission (m)');
    ylabel('Speed over mission (m)');
    saveas(gcf,strcat('Plots/',figureName,'.pdf'));
end

%% Clear unnecessary variables from workspace
clearvars i j path fullPath fileNameNumber fileName extension;
