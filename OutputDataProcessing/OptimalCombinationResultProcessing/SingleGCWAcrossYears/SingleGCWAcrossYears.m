clear all
close all
clc

folderLocation = 'C:\Karthik\Sweden\Chalmers\MasterThesis-PropOpt\OutputDataProcessing\OptimalProductivityOutputs\';
gcwCode='D';
resultFolderLocation = 'C:\Karthik\Sweden\Chalmers\MasterThesis-PropOpt\OutputDataProcessing\OptimalCombinationResults\SingleGCW\';

for i=1:4
    if(i==1)
        year = 2015;
    elseif(i==2)
        year = 2020;
    elseif(i==3)
        year = 2025;
    else
        year = 2030;
    end
    optFileLocation = strcat(folderLocation,int2str(year), '\', gcwCode,'1.mat');
    opt=load(optFileLocation);
    
    fixedCostCompResultLocation = strcat(resultFolderLocation, gcwCode,'\','Fixed.pdf');
    variableCostCompResultLocation = strcat(resultFolderLocation, gcwCode,'\','Variable.pdf');
    prodCompResultLocation = strcat(resultFolderLocation, gcwCode,'\','Productivity.pdf');

    rev = opt.revMission;
    
    y1(1,i) = rev/(opt.cFixedConv/5);
    y1(2,i) = rev/(sum(opt.cFixedElec)/5);
    y2(1,i) = rev/opt.cDriver;
    y2(2,i) = rev/opt.cFuel;
%     y2(3,i) = rev/opt.cTolls;
%     y2(4,i) = rev/opt.cTyres;
%     y2(5,i) = rev/opt.cMnt;
    
    prod(i) = opt.P;
end

figure('name', 'Combination Fixed Costs')
hy1=bar(y1);
xticklabel_rotate([1:2],25,{'R to Fixed Cost (Conv)', 'R to Fixed Cost (Elec)'},'interpreter','none');
set(gcf, 'Position', get(0,'Screensize')); 
legend('GCW=50t', 'GCW=60t', 'GCW=70t', 'GCW=80t', 'Location', 'NorthWest');
saveas(gcf, fixedCostCompResultLocation);

figure('name', 'Variable Costs')
hy2=bar(y2);
xticklabel_rotate([1:2],0,{'R to Driver Costs', 'R to Fuel Costs'},'interpreter','none');
set(gcf, 'Position', get(0,'Screensize')); 
legend('GCW=50t', 'GCW=60t', 'GCW=70t', 'GCW=80t');
saveas(gcf, variableCostCompResultLocation);

figure('name', 'Productivity')
hy3=bar(prod);
xticklabel_rotate([1:4],0,{'GCW=50t', 'GCW=60t', 'GCW=70t', 'GCW=80t'},'interpreter','none');
set(gcf, 'Position', get(0,'Screensize')); 
saveas(gcf, prodCompResultLocation);