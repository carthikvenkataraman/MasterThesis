clear all
close all
clc

% Engine BSFC vs RPM
rawData=d_em_pmsm_kollmorgen_120kW_230Nm;

rpmRange=rawData.motorMapSpeed_x';
save('Motor00.mat', 'rpmRange', '-append');
torqueRange=rawData.motorMapTorque_y;
save('Motor00.mat', 'torqueRange', '-append');
motorEfficiency=rawData.motorMapEfficiency_z';
save('Motor00.mat', 'motorEfficiency', '-append');
clear rawData;

torqueRange(14)=[];  % 14 for 120kW, 230Nm
motorEfficiency(14,:)=[];  % 14 for 120kW, 230Nm 

% Maximum brake torque vs RPM
rawData=d_mcu_pmsm_kollmorgen_120kW_230Nm;

rpmRangeForMaximumBrakeTorque=rawData.motorSpeed_x;
rpmRangeForMaximumBrakeTorque(1)=[];
save('Motor00.mat', 'rpmRangeForMaximumBrakeTorque', '-append');
machineMaximumBrakeTorque=rawData.maximalMotorTorque_y;
machineMaximumBrakeTorque(1)=[];
save('Motor00.mat', 'machineMaximumBrakeTorque', '-append');
machineMaximumRegenerationTorque=-rawData.maximalRegenTorque_y;
machineMaximumRegenerationTorque(1)=[];
save('Motor00.mat', 'machineMaximumRegenerationTorque', '-append');
clear rawData;
% plot(rpmRangeForMaximumBrakeTorque,machineMaximumBrakeTorque);
% hold on;
% plot(rpmRangeForMaximumBrakeTorque,machineMaximumRegenerationTorque);


% save('Motor00.mat','rpmRange', 'torqueRange', 'motorEfficiency', 'rpmRangeForMaximumBrakeTorque', 'machineMaximumBrakeTorque','machineMaximumRegenerationTorque'); 
% Motor00.mat - 120kW, 230Nm, Motor00.mat - D13, Engine02.mat - D16