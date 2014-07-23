function traplnt = d_motor_transmission

%==========================================================================
% Description: initialise a 2-speed gearbox for electric motors on trailing units
%
% Filename:                         d_motor_transmission.m
% Author:                           Karthik Venkataraman, A047977
% Creation date:                    2014-06-02
%
% Info: - Data for transmission gearbox for electric motors
%
% Syntax: - traplnt=d_traplnt_smt_VT2214B;
%==========================================================================

traplnt.comp_info='2 Speed Gearbox for electric transmission';
traplnt.file_name='%PM%';
traplnt.file_version='%PR%';

traplnt.gbxType = 2;  % 1-AT 2-SMT 3-AMT 4-AMTPS

traplnt.Gearbox_n       	= [1        2   ];  % [-]
traplnt.Gearbox_ratio       = [7.158	5.739	];  % [-]
traplnt.Gearbox_eff         = [0.974   0.974   ];  % [-]
traplnt.Gearbox_losscoeff   = [0.0038  0.0053  ];  % []
traplnt.Gearbox_inertia     = [8.2     7.8     ];  % [kgm2] Gearbox inertia seen by outputshaft

% inertia
traplnt.Gearbox_input_shaft_inertia=0.1;

% other needed parameters
traplnt.Ts_Tecu1=0.02;

