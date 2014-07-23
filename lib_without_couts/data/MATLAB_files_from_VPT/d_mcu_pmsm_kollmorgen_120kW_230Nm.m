function mcu = d_mcu_pmsm_kollmorgen_120kW_230Nm

%==========================================================================
% Description: initialize a 120-kW MCU from Kollmorgen.
%
% Filename:                         d_mcu_pmsm_kollmorgen_120kW_230Nm.m
% Author:                           Bertrand Molimard, 91554
% Date:                             2011-02-15
%
% Info: - intended to be used with ExSAM topology.
%       - preliminary data. Original map from Remy.
% 
% Syntax: - mcu=d_mcu_pmsm_kollmorgen_120kW_230Nm;
%==========================================================================

mcu.compInfo='MCU 120 kW Kollmorgen';
mcu.versionInfo.file_name='%PM%';
mcu.versionInfo.file_version='%PR%';

mcu.sampleTime            = 0.02;           % Sample time of the MCU [s]

% Characteristics *********************************************************
mcu.maximalTorque         = 230;            % Maximal/peak torque [Nm]
mcu.maximalPower          = 120000;         % Maximal/peak power [W]
mcu.maximalSpeed          = 12000;          % Maximal rotational speed [rpm]
mcu.motorSpeed_x          = [0:100:12000];  % Speed row [rpm]
mcu.maximalMotorTorque_y  = min(mcu.maximalTorque, mcu.maximalPower./(mcu.motorSpeed_x*pi/30+eps));
mcu.maximalRegenTorque_y  = mcu.maximalMotorTorque_y;
mcu.continuousTorque      = 115;            % Continuous torque [Nm]
mcu.continuousPower       = 70000;          % Continuous power [W]
mcu.peakTime              = 10;             % Peak time [sec]
mcu.restTime              = 50;             % Rest time [sec]
mcu.averageEfficiencyEM   = 0.9;            % Average efficiency [0..1]

% Limitations *************************************************************
mcu.highSpeedLimit_x      = [0    10000   10200   12000];  % [rpm]
mcu.highSpeedLimit_y      = [1    1       0       0];
mcu.lowSpeedLimit_x       = [0    100     200     12000];  %  [rpm]
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
mcu.speedControl.K_P           = 0.03;         % Gain of the proportionnal part
mcu.speedControl.K_I           = 0;            % Gain of the integral part
