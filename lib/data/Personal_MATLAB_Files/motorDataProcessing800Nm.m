clear all
close all
clc

% Engine BSFC vs RPM
rawData=d_em_pmsm_kollmorgen_120kW_800Nm;

rpmRange=rawData.motorMapSpeed_x;
save('Motor02.mat', 'rpmRange', '-append');
torqueRange=rawData.motorMapTorque_y;
save('Motor02.mat', 'torqueRange', '-append');
motorEfficiency=rawData.motorMapEfficiency_z';
save('Motor02.mat', 'motorEfficiency', '-append');
clear rawData;

torqueRange(9)=[];  % 9 for 120kW, 800Nm
motorEfficiency(9,:)=[];  % 9 for 120kW, 800Nm

% Maximum brake torque vs RPM
rawData=d_mcu_pmsm_kollmorgen_120kW_800Nm;

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


% save('Motor02.mat','rpmRange', 'torqueRange', 'motorEfficiency', 'rpmRangeForMaximumBrakeTorque', 'machineMaximumBrakeTorque','machineMaximumRegenerationTorque'); 
% Motor00.mat - 120kW, 230Nm, Motor01.mat - 174kW, 411Nm, Motor02.mat - 120kW, 800Nm