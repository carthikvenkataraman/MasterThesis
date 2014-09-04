clear all
close all
clc

% Buffer Data in order [maximumBufferLevel, minimumBufferLevel, maximumStateOfBuffer, minimumAllowedStateOfBuffer]
maximumBufferLevel = 90;   % MegaCoulombs
minimumBufferLevel = 0;
maximumStateOfBuffer = 0.7;
minimumAllowedStateOfBuffer = 0.3;  % Minimum allowed SoC / fuel level can be adjusted
openCircuitVoltage = 2.81;  % Volts

bufferData=[maximumBufferLevel, minimumBufferLevel, maximumStateOfBuffer, minimumAllowedStateOfBuffer, openCircuitVoltage];

save('Battery02.mat','bufferData'); 
clear bufferData;