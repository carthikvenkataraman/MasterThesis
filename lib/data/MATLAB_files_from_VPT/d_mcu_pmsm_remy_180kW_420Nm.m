function mcu = d_mcu_pmsm_remy_180kW_420Nm

%==========================================================================
% Description: initialize a 180-kW MCU from Remy.
%
% Filename:                         d_mcu_pmsm_remy_180kW_420Nm.m
% Author:                           Bertrand Molimard, 91554
% Date:                             2011-02-15
%
% Info: - intended to be used with ExSAM topology.
%       - correspond to Remy HVH 250 115 mm Series.
% 
% Syntax: - mcu=d_mcu_pmsm_remy_180kW_420Nm;
%==========================================================================

mcu.compInfo='MCU 180 kW Kollmorgen';
mcu.versionInfo.file_name='%PM%';
mcu.versionInfo.file_version='%PR%';

mcu.sampleTime            = 0.02;           % Sample time of the MCU [s]

% Characteristics *********************************************************
mcu.maximalTorque         = 420;            % Maximal/peak torque [Nm]
mcu.maximalPower          = 180000;         % Maximal/peak power [W]
mcu.maximalSpeed          = 12000;          % Maximal rotational speed [rpm]
mcu.motorSpeed_x          = (0:200:12000);  % Speed row [rpm]
mcu.maximalMotorTorque_y  = [413   414   414   414   413   413   413   412   412   412   412   411   411   411   411   411   410   410   410   402   387   369   351   334   317   300   284   269   255   242   230   220   210   200   192   184   177   170   163   157   152   146   141   137   132   128   124   120   117   113   110   107   104   101    99    96    94    91    89    87 85];
mcu.maximalRegenTorque_y  = [418  417  417  417  418  418  418  418  419  419  419  419  420  420  420  420  420  421  420  420  415  402  386  370  354  339  323  309  295  281  268  256  246  236  226  218  210  202  196  189  183  177  172  167  162  158  154  150  146  142  139  136  133  130  127  125  122  120  117  115 113];
mcu.continuousTorque      = 275;            % Continuous torque [Nm]
mcu.continuousPower       = 105000;         % Continuous power [W]
mcu.peakTime              = 10;             % Peak time [sec]
mcu.restTime              = 50;             % Rest time [sec]
mcu.averageEfficiencyEM   = 0.9;            % Average efficiency [0..1]

% Limitations *************************************************************
mcu.highSpeedLimit_x      = [0    13000   13200   15000];  % [rpm]
mcu.highSpeedLimit_y      = [1    1       0       0];
mcu.lowSpeedLimit_x       = [0    100     200     13000];  %  [rpm]
mcu.lowSpeedLimit_y       = [0    0       1       1];
mcu.highVoltageLimit_x    = [0    1000    1100    1200];   %  [V]
mcu.highVoltageLimit_y    = [1    1       0       0];
mcu.lowVoltageLimit_x     = [0    100     150     1200];   % [V]
mcu.lowVoltageLimit_y     = [0    0       1       1];
mcu.windingTempLimit_x    = [0    160     170     1000];   % [degC] EM temperature
mcu.windingTempLimit_y    = [1    1       0       0];
mcu.heatsinkTempLimit_x   = [0    80      90      1000];   % [K] Heatsink temperature
mcu.heatsinkTempLimit_y   = [1    1       0       0];

% Peak torque limit: peak torque allowed during mcu.peakTime, then limited to
% mcu.coefficientPeakTorqueLimit of continuous torque during mcu.restTime
mcu.activatePeakTorqueLimit    = 0;            % Flag that activate/disable the peak torque limitation.
mcu.coefficientPeakTorqueLimit = 60/100;
mcu.ratioPeakTorqueLimit       = (mcu.maximalTorque-mcu.continuousTorque)*mcu.peakTime/((mcu.continuousTorque-mcu.coefficientPeakTorqueLimit*mcu.continuousTorque)*mcu.restTime);

% Peak power limit: peak power allowed during mcu.peakTime, then limited to
% mcu.coefficientPeakPowerLimit of continuous power during mcu.restTime
mcu.activatePeakPowerLimit     = 0;            % Flag that activate/disable the peak power limitation.
mcu.coefficientPeakPowerLimit  = 80/100;         
mcu.ratioPeakPowerLimit        = (mcu.maximalPower-mcu.continuousPower)*mcu.peakTime/((mcu.continuousPower-mcu.coefficientPeakPowerLimit*mcu.continuousPower)*mcu.restTime);

mcu.derateMotorPowerValue      = 0;            % Derate motoring power
mcu.derateRegenPowerValue      = 0;            % Derate regenerating power
mcu.derateMotorTorqueValue     = 0;            % Derate motoring torque
mcu.derateRegenTorqueValue     = 0;            % Derate regenerating torque

% Speed regulator *********************************************************
mcu.speedControl.K_P           = 0.02;         % Gain of the proportionnal part
mcu.speedControl.K_I           = 0;            % Gain of the integral part
