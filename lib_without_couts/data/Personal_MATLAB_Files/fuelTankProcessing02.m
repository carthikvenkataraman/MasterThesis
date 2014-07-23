clear all
close all
clc

% Buffer Data in order [maximumBufferLevel, minimumBufferLevel, maximumStateOfBuffer, minimumAllowedStateOfBuffer]
maximumBufferLevel = 600;   % TNK-DUAL, LFUEL160, RFUEL160
minimumBufferLevel = 0;
maximumStateOfBuffer = 1;
minimumAllowedStateOfBuffer = 0.3;  % Minimum allowed SoC / fuel level can be adjusted
openCircuitVoltage = 0;

bufferData=[maximumBufferLevel, minimumBufferLevel, maximumStateOfBuffer, minimumAllowedStateOfBuffer, openCircuitVoltage];

save('FuelTank02.mat','bufferData'); 
clear bufferData;