clear all
close all
clc

% Engine BSFC vs RPM
rawData=d_engplnt_wh_vtc_d16g750eu5;
engineData=rawData.Tdata;
engineData(6,:)=[];       % 5 in case of D11, 3 for D13, 6 for D16
clear rawData;

rpmRange=engineData(1,2:end);
torqueRange=engineData(2:end,1)';
fcInMilligramsPerStroke=engineData(2:end,2:end);

numberOfCylinders=6;

for i=1:length(torqueRange)
    for j=1:length(rpmRange)
        powerInW(i,j)=torqueRange(i)*rpmRange(j)*(2*pi/60); % T(Nm)*RPM*(2*pi/60) W
        powerInkW(i,j)=powerInW(i,j)/1000;
        fcInGramsPerSecond(i,j) = fcInMilligramsPerStroke(i,j)*numberOfCylinders*(rpmRange(j)/2)*(1/60)*1e-3; % g/sec
        if(powerInW(i,j)~=0) 
            bsfcInGramsPerJoule(i,j) = fcInGramsPerSecond(i,j)/powerInW(i,j);   % grams/Joule
        else
            bsfcInGramsPerJoule(i,j) = 0;   % grams/Joule
        end
        bsfcInGramsPerkWh(i,j)=bsfcInGramsPerJoule(i,j)*3.6*1000000;%g/kWh
    end
end

figure()
[RR, TT]=meshgrid(rpmRange,torqueRange);
contour(RR,TT,bsfcInGramsPerkWh);

% Maximum brake torque vs RPM
% rawData=d_ems_wh_vtc_d16c610ec01;
% engineMaxTorqueData = rawData.mt_MAX_TORQUE_HIGH_MAP_y;
% clear rawData;
% 
% rpmRangeForMaximumBrakeTorque = engineMaxTorqueData(1,:);
% rpmRangeForMaximumBrakeTorque(1)=[];
% machineMaximumBrakeTorque = engineMaxTorqueData(2,:);
% machineMaximumBrakeTorque(1)=[];
% 
% save('Engine02.mat','rpmRange', 'torqueRange', 'bsfcInGramsPerJoule', 'rpmRangeForMaximumBrakeTorque', 'machineMaximumBrakeTorque'); 
                                                                    % Engine00.mat - D11, Engine01.mat - D13, Engine02.mat - D16