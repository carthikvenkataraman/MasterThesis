close all;
clear all;
clc;

data = d_traplnt_conv_amt_directdrive;
gearRatios = data.Gearbox_ratio';
gearEfficiencies = data.Gearbox_eff';
gearInertias = [36.6    17.8    16.8    8.2     7.8     4.3     4.1     1.64    1.57    0.95    0.68    0.67];
save('Gearbox.mat', 'gearRatios', 'gearEfficiencies', 'gearInertias');
clear data;