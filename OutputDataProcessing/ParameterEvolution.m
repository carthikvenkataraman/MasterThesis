% close all
% clear all
% clc
%% Currency conversion

USDtoEUR = 0.76;

%% Battery data

batterySizes = [5,50,90];    % kWh
batteryMasses = [167, 524, 918];  % kg

%% Fuel and other parameters in year 2015
fuelPrice(1)= 1.468;
elecPrice(1)= 0.21;
driverPrice(1) = 24;
batteryPricePerkWh = [436 275 244 230]*USDtoEUR;
for i=1:3
    batteryPrice(i,:) = batterySizes(i)*batteryPricePerkWh;
end
batteryMass(:,1) = batteryMasses(:);  

%% Percentage change in prices

rFuel = 3;  % YoY
rElec = 3;  % YoY
rDriver = 2.73; % YoY

rBattery = [22.92, 9.23, 0.56]; % Every 5 years

%% Parameter evolution upto year 2030

for i=2:4   % Different years
    fuelPrice(i) = fuelPrice(i-1)*(1+rFuel/100)^5;
    elecPrice(i) = elecPrice(i-1)*(1+rFuel/100)^5;
    driverPrice(i) = driverPrice(i-1)*(1+rDriver/100)^5;
    
end

for i=1:3
    for j=2:4
        batteryMass(i,j) = batteryMass(i,j-1)*(1-rBattery(j-1)/100);
    end
end

%% Output parameters

% fuelPrice
% elecPrice
% driverPrice
% batteryPrice(2,:)
% batteryMass(2,:)