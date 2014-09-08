close all
clear all
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

%% Plots

plot(fuelPrice);
hold all; 
plot(elecPrice);
legend('Fuel Prices', 'Electricity (Grid) Prices');
xlabel('Year'), ylabel('Fuel and grid electricity price trends (EUR)');
set(gca,'XTick',[1 2 3 4]);
set(gca,'XTickLabel',{'2015','2020','2025', '2030'});
saveas(gcf, 'PlotsProductivity/FuelElectricityTrends.pdf');

figure;
plot(batteryPricePerkWh);
hold all;
plot(batteryMass(2,:));
legend('Battery Prices Per kWh', 'Battery Mass');
xlabel('Year'), ylabel('Battery mass (kg) and price (EUR) trends');
set(gca,'XTick',[1 2 3 4]);
set(gca,'XTickLabel',{'2015','2020','2025', '2030'});
saveas(gcf, 'PlotsProductivity/BatteryMassPriceTrends.pdf');

figure;
plot(driverPrice);
legend('Driver Hourly Wages');
xlabel('Year'), ylabel('Driver Hourly Wages (EUR/hour)');
set(gca,'XTick',[1 2 3 4]);
set(gca,'XTickLabel',{'2015','2020','2025', '2030'});
saveas(gcf, 'PlotsProductivity/DriverWageTrends.pdf');

%% Output parameters

% fuelPrice
% elecPrice
% driverPrice
% batteryPrice(2,:)
% batteryMass(2,:)