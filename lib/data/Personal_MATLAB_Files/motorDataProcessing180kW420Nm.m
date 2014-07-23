clear all
close all
clc

% Engine BSFC vs RPM
rawData=d_em_pmsm_remy_180kW_420Nm;

rpmRange=rawData.motorMapSpeed_x;
save('Motor02.mat', 'rpmRange', '-append');
torqueRange=rawData.motorMapTorque_y;
save('Motor02.mat', 'torqueRange', '-append');
motorEfficiency=rawData.motorMapEfficiency_z';
save('Motor02.mat', 'motorEfficiency', '-append');
clear rawData;

torqueRange(43)=[];  % 43 for 180kW, 420Nm
motorEfficiency(43,:)=[];  % 43 for 180kW, 420Nm 

% Maximum brake torque vs RPM
rawData=d_mcu_pmsm_remy_180kW_420Nm;

rpmRangeForMaximumBrakeTorque=rawData.motorSpeed_x;
rpmRangeForMaximumBrakeTorque(1)=[];
save('Motor02.mat', 'rpmRangeForMaximumBrakeTorque', '-append');
machineMaximumBrakeTorque=rawData.maximalMotorTorque_y;
machineMaximumBrakeTorque(1)=[];
save('Motor02.mat', 'machineMaximumBrakeTorque', '-append');
machineMaximumRegenerationTorque=-rawData.maximalRegenTorque_y;
machineMaximumRegenerationTorque(1)=[];
save('Motor02.mat', 'machineMaximumRegenerationTorque', '-append');
clear rawData;

% plot(rpmRangeForMaximumBrakeTorque,machineMaximumBrakeTorque);
% hold on;
% plot(rpmRangeForMaximumBrakeTorque,machineMaximumRegenerationTorque); 
% 
% save('Motor02.mat','rpmRange', 'torqueRange', 'motorEfficiency', 'rpmRangeForMaximumBrakeTorque', 'machineMaximumBrakeTorque','machineMaximumRegenerationTorque'); 
% Motor00.mat - 120kW, 230Nm, Motor02.mat - 173kW, 800Nm, Motor02.mat - 180kW, 420Nm