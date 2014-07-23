function mcu = d_mcu_pmsm_remy_173kW_411Nm

%==========================================================================
% Description: initialize a 173-kW MCU from Remy.
%
% Filename:                         d_mcu_pmsm_remy_173kW_411Nm.m
% Author:                           Bertrand Molimard, 91554
% Date:                             2011-02-15
%
% Info: - intended to be used with ExSAM topology.
%       - correspond to Remy HVH 250 High torque.
% 
% Syntax: - mcu=d_mcu_pmsm_remy_173kW_411Nm;
%==========================================================================

mcu.compInfo='MCU 173 kW Kollmorgen';
mcu.versionInfo.file_name='%PM%';
mcu.versionInfo.file_version='%PR%';

mcu.sampleTime            = 0.02;           % Sample time of the MCU [s]

% Characteristics *********************************************************
mcu.maximalTorque         = 411;            % Maximal/peak torque [Nm]
mcu.maximalPower          = 173000;         % Maximal/peak power [W]
mcu.maximalSpeed          = 10000;          % Maximal rotational speed [rpm]
mcu.motorSpeed_x          = [0:200:10000];  % Speed row [rpm]
mcu.maximalMotorTorque_y  = [399   399   399   398   398   398   398   397   397   397   397   397   397   397   397   397   396   396   396   387   372   355   338   321   304   288   273   258   244   231   220   209   199   190   182   174   167   160   154   148   143   138   134   129   125   121   117   113   110   107   104];
mcu.maximalRegenTorque_y  = [411   407   407   407   408   408   408   408   408   408   409   409   409   409   409   409   409   409   409   409   402   390   376   360   344   329   314   299   285   272   259   247   236   226   217   208   201   193   186   180   174   168   163   158   153   149   144   140   137   133  130];
mcu.continuousTorque      = 266;            % Continuous torque [Nm]
mcu.continuousPower       = 110000;         % Continuous power [W]
mcu.peakTime              = 10;             % Peak time [sec]
mcu.restTime              = 50;             % Rest time [sec]
mcu.averageEfficiencyEM   = 0.92;           % Average efficiency [0..1]

% Limitations *************************************************************
mcu.highSpeedLimit_x      = [0    10000   10200   12000];  % [rpm]
mcu.highSpeedLimit_y      = [1    1       0       0];
mcu.lowSpeedLimit_x       = [0    300     500     12000];  %  [rpm]
mcu.lowSpeedLimit_y       = [0    0       1       1];
mcu.highVoltageLimit_x    = [0    725     800     1000];   %  [V]
mcu.highVoltageLimit_y    = [1    1       0       0];
mcu.lowVoltageLimit_x     = [0    350     500     1000];   % [V]
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
