% close all
% clear all
% clc

%% Currency conversion

USDtoEUR = 0.76;

%% Battery size and mass data (year 2015)

batterySizes = [5,50,90];    % kWh
cellMasses(:,1) = [167, 524, 918];  % kg - Cell mass is 60% of pack mass. Hence must be divided by 0.6

%% Fuel and other parameter price data (year 2015)
fuelPrice(1)= 1.468;            % EUR
elecPrice(1)= 0.21;             % EUR
driverPrice(1) = 24;            % EUR
batteryPricePerkWh(1) = 800;    % EUR [436 275 244 230]*USDtoEUR;
revUnitFreight(1) = 0.06;       % EUR

%% Base combination price

pTractorBase = 130000;
deltaPowertrain = 15000;    % D16-D11
pUnit = [pTractorBase, 70000, 40000, 70000]*USDtoEUR+[deltaPowertrain, 0, 0, 0];
cFixedConv(1) = sum(pUnit);

%% Percentage change in prices

rFuel = 3;  % YoY
rElec = 3;  % YoY
rDriver = 2.73; % YoY
rBatt = 5;  % YoY per kWh
rCellMass = 5; % YoY
rRev = 3;   % YoY
rFixed = 3; % YoY

%% Parameter evolution upto year 2030

for i=2:4   % Different years
    fuelPrice(i) = fuelPrice(i-1)*(1+rFuel/100)^5;
    elecPrice(i) = elecPrice(i-1)*(1+rFuel/100)^5;
    driverPrice(i) = driverPrice(i-1)*(1+rDriver/100)^5;
    batteryPricePerkWh(i) = batteryPricePerkWh(i-1)*(1-rBatt/100)^5;    % per kWh
    cellMasses(:,i)=cellMasses(:,i-1)*(1-rCellMass/100)^5; 
    revUnitFreight(i) = revUnitFreight(i-1)*(1+rRev/100)^5;
    cFixedConv(i) = cFixedConv(i-1)*(1+rFixed/100)^5;
end

for i=1:3
    batteryPrice(i,:) = batterySizes(i)*batteryPricePerkWh;
    batteryMass(i,:) = cellMasses(i,:)/0.6; % kg - Cell mass is 60% of pack mass. Hence must be divided by 0.6
end

%% Plots

% plot(fuelPrice);
% hold all; 
% plot(elecPrice);
% legend('Fuel Prices', 'Electricity (Grid) Prices');
% xlabel('Year'), ylabel('Fuel and grid electricity price trends (EUR)');
% set(gca,'XTick',[1 2 3 4]);
% set(gca,'XTickLabel',{'2015','2020','2025', '2030'});
% % saveas(gcf, 'PlotsProductivity/FuelElectricityTrends.pdf');
% 
% figure;
% plot(batteryPricePerkWh);
% hold all;
% plot(batteryMass(2,:));
% legend('Battery Prices Per kWh', 'Battery Mass');
% xlabel('Year'), ylabel('Battery mass (kg) and price (EUR) trends');
% set(gca,'XTick',[1 2 3 4]);
% set(gca,'XTickLabel',{'2015','2020','2025', '2030'});
% % saveas(gcf, 'PlotsProductivity/BatteryMassPriceTrends.pdf');
% 
% figure;
% plot(driverPrice);
% legend('Driver Hourly Wages');
% xlabel('Year'), ylabel('Driver Hourly Wages (EUR/hour)');
% set(gca,'XTick',[1 2 3 4]);
% set(gca,'XTickLabel',{'2015','2020','2025', '2030'});
% saveas(gcf, 'PlotsProductivity/DriverWageTrends.pdf');

%% Output parameters

% fuelPrice
% elecPrice
% driverPrice
% batteryPrice(2,:)
%batteryMass(2,:)