clear all
close all
clc

% Engine BSFC vs RPM
rawData=d_em_pmsm_remy_173kW_411Nm;

rpmRange=rawData.motorMapSpeed_x;
torqueRange=rawData.motorMapTorque_y;
motorEfficiency=rawData.motorMapEfficiency_z';
torqueRange(22)=[];  % 22 for 174kW, 411Nm
motorEfficiency(22,:)=[];  % 22 for 174kW, 411Nm 

save('Motor01.mat', 'rpmRange', '-append');
save('Motor01.mat', 'torqueRange', '-append');
save('Motor01.mat', 'motorEfficiency', '-append');
clear rawData;

% Maximum brake torque vs RPM
rawData=d_mcu_pmsm_remy_173kW_411Nm;

rpmRangeForMaximumBrakeTorque=rawData.motorSpeed_x;
rpmRangeForMaximumBrakeTorque(1)=[];
save('Motor01.mat', 'rpmRangeForMaximumBrakeTorque', '-append');
machineMaximumBrakeTorque=rawData.maximalMotorTorque_y;
machineMaximumBrakeTorque(1)=[];
save('Motor01.mat', 'machineMaximumBrakeTorque', '-append');
machineMaximumRegenerationTorque=-rawData.maximalRegenTorque_y;
machineMaximumRegenerationTorque(1)=[];
save('Motor01.mat', 'machineMaximumRegenerationTorque', '-append');
clear rawData;
plot(rpmRangeForMaximumBrakeTorque,machineMaximumBrakeTorque);
hold on;
plot(rpmRangeForMaximumBrakeTorque,machineMaximumRegenerationTorque);
% 
% save('Motor01.mat');%,'rpmRange', 'torqueRange', 'motorEfficiency', 'rpmRangeForMaximumBrakeTorque', 'machineMaximumBrakeTorque','machineMaximumRegenerationTorque'); 
% Motor00.mat - 120kW, 230Nm, Motor01.mat - 174kW, 411Nm, Engine02.mat - D16