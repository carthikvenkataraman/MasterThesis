clear all
close all
clc

load('FitnessEachGeneration.txt');  % Fitness of each individual in every population
load('PopulationEachGeneration.txt'); % Population in each generation
load('fitness_vs_generation.txt');  % Improvement of fitness over generations

numberOfGenerations=500;
populationSize=30;
numberOfVariables=2;
variableRange=5;
fitness=zeros(populationSize,1);

% Sort populations; There are 500 populations; Each population contains 30 individuals; Each individual contains 2 variables
% Each time, the population matrix is created before it is plotted. It contains 30 individuals of 2 variables each.

populationCount=1;
fitnessCount=1;
populationtest=zeros(numberOfGenerations, populationSize, numberOfVariables);

for i=1:numberOfGenerations
    for j=1:populationSize
        fitnessFromC(i,j)=FitnessEachGeneration(fitnessCount);
        fitnessCount=fitnessCount+1;
        for k=1:numberOfVariables
            population(i,j,k)=PopulationEachGeneration(populationCount);
            populationCount=populationCount+1;
        end
    end
end

% fitnessFigureHandle = figure;
% hold on;
% set(fitnessFigureHandle, 'Position', [50,50,500,200]);
% set(fitnessFigureHandle, 'DoubleBuffer', 'on');
% axis([1 numberOfGenerations -25 2]);
% bestPlotHandle = plot(1:numberOfGenerations,ones(1,numberOfGenerations)*-25);
% title('Fitness measure over generations');
% set(get(gca,'XLabel'),'String','Generation Number');
% set(get(gca,'YLabel'),'String','Fitness measure');
% textHandle = text(10,-2,sprintf('best: %4.3f',0.0));
% hold off;
% drawnow;

surfaceFigureHandle = figure;
hold on;
set(surfaceFigureHandle, 'DoubleBuffer', 'on');
delta = 0.1;
limit=fix(2*variableRange/delta)+1;
[xValues,yValues] = meshgrid(-variableRange:delta:variableRange,-variableRange:delta:variableRange);
zValues=zeros(limit,limit);
for i=1:size(xValues,1)
    for j=1:size(yValues,1)
%         zValues(i,j)=GPR(xValues(i,j),yValues(i,j));
          zValues(i,j)=Booth(xValues(i,j),yValues(i,j));
%         zValues(i,j)=Beale(xValues(i,j),yValues(i,j));
    end
end
surfl(xValues,yValues,zValues)
%colormap gray;
shading interp;
colorbar
view([7 4 0]);
decodedPopulation=zeros(populationSize,2);
populationPlotHandle = plot3(decodedPopulation(:,1),decodedPopulation(:,2),fitness(:),'kp');
hold off;
drawnow;

% for iGeneration = 1:numberOfGenerations
%     maximumFitness=fitness_vs_generation(iGeneration); 
%     plotvector = get(bestPlotHandle, 'YData');
%     plotvector(iGeneration) = maximumFitness;
%     set(bestPlotHandle, 'YData', plotvector);
%     set(textHandle, 'String', sprintf('best: %4.3f', maximumFitness));
%     
%     for i=1:populationSize
%         decodedPopulation(i,1)=population(iGeneration,i,1);
%         decodedPopulation(i,2)=population(iGeneration,i,2);
%         fitness(i)=-fitnessFromC(iGeneration,i);
%     end
%     set(populationPlotHandle, 'XData', decodedPopulation(:,1), 'YData', decodedPopulation(:,2), 'ZData', fitness(:));
%     drawnow;
% %     keyboard;
% end