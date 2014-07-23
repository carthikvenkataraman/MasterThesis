function traplnt=d_traplnt_conv_amt_directdrive

%==========================================================================
% Description: initialise a 12-speed AMT gearbox.
%
% Filename:                         d_traplnt_conv_amt_directdrive.m
% Author:                           Peter Templin, BF63320
% Creation date:                    2011-03-02
%
% Info: - 
%
% Syntax: - traplnt=d_traplnt_conv_amt_directdrive;
%==========================================================================

traplnt.comp_info='AMTD DirectDrive';
traplnt.version_info.file_name='%PM%';
traplnt.version_info.file_version='%PR%';

traplnt.gbxType = 3;  % 1-AT 2-SMT 3-AMT 4-AMTPS
traplnt.GBXTYPE_AMT=1;
traplnt.GBXTYPE_AMTPS=0;
traplnt.PTO_CONNECTION=0;

% gear ratios and losses
% gear	ratio i_g	eta_g	const.	P_idle/[W] = const.*(n_in/[rpm])^1,7		
% $$$ temp=[
% $$$ 1	14.941	0.972	0.0022
% $$$ 2	11.729	0.972	0.0031
% $$$ 3	9.036	0.974	0.0027
% $$$ 4	7.094	0.974	0.0038
% $$$ 5	5.538	0.974	0.0037
% $$$ 6	4.348	0.985	0.0055
% $$$ 7	3.436	0.982	0.0024
% $$$ 8	2.698	0.982	0.0034
% $$$ 9	2.078	0.984	0.0029
% $$$ 10	1.632	0.984	0.0042
% $$$ 11	1.274	0.984	0.0041
% $$$ 12	1.000	0.995	0.0060
% $$$ ];
  
% InputShaft to OutputShaft related vectors
traplnt.Gearbox_n=        [1,        2,        3,        4,        5,        6,        7,        8,        9,        10,       11,       12]'; 
traplnt.Gearbox_ratio=    [14.941,   11.729,   9.036,    7.094,    5.538,    4.348,    3.436,    2.698,    2.078,    1.632,    1.274,    1.000]';
traplnt.Gearbox_eff=      [0.972,    0.972,    0.974,    0.974,    0.974,    0.985,    0.982,    0.982,    0.984,    0.984,    0.984,    0.995]';
traplnt.Gearbox_losscoeff=[0.0022,   0.0031,   0.0027,   0.0038,   0.0037,   0.0055,   0.0024,   0.0034,   0.0029,   0.0042,   0.0041,   0.0060]';
 
% CounterShaft to OutputShaft related vectors
traplnt.Pto_n =     [1.000  2.000   3.000   4.000   5.000   6.000   7.000   8.000   9.000   10.00   11.00   12.00]';
traplnt.Pto_ratio = [10.526 10.526  6.366   6.366   3.902   3.902   2.421   2.421   1.464   1.464   0.897   0.897]';
traplnt.Pto_eff =   [0.981  0.981   0.982   0.982   0.982   0.982   0.991   0.991   0.992   0.992   0.992   0.992]';


% Gearbox gear pair ratios
% 44/31=1.4193548387096775
% 39/35=1.1142857142857143
traplnt.ratio_split_gear=[0; 1.4193548387096775; 1.1142857142857143]; % for directdrive gearbox
% 46/19=2.4210526315789473
% 41/28=1.4642857142857142
% 35/39=0.8974358974358975
traplnt.ratio_base_gear=[0; 2.4210526315789473; 1.4642857142857142; 0.8974358974358975];
% 1+77/23=4.347826086956522
traplnt.ratio_range_gear=[0; 4.347826086956522; 1];
traplnt.clutch_max_torque=5000;
traplnt.Gearbox_input_shaft_inertia=0.1;

% Maximum clutch torque that the discs can transfer
traplnt.clutch_max_torque=5000;

traplnt.Gearbox_input_shaft_inertia=0.1;

% Shaft inertia parameters used by "tra_conv_amt_int.mdl"
traplnt.inputShaftInertia=0.05;
traplnt.counterShaftInertia=0.05;
traplnt.outputShaftInertia=0.05;





