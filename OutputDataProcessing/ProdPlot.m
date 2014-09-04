%% To plot how productivities of different combinations vary over years

clear all
close all
clc

C1 = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsProductivity/Mission1/Productivity15.mat');
C2 = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsProductivity/Mission2/Productivity15.mat');
C3 = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsProductivity/Mission3/Productivity15.mat');

figure('name', 'Productivity trends for different combinations compared');
plot(C1.P);
hold all
plot(C2.P);
hold all
plot(C3.P);
legend('1 Mission per day', '2 Missions per day', '3 Missions per day');
xlabel('Year'), ylabel('Mission N-year Productivity (EUR/EUR)');
set(gca,'XTick',[1 2 3 4]);
set(gca,'XTickLabel',{'2015','2020','2025', '2030'});
grid on
saveas(gcf, 'PlotsProductivity/ProdTrendMissionNum15.pdf');