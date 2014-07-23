function bat = d_bat_lithiumIon_SAFT_19p2kWh(SocInit)
%==========================================================================
% FILENAME
% C:\GSP_Projects\GSP_P4388_3_2\GSP\base\components\electric_energy_storage_systems\data\d_bat_lithiumIon_SAFT_19p2kWh.m
%--------------------------------------------------------------------------
% ABSTRACT
%   Describe the script purpose...
%--------------------------------------------------------------------------
% EXAMPLES
%   Define some examples on how to use the script...
%--------------------------------------------------------------------------
% SYNTAX
%   Define the script syntax...
%--------------------------------------------------------------------------
% INPUT
%   SocInit
%--------------------------------------------------------------------------
% OUTPUT
%    bat
%--------------------------------------------------------------------------
% REVISION HISTORY
% History is given at the end of the file.
%--------------------------------------------------------------------------
% DEPENDENCIES
%--------------------------------------------------------------------------
% ALGORITHM
%==========================================================================
try
	
	%==========================================================================
	% Description: Initialize Li-ion battery with 30 Ah cells
	%              (based on measurements on the real cell (VTEC))
	%
	% Filename:                         d_bat_lithiumIon_SAFT_19p2kWh.m
	% Author:                           Andreas Jansson, BF68340
	% Date:                             2013-06-01
	%
	% Info: - 30 Ah cell, 192 cells 19.2 kWh battery
	%
	% Syntax: - d_bat_lithiumIon_SAFT_19p2kWh              - SOC(0) = 42% and SOH(0) = 100%
	%         - d_bat_lithiumIon_SAFT_19p2kWh(0.56)        - SOC(0) = 56% and SOH(0) = 100%
	%         - d_bat_lithiumIon_SAFT_19p2kWh(0.56,0.95)   - SOC(0) = 56% and SOH(0) =  95%
	%==========================================================================
	
	if nargin<1
		SocInit=0.42;
	end
	
	assert(SocInit>0 & SocInit<=1,'Error. SOC init is out of bound [0..1]')
	
	%Load parameter files *****************************************************
	cell = d_cell_lithiumIon_SAFT_30Ah;
	bat=cell.bat;
	
	
	bat.name ='SAFT 19.2 kWh';
	% 
	version_info.file_name='d_bat_lithiumIon_SAFT_19p2kWh.m';
	version_info.file_version='1.2';
	version_info.file_status = 'IN_REVIEW';
	version_info.kola_name = 'KOLA';
	bat.version_info(end+1)=version_info;
	clear cell
	
	% Number of cells *********************************************************
	bat.No_cellSeries=192;
	bat.No_strings=1;
	
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
	bat.R_contact=0;%0.0014;                % Contactors resistance
	bat.co_depression = 1;               % enablat subsystem som inte används, men iom C koden så blir modellen ej körbar.
	
	
catch exception
	
	% Manage the error
	if isappdata(0, 'ErrManager')
		
		% Display the error using specific interface
		err_manager = getappdata(0, 'ErrManager');
		err_manager = err_manager.display(exception);
		setappdata(0, 'ErrManager', err_manager);
		
	else
		
		disp(exception.getReport);
		
	end
	
end
%==========================================================================

%==========================================================================
% HISTORY
% Revision 1.2 (IN_REVIEW)
%   Created:  28-APR-2014 14:22:03      T0C0339 (Albo David)
%     Updated with gsp_format_script_data
% 
% Revision 1.1 (REJECTED)
%   Created:  25-APR-2014 14:21:31      T0C0339 (Albo David)
%     Updated with gsp_format_script_data
% 
% Revision 1.0 (IN_REVIEW)
%   Created:  25-JUN-2013 11:08:12      A021647 (Jansson Andreas)
%     Initial Revision
% 
%==========================================================================
