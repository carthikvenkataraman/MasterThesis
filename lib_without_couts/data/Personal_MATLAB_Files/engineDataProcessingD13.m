clear all
close all
clc

% Engine BSFC vs RPM
rawData=d_engplnt_wh_vtc_d13c540eu5;
engineData=rawData.Tdata;
engineData(3,:)=[];       % 5 in case of D11, 3 for D13, 6 for D16
clear rawData;

rpmRange=engineData(1,2:end);
torqueRange=engineData(2:31,1)';
fcInMilligramsPerStroke=engineData(2:31,2:end);

numberOfCylinders=6;

for i=1:length(torqueRange)
    for j=1:length(rpmRange)
        powerInW(i,j)=torqueRange(i)*rpmRange(j)*(2*pi/60); % T(Nm)*RPM*(2*pi/60) W
        powerInkW(i,j)=powerInW(i,j)/1000;
        fcInGramsPerSecond(i,j) = fcInMilligramsPerStroke(i,j)*1e-3*numberOfCylinders*(rpmRange(j)/2)*(1/60); % g/sec
        bsfcInGramsPerJoule(i,j) = fcInGramsPerSecond(i,j)/powerInW(i,j);   % grams/Joule
    end
end

% Maximum brake torque vs RPM
rawData=d_ems_wh_vtc_d13c540eu5;
engineMaxTorqueData = rawData.mt_MAX_TORQUE_HIGH_MAP_y;
clear rawData;

rpmRangeForMaximumBrakeTorque = engineMaxTorqueData(1,:);
machineMaximumBrakeTorque = engineMaxTorqueData(2,:);
powerInWCalc=machineMaximumBrakeTorque.*rpmRangeForMaximumBrakeTorque*(2*pi/60);

save('Engine01.mat','rpmRange', 'torqueRange', 'bsfcInGramsPerJoule', 'rpmRangeForMaximumBrakeTorque', 'machineMaximumBrakeTorque'); 
                                                                    % Engine00.mat - D11, Engine01.mat - D13, Engine02.mat - D16