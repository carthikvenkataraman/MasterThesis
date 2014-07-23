function em = d_em_pmsm_kollmorgen_120kW_800Nm

%==========================================================================
% Description: initialize a 120-kW Permanant Magnet Synchronous Motor from Kollmorgen.
%
% Filename:                         d_em_pmsm_kollmorgen_120kW.m
% Author:                           Bertrand Molimard, 91554
% Date:                             2010-11-25
%
% Info: - originally created from d_eds_dm_120.m
%       - intended to be used with ISAM or ISG topologies.
% 
% Syntax: - em=d_em_pmsm_kollmorgen_120kW_800Nm;
%==========================================================================

em.compInfo='Electric Machine 120 kW Kollmorgen';
em.versionInfo.file_name='%PM%';
em.versionInfo.file_version='%PR%';

% Characteristics *********************************************************
em.rotorInertia         = 1.59;	  % EM inertia [kgm^2]

% Losses ******************************************************************
% Load loss data (losses includes EM and inverter)
em.motorMapSpeed_x=[0	300	600	900	1200 1500	1800	2100	2400	2700];
em.motorMapTorque_y=[-800 -700 -600 -500 -400 -300 -200 -100 0 100 200 300 400 500 600 700 800];
em.motorMapEfficiency_z=[
    0,0.783,0.859,0.893,0.909,0.923,0.935,0,0,0;
    0,0.804,0.866,0.899,0.911,0.929,0.942,0.938,0,0;
    0,0.816,0.873,0.902,0.918,0.932,0.936,0.939,0.94,0;
    0,0.832,0.887,0.908,0.921,0.933,0.934,0.941,0.94,0.925;
    0,0.843,0.889,0.911,0.927,0.931,0.925,0.944,0.944,0.939;
    0,0.856,0.893,0.903,0.925,0.929,0.93,0.943,0.942,0.952;
    0,0.874,0.896,0.913,0.924,0.925,0.939,0.934,0.931,0.936;
    0,0.88,0.883,0.873,0.902,0.901,0.897,0.904,0.911,0.919;
    0,0.55,0.7,0.7,0.7,0.7,0.7,0.7,0.7,0.7;
    0,0.88,0.883,0.873,0.902,0.901,0.897,0.904,0.911,0.919;
    0,0.874,0.896,0.913,0.924,0.925,0.939,0.934,0.931,0.936;
    0,0.856,0.893,0.903,0.925,0.929,0.93,0.943,0.942,0.952;
    0,0.843,0.889,0.911,0.927,0.931,0.925,0.944,0.944,0.939;
    0,0.832,0.887,0.908,0.921,0.933,0.934,0.941,0.94,0.925;
    0,0.816,0.873,0.902,0.918,0.932,0.936,0.939,0.94,0;
    0,0.804,0.866,0.899,0.911,0.929,0.942,0.938,0,0;
    0,0.783,0.859,0.893,0.909,0.923,0.935,0,0,0;]';

em.motorMapLoss_z=[
 0,6933,8298,9036,10105,10464,10700,0,0,0;
0,5413,6797,7397,8562,8519,8185,10000,0,0;
0,4259,5500,6123,6727,6849,7821,8728,9500,0;
0,3208,4007,4802,5420,5660,6617,6963,8132,9872;
0,2336,3148,3698,4006,4635,6063,5224,5959,7352;
0,1585,2274,3030,3064,3614,4279,4058,4658,4353;
0,910,1467,1817,2075,2549,2481,3094,3716,3964;
0,430,833,1368,1393,1766,2160,2379,2476,2540;
0,19,74,167,296,463,667,907,1185,1500;
0,430,833,1368,1393,1766,2160,2379,2476,2540;
0,910,1467,1817,2075,2549,2481,3094,3716,3964;
0,1585,2274,3030,3064,3614,4279,4058,4658,4353;
0,2336,3148,3698,4006,4635,6063,5224,5959,7352;
0,3208,4007,4802,5420,5660,6617,6963,8132,9872;
0,4259,5500,6123,6727,6849,7821,8728,9500,0;
0,5413,6797,7397,8562,8519,8185,10000,0,0;
0,6933,8298,9036,10105,10464,10700,0,0,0;]';

em.inverterEfficiency =1;        % Set inverter loss to zero since included above

% Motor thermal model ***********************************************************
em.activateThermalModel  = 0;
maximalTorque            = 800;     % Peak torque for 10 sec [Nm]
maximalPower             = 120000;  % Peak mechanical power [W]
continuousTorque         = 400;     % Continuous torque [Nm]
continuousPower          = 70000;   % Continuous mechanical power [W]
halfTorque               = 500;     % Peak torque for 30 sec [Nm]
halfPower                = (continuousPower+maximalPower)/2;      %[W]
initialTemp              = 50+273.15;    % Motor Temperature		[K]
halfTime                 = 30;       %[s], time to reach max allowed temperature at a load step from nominal to half torque/power
peakTime                 = 10;       %[s], time to reach max allowed temperature at a load step from nominal to peak torque/power
maxWindingTemp           = 180+273.15;   %[K], max allowed temperature before derating
em.coolantTemperature    = 50+273.15;    %[K], assumed average value during operation
nomWindingTemp           = 120+273.15;   %[K], temperature at nominal torque/power operation (i.e. 70 deg C rise above coolant)
Rth_TLF_nom              = (nomWindingTemp-em.coolantTemperature)/continuousTorque; %TorqueLimitFactor (TLF) at nominal torque
Cx                       = 10; %value 1 gives exact rise - wait for test data
Cth                      = -Cx*peakTime/(Rth_TLF_nom*log(1-(maxWindingTemp-nomWindingTemp)/ ...
    (Rth_TLF_nom*(maximalTorque-continuousTorque)))); %Thermal capacitance estimation
em.thermalTime           = Cth*Rth_TLF_nom;      %%Thermal time constant [s]

%TorqueLimitFactor at half and peak performance
Rth_TLF_half             = ((nomWindingTemp-em.coolantTemperature) + ((maxWindingTemp-nomWindingTemp)/ ...
    (1-exp(-halfTime/em.thermalTime))))/halfTorque;
Rth_TLF_peak             = ((nomWindingTemp-em.coolantTemperature) + ((maxWindingTemp-nomWindingTemp)/ ...
    (1-exp(-peakTime/em.thermalTime))))/maximalTorque;


%Create TorqueLimitFactor lookup-vectors
em.torqueLimitFactor_x   = [continuousTorque halfTorque maximalTorque];
em.torqueLimitFactor_y   = [Rth_TLF_nom Rth_TLF_half Rth_TLF_peak];

%Create PowerLimitFactor lookup-vectors
em.powerLimitFactor_x    = [continuousPower halfPower maximalPower];
em.powerLimitFactor_y    = em.torqueLimitFactor_x.*em.torqueLimitFactor_y./em.powerLimitFactor_x;

%Extend lookup-vectors
em.torqueLimitFactor_x   = [0 em.torqueLimitFactor_x];
em.torqueLimitFactor_y   = [0 em.torqueLimitFactor_y];
em.powerLimitFactor_x    = [0 em.powerLimitFactor_x];
em.powerLimitFactor_y    = [0 em.powerLimitFactor_y];
em.torqueLimitFactor_x   = [0   399   400   800];

% PEC thermal model, can be used to calculate PEC life ********************
em.loadanglefactor       = 1; % worst case=1, normal=0.45
em.pec_size              = 1; %1=600a, 0=900a
if em.pec_size == 1 %600a
    em.maxAcCurrent      = 280;
    em.igbt_gain         = 160/1000;
    em.diode_gain        = 200/1000;
    em.R_th              = 0.75;
    em.C_th              = 1.25;
else em.pec_size == 0 %900a
    em.maxAcCurrent      = 400;   
    em.igbt_gain         = 125/1000;
    em.diode_gain        = 160/1000;
    em.R_th              = 0.75;
    em.C_th              = 1.25;
end


