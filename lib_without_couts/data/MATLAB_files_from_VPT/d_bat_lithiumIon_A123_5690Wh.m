function bat = d_bat_lithiumIon_A123_5690Wh(SocInit)

%==========================================================================
% Description: initialize Li-ion battery with A123 4.4 Ah cells.
%
% Filename:                         d_bat_lithiumIon_A123_5690Wh.m
% Author:                           Jens Groot
% Date:                             
%
% Info: - Generation II of A123 cells
%       - Generation III system (with 198 cells)
% 
% Syntax: - d_bat_lithiumIon_A123_5690Wh              - SOC(0) = 42%
%         - d_bat_lithiumIon_A123_5690Wh(0.56)        - SOC(0) = 56%
%==========================================================================

if nargin<1
    SocInit=0.42;
end

assert(SocInit>0 & SocInit<=1,'Error. SOC init is out of bound [0..1]')

%Load parameter files *****************************************************
cell = d_cell_lithiumIon_A123_4Ah4;
bat=cell.bat;
bat.name ='Li-Ion A123 5690 Wh';
bat.version_info.file_name='%PM%';
bat.version_info.file_version='%PR%';
clear cell

% Number of cells *********************************************************
bat.No_cellSeries=198;
bat.No_strings=2;

%Initial Conditions *******************************************************
bat.rt_SOCInit=SocInit;              % State of charge [-] (0.5)
bat.Te_init=23+273.15;               % Initial cell temperature [K] (296.15)
bat.rt_initialSOH=1;                 % State of health init [-] (1)

% Thermal model (to be changed by user). Default value in ( ).
bat.b_thermalModel = 0;              % Select thermal model [-] (0) 
                                     % (0 sets T_ess to T_ambient, 1 activates thermal model on cell level,
                                     % 2 activate thermal model on system level including heating system).
run d_bat_thermalModel_hybridTruck   % Initialize thermal model 2.

%Model Options (to be changed by user). Default value in ( ).
bat.b_cellVariation=0;               % Cell-to-cell variation enable [-] (0)
bat.b_losses=1;                      % Cell losses (dynamics) [-] (1)
bat.b_precharge=0;                   % System pre-charge circuit enable [-] (0)
bat.b_check=0;                       % Check valid operation [-] (1)
bat.b_depression=0;                  % Voltage depression (for plug-in HEV) [-] (0)
bat.t_precharge=1;                   % Time constant for precharge [s] (1)
bat.Te_ambient=38+273.15;            % Ambient temperature [K] (311.15)
bat.co_depression = 1;               % enablat subsystem som inte används, men iom C koden så blir modellen ej körbar.
