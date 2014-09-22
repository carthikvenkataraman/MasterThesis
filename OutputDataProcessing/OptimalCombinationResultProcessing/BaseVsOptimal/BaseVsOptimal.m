clear all
close all
clc

folderLocation = 'C:\Karthik\Sweden\Chalmers\MasterThesis-PropOpt\OutputDataProcessing\OptimalProductivityOutputs\';
year = 2015;
gcwCode = 'C';
baseFileLocation = strcat(folderLocation,int2str(year), '\', gcwCode,'0.mat');
optimalFileLocation = strcat(folderLocation,int2str(year), '\', gcwCode,'1.mat');

resultFolderLocation = 'C:\Karthik\Sweden\Chalmers\MasterThesis-PropOpt\OutputDataProcessing\OptimalCombinationResults\';
dummyResultLocation = strcat(resultFolderLocation,int2str(year),'\Dummy',gcwCode,'.pdf');
actualResultLocation = strcat(resultFolderLocation,int2str(year),'\',gcwCode,'.pdf');

Base = load(baseFileLocation);
Optimal = load(optimalFileLocation);

revBase = Base.rN/Base.rN;
revOpt = Optimal.rN/Base.rN;
revOptDummy = 0;
rN = [revBase, revOpt];
rNDummy = [revBase, revOptDummy];
y(1,:)=rN;
yDummy(1,:)=rNDummy;

cFixedConvBase = Base.cFixedConv/Base.cFixedConv;
cFixedConvOpt = Optimal.cFixedConv/Base.cFixedConv;
cFixedConvOptDummy = 0;
cFixedConv = [cFixedConvBase, cFixedConvOpt];
cFixedConvDummy = [cFixedConvBase, cFixedConvOptDummy];
y(2,:)=cFixedConv;
yDummy(2,:)=cFixedConvDummy;

cFixedElecBase = 0;
cFixedElecOpt = sum(Optimal.cFixedElec)/sum(Optimal.cFixedElec);
cFixedElecOptDummy = 0;
cFixedElec = [cFixedElecBase, cFixedElecOpt];
cFixedElecDummy = [cFixedElecBase, cFixedElecOptDummy];
y(3,:)=cFixedElec;
yDummy(3,:)=cFixedElecDummy;

cDriverBase = Base.cDriver/Base.cDriver;
cDriverOpt = Optimal.cDriver/Base.cDriver;
cDriverOptDummy = 0;
cDriver = [cDriverBase, cDriverOpt];
cDriverDummy = [cDriverBase, cDriverOptDummy];
y(4,:)=cDriver;
yDummy(4,:)=cDriverDummy;

cFuelBase = Base.cFuel/Base.cFuel;
cFuelOpt = Optimal.cFuel/Base.cFuel;
cFuelOptDummy = 0;
cFuel = [cFuelBase, cFuelOpt];
cFuelDummy = [cFuelBase, cFuelOptDummy];
y(5,:)=cFuel;
yDummy(5,:)=cFuelDummy;

cElecBase = 0;
cElecOpt = (Optimal.cElec~=0);
cElecOptDummy = 0;
cElec = [cElecBase, cElecOpt];
cElecDummy = [cElecBase, cElecOptDummy];
y(6,:)=cElec;
yDummy(6,:)=cElecDummy;

cMntBase = Base.cMnt/Base.cMnt;
cMntOpt = Optimal.cMnt/Base.cMnt;
cMntOptDummy = 0;
cMnt = [cMntBase, cMntOpt];
cMntDummy = [cMntBase, cMntOptDummy];
y(7,:)=cMnt;
yDummy(7,:)=cMntDummy;

cTollsBase = Base.cTolls/Base.cTolls;
cTollsOpt = Optimal.cTolls/Base.cTolls;
cTollsOptDummy = 0;
cTolls = [cTollsBase, cTollsOpt];
cTollsDummy = [cTollsBase, cTollsOptDummy];
y(8,:)=cTolls;
yDummy(8,:)=cTollsDummy;

cTyresBase = Base.cTyres/Base.cTyres;
cTyresOpt = Optimal.cTyres/Base.cTyres;
cTyresOptDummy = 0;
cTyres = [cTyresBase, cTyresOpt];
cTyresDummy = [cTyresBase, cTyresOptDummy];
y(9,:)=cTyres;
yDummy(9,:)=cTyresDummy;

prodBase = Base.P;
prodOpt = Optimal.P;
prodOptDummy = 0;
prod = [prodBase, prodOpt];
prodDummy = [prodBase, prodOptDummy];
y(10,:)=prod;
yDummy(10,:)=prodDummy;

pieParameters = [Optimal.cFixedConv, Optimal.cFixedElec, Optimal.cDriver, Optimal.cFuel, Optimal.cElec, Optimal.cMnt, Optimal.cTyres, Optimal.cTolls];

figure('name', 'Dummy');
hD = bar(yDummy);
xticklabel_rotate([1:10],45,{'Mission Revenues', 'Fixed Cost (Conv)', 'Fixed Cost (Elec)',...
        'Driver Costs', 'Fuel Costs', 'Electricity costs',...
        'Maintenance costs', 'Tolls', 'Tyre costs', 'N-year Productivity'},'interpreter','none');
% axis([0 11 0 max(1,prodOpt)+0.3]);
% lDummy=legend('Base', 'Location', 'SouthEastOutside');
% set(lDummy, 'FontSize', 8);
set(gcf, 'Position', get(0,'Screensize')); 
saveas(gcf, dummyResultLocation);

figure('name', 'Actual');
h = bar(y);
xticklabel_rotate([1:10],45,{'Mission Revenues', 'Fixed Cost (Conv)', 'Fixed Cost (Elec)',...
        'Driver Costs', 'Fuel Costs', 'Electricity costs',...
        'Maintenance costs', 'Tolls', 'Tyre costs', 'N-year Productivity'},'interpreter','none');
% axis([0 11 0 max(1,prodOpt)+0.3]);
% lActual=legend('Base', 'Optimal', 'Location', 'SouthEastOutside');
% set(lActual, 'FontSize', 8);
set(gcf, 'Position', get(0,'Screensize')); 
saveas(gcf, actualResultLocation);