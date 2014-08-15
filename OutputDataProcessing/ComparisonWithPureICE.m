clear all
close all
clc

filePath='/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsWithPredictiveControl/';
fileName=strcat(filePath,int2str(7),'/C.mat');
load(fileName);
missionProd(1)=missionProductivity;
time(1)=size(positionOverMission,2);
fuel(1)=fuelConsumptionOverMission(end);
charge(1)=sum(chargeConsumptionOverMission);

for i=58:69
    fileName=strcat(filePath,int2str(i),'/C.mat');
    load(fileName);
    missionProd(i-56)=missionProductivity;
    time(i-56)=size(positionOverMission,2);
    fuel(i-56)=600-fuelConsumptionOverMission(end);
    charge(i-56)=sum(chargeConsumptionOverMission);
end

figure('name','Mission Productivity Comparison')
plot(missionProd);
set(gca,'XTickLabel',{'Pure ICE','1 axle, Buffer 1', '1 axle, Buffer 2', '1 axle, Buffer 3'...
, '2 axles, Buffer 1', '2 axles, Buffer 2','2 axles, Buffer 3',...
'3 axles, Buffer 1', '3 axles, Buffer 2','3 axles, Buffer 3'})
title('Mission Productivity - Pure ICE vs varying combinations & buffer sizes');
xlabel('Combinations & buffer used'), ylabel('Mission Productivity')
saveas(gcf,'/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/PlotsWithPredictiveControl/BufferSize/MissionProductivityComparedWithPureICE.pdf')

figure('name','Fuel Consumption Comparison')
plot(fuel);
set(gca,'XTickLabel',{'Pure ICE','1 axle, Buffer 1', '1 axle, Buffer 2', '1 axle, Buffer 3'...
, '2 axles, Buffer 1', '2 axles, Buffer 2','2 axles, Buffer 3',...
'3 axles, Buffer 1', '3 axles, Buffer 2','3 axles, Buffer 3'})
title('Fuel Consumption - Pure ICE vs varying combinations & buffer sizes');
xlabel('Combinations & buffer used'), ylabel('Fuel Consumption (l)')
saveas(gcf,'/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/PlotsWithPredictiveControl/BufferSize/FuelConsumptionComparedWithPureICE.pdf')

figure('name','Charge Consumption Comparison')
plot(charge*2.14/3.6e6);
set(gca,'XTickLabel',{'Pure ICE','1 axle, Buffer 1', '1 axle, Buffer 2', '1 axle, Buffer 3'...
, '2 axles, Buffer 1', '2 axles, Buffer 2','2 axles, Buffer 3',...
'3 axles, Buffer 1', '3 axles, Buffer 2','3 axles, Buffer 3'})
title('Charge Consumption - Pure ICE vs varying combinations & buffer sizes');
xlabel('Combinations & buffer used'), ylabel('Charge Consumption (kWh)')
saveas(gcf,'/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/PlotsWithPredictiveControl/BufferSize/ChargeConsumptionComparedWithPureICE.pdf')
