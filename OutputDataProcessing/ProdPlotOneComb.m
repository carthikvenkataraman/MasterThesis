%% To plot how productivities of different combinations vary over years

clear all
close all
clc

C1 = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsProductivity/Mission3/ProductivityTractorST.mat');
C2 = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsProductivity/Mission3/Productivity11.mat');
C3 = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsProductivity/Mission3/Productivity15.mat');
C4 = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsProductivity/Mission3/Productivity33.mat');
C5 = load('/home/karthik/Documents/GitHubRepos/MasterThesis-PropOpt/OutputDataProcessing/OutputsProductivity/Mission3/Productivity57.mat');

figure('name', 'Productivity trends for different combinations compared');
plot(C1.P);
hold all
plot(C2.P);
hold all
plot(C3.P);
hold all
plot(C4.P);
hold all
plot(C5.P);
legend('Tractor-SemiTrailer', 'A-Double', '011-010-00-000', '011-010-01-010', '011-111-11-111');
xlabel('Year'), ylabel('Mission N-year Productivity (EUR/EUR)');
set(gca,'XTick',[1 2 3 4]);
set(gca,'XTickLabel',{'2015','2020','2025', '2030'});
grid on
saveas(gcf, 'PlotsProductivity/Mission3/ProdTrendMissionNum1.pdf');