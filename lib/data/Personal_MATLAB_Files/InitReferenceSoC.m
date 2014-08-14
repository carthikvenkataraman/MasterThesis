function zeta = InitReferenceSoC(targetSpeed, longitudinalPosition, roadGradientInRadians)

% clear all
close all
clc

%% Vehicle data
A = 9.7;  % m2
Cd = 0.6;
rho = 1.184; % kg/m3
Cr = 0.005;
g = 9.81;       % m/s^2
M = 68000;  % kg

%% Mission route data

% missionRouteData = csvread('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/MissionData.csv');
% missionRouteData(1:10,3)=0;
% missionRouteData(11:end,3)=0.02;
% 
% targetSpeed=missionRouteData(:,1)';              % m/s
% longitudinalPosition=missionRouteData(:,2)';     % m
% roadGradientInRadians=missionRouteData(:,3)';    % rad
% clear missionRouteData;

elevation(1)=0;
for i=2:size(roadGradientInRadians,2)
    elevation(i)=(longitudinalPosition(i)-longitudinalPosition(i-1))*tan(roadGradientInRadians(i-1))+elevation(i-1);    % (m)
end

%% Buffer data

numberOfBuffers=3;
buffer(1) = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/Battery00.mat');
buffer(2) = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/Battery01.mat');
buffer(3) = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/lib/data/Battery02.mat');
bufferCapacity = [buffer(1).bufferData(1) buffer(2).bufferData(1) buffer(3).bufferData(1)];
totalBufferCapacity = sum(bufferCapacity);
OCV = 2.81;  % V
Pmax=500000;    %W
etaRegen = 0.9;

%% To make minimum zeta linearly decreasing
% zeta_min,k definition
% Minimum allowed SoC level linearly decreases over the mission to simulate
% a plug-in hybrid. At the beginning of the mission, the minimum allowed
% SoC is 0.3 and at the end, the minimum allowed SoC is 0.15 in this case.

zeta_min_max = 0.3;
zeta_min_min = 0.15;
zeta_max_max = 0.7;
x_max = longitudinalPosition(end);
x_min = longitudinalPosition(1);
zeta_min=zeros(1,size(longitudinalPosition,2));

for i=1:size(longitudinalPosition,2)
    zeta_min(i)=zeta_min_max-((zeta_min_max-zeta_min_min)*(longitudinalPosition(i)-x_min)/(x_max-x_min));
end

% clearvars x_max x_min zeta_min zeta_max zeta_max_max zeta_min_max zeta_min_min

%% Find crests and troughs of mission

[~, peakIndices] = findpeaks(elevation);%, 'MINPEAKDISTANCE', 10);
invElevation = 1.01*max(elevation)-elevation;
[~, troughIndices]=findpeaks(invElevation);
troughIndices = [1 troughIndices size(elevation,2)];
clear invElevation;

if(size(peakIndices)~=0)
    peakIndicesToRemove=[];
    troughIndicesToRemove=[];
    for i=1:size(troughIndices,2)-2
        heightDescent = elevation(peakIndices(i))-elevation(troughIndices(i+1));
        if(heightDescent < 10)
            peakIndicesToRemove=[peakIndicesToRemove peakIndices(i)];
            troughIndicesToRemove=[troughIndicesToRemove troughIndices(i+1)];
        end
    end
    
    plot(elevation);
    hold on, plot(peakIndices, elevation(peakIndices), 'r*',troughIndices, elevation(troughIndices), 'gO');
    hold on, plot(peakIndicesToRemove, elevation(peakIndicesToRemove), 'k*',troughIndicesToRemove, elevation(troughIndicesToRemove), 'k*');
    
    for i=1:size(peakIndicesToRemove,2)
        peakIndices(peakIndices==peakIndicesToRemove(i))=[];
    end
    for i=1:size(troughIndicesToRemove,2)
        troughIndices(troughIndices==troughIndicesToRemove(i))=[];
    end
    
    if(troughIndices(end)~=size(elevation,2))
        troughIndices = [troughIndices size(elevation,2)];
    end
    
    figure;
    plot(elevation);
    % saveas(gcf,strcat('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt-PropOpt/OutputDataProcessing/PlotsWithPredictiveControl/','Elevation profile over the mission','.pdf'));
    
    figure;
    plot(elevation);
    sortedIndices = sort([peakIndices troughIndices]);
    hold on, plot(sortedIndices, elevation(sortedIndices), 'r-*');
    % saveas(gcf,strcat('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt-PropOpt/OutputDataProcessing/PlotsWithPredictiveControl/','Elevation profile over the mission with peaks and troughs','.pdf'));
    
    % figure;
    % plot(longitudinalPosition, elevation);
    % hold on, plot(longitudinalPosition(sortedIndices), elevation(sortedIndices), 'r-*');
    
    %% Calculate zeta_k for all k (minimum allowed SoC at each point along the mission)
    
    % The portion of the road between two consecutive troughs is called one section.
    % In each section, the lowest allowed SoC (at that point, as calculated above) is assigned to the 'crest / peak'.
    % The amount of recuperation energy is calculated using the average speed and average gradient between the 'peak' and the 'trough'.
    % This recuperation energy is converted to an equivalent change in battery SoC, based on TOTAL available charge of all buffers put together.
    % The maximum 'minimum-allowed SoC' at the trough is thus fixed to zeta_top + delta(zeta)
    % Based on this range of zeta, all the points in this section are assigned zeta_k values, which correspond to the lowest allowed SoC at each x.
    
    % The starting reference SoC must be the absolute minimum, zeta_min_min (say 0.15).
    % For following troughs, the minimum SoC must be minimum SoC at the peak + delta_SoC.
    % delta_SoC is the maximum possible change in SoC due to regeneration.
    % Hence the minimum SoC that can occur at the following trough is min SoC @ peak + delta_Soc
    
    % zeta_max(1) is the maximum min Soc at the 1st trough
    % zeta_max(1) = zeta_min_min;
    for i=1:size(troughIndices,2)-1     % Each i marks the beginning of a section
        delta_h = elevation(peakIndices(i))-elevation(troughIndices(i+1));
        meanSpeed = mean(targetSpeed(peakIndices(i):troughIndices(i+1)));
        meanGradient = mean(roadGradientInRadians(peakIndices(i):troughIndices(i+1)));
        delta_d = (longitudinalPosition(troughIndices(i+1))-longitudinalPosition(peakIndices(i)))/sin(meanGradient);
        forceOnVehicle = 0.5*Cd*rho*A*meanSpeed^2+Cr*M*g*cos(meanGradient)+M*g*sin(meanGradient);
        maxForce = -Pmax/(meanSpeed*etaRegen);
        regenForce = max(forceOnVehicle, maxForce);
        if(regenForce<0)
            E_recup = regenForce*etaRegen*delta_d;    % J
        else
            E_recup = 0;
        end
        % zeta_max(k) is the maximum min Soc at the 'k'th trough
        zeta_max(i) = zeta_min(peakIndices(i))+E_recup/(totalBufferCapacity*OCV);
        if(zeta_max(i)>zeta_max_max)
            zeta_max(i) = zeta_max_max;
        end
        
        for j=troughIndices(i):troughIndices(i+1)
            if(j<peakIndices(i))
                ratio = (elevation(peakIndices(i))-elevation(j))/(elevation(peakIndices(i))-elevation(troughIndices(i)));
            else
                ratio = (elevation(peakIndices(i))-elevation(j))/(elevation(peakIndices(i))-elevation(troughIndices(i+1)));
            end
            
            if(ratio>1)
                ratio=1;
                %             disp('blah');
                %             ratio
                %             i
                %             j
                %             troughIndices(i)
                %             peakIndices(i)
                %             troughIndices(i+1)
                %             elevation(troughIndices(i))
                %             elevation(peakIndices(i))
                %             elevation(troughIndices(i+1))
                %             elevation(j)
                %             keyboard;
            end
            zeta(j)=zeta_min(peakIndices(i))+ratio*(zeta_max(i)-zeta_min(peakIndices(i)));
        end
    end
else
    for i=1:size(elevation,2)
        ratio = (max(elevation)-elevation(i))/(max(elevation)-min(elevation));
        zeta(i) = zeta_min(end)+ratio*(zeta_max_max-zeta_min(end));
    end
end

% clearvars rho delta_h meanSpeed meanGradient delta_d;
% clearvars forceOnVehicle maxForce regenForce E_recup;
% clearvars troughs troughIndices troughIndicesToRemove sortedIndices;

figure('name', 'Reference SoC vs Position Index');
plot(zeta);
title('Minimum allowed SoC at each longitudinal position index');
% saveas(gcf,strcat('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt-PropOpt/OutputDataProcessing/PlotsWithPredictiveControl/','Reference SoC vs Position Index','.pdf'));

clearvars -except zeta 
% close all;
clc