function mcu = d_mcu_pmsm_remy_HVH250_90

%==========================================================================
% Description: initialize HVH250 90mm SOM from Remy.
%
% Filename:                         d_mcu_pmsm_remy_HVH250_90.m
% Author:                           David Albo, BF68340
% Date:                             2013-12-11
%
% Info: - intended to be used with ExSAM topology.
%       - correspond to Remy HVH 250 90mm SOM.
% 
% Syntax: - mcu=d_mcu_pmsm_remy_HVH250_90;
%==========================================================================

mcu.compInfo='MCU HVH250_90 Remy';
mcu.versionInfo.file_name='d_mcu_pmsm_remy_HVH250_90.m';
mcu.versionInfo.file_version='2.0';

mcu.sampleTime            = 0.02;           % Sample time of the MCU [s]

% Characteristics *********************************************************
mcu.maximalTorque         = 335;            % Maximal/peak torque [Nm]
mcu.maximalPower          = 162000;         % Maximal/peak power [W]
mcu.maximalSpeed          = 12000;          % Maximal rotational speed [rpm]
mcu.motorSpeed_x          = [0:100:12000];  % Speed row [rpm]
mcu.maximalMotorTorque_y  = [331 329 329 329 329 329 329 329 329 329 329 329 329 328 328 328 328 328 328 328 328 328 328 327 327 327 327 327 327 327 327 327 327 327 327 326 326 326 326 326 326 326 325 321 316 311 305 299 293 287 281 275 268 262 256 251 245 239 234 228 223 217 212 207 203 198 194 189 185 181 178 174 171 167 164 161 158 155 152 149 146 144 141 139 136 134 132 130 128 126 124 122 120 118 116 114 113 111 109 108 106 105 103 102 100 99 98 96 95 94 93 91 90 89 88 87 86 85 84 83 82];
mcu.maximalRegenTorque_y  = [331 332 332 332 332 332 332 332 332 332 333 333 333 333 333 333 333 333 333 333 334 334 334 334 334 334 334 334 334 334 334 334 335 335 335 335 335 335 335 335 335 335 335 335 335 332 328 324 319 314 308 303 297 292 286 280 275 270 264 259 254 249 244 239 234 230 225 220 216 212 208 204 200 196 193 190 186 183 180 177 174 172 169 166 164 161 159 157 154 152 150 148 146 144 142 140 139 137 135 134 132 130 129 127 126 124 123 122 120 119 118 117 115 114 113 112 111 110 109 108 107;]
mcu.continuousTorque      = 225;            % Continuous torque [Nm]
mcu.continuousPower       = 116000;         % Continuous power [W]
mcu.peakTime              = 10;             % Peak time [sec]
mcu.restTime              = 50;             % Rest time [sec]
mcu.averageEfficiencyEM   = 0.9;            % Average efficiency [0..1]

% Limitations *************************************************************
mcu.highSpeedLimit_x      = [0    10000   10200   12000];  % [rpm]
mcu.highSpeedLimit_y      = [1    1       0       0];
mcu.lowSpeedLimit_x       = [0    100     300     12000];  %  [rpm]
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
