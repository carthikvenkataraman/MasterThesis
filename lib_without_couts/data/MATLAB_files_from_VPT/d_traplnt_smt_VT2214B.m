function traplnt = d_traplnt_smt_VT2214B

%==========================================================================
% Description: initialise a 14-speed manual gearbox from Volvo.
%
% Filename:                         d_tra_smt_VT2214B.m
% Author:                           Per Hillerborg / Sixten Berglund, BF63320
% Creation date:                    2007-07-09
%
% Info: - Data for transmission gearbox VT2214
%
% Syntax: - traplnt=d_traplnt_smt_VT2214B;
%==========================================================================

traplnt.comp_info='14 speeds Manual Transmission Volvo';
traplnt.file_name='%PM%';
traplnt.file_version='%PR%';

traplnt.gbxType = 2;  % 1-AT 2-SMT 3-AMT 4-AMTPS

traplnt.Gearbox_n       	= [1        2       3       4       5       6       7       8       9       10      11      12      13      14      ];  % [-]
traplnt.Gearbox_ratio       = [16.855	13.513	11.129	8.922	7.158	5.739	4.677	3.750	2.968	2.379	1.909	1.530	1.247	1.000   ];  % [-]
traplnt.Gearbox_eff         = [0.97     0.97    0.972   0.972   0.974   0.974   0.974   0.985   0.982   0.982   0.984   0.984   0.984   0.995   ];  % [-]
traplnt.Gearbox_losscoeff   = [0.0028   0.0038  0.0031  0.0043  0.0038  0.0053  0.0052  0.0074  0.0034  0.0047  0.0042  0.0059  0.0058  0.0082  ];  % []
traplnt.Gearbox_inertia     = [38.9     36.6    17.8    16.8    8.2     7.8     4.3     4.1     1.64    1.57    0.95    0.92    0.68    0.67    ];  % [kgm2] Gearbox inertia seen by outputshaft

% inertia
traplnt.Gearbox_input_shaft_inertia=0.1;

% other needed parameters
traplnt.Ts_Tecu1=0.02;

