#include <iostream>
#include <random>
#include <vector>
#include <utility>
#include <math.h>
#include <fstream>

typedef std::vector<double> Individual;      // Each individual =  x & y
typedef std::vector<Individual> Population; // Many individuals make up a population
typedef std::vector<double> Fitnesses;       // Vector containing the fitness of each individual in the population

double GetRandomDouble() {
   std::random_device rdev{};
   std::default_random_engine engineRandom{rdev()};
   std::uniform_real_distribution<double> distribution(0.0,1.0);
   double randomDouble =  distribution(engineRandom);
   return randomDouble;
}

int GetRandomIndex(int populationSize) {
   std::random_device rdev{};
   std::default_random_engine engineRandom{rdev()};
   std::uniform_int_distribution<int> distribution(0,populationSize-1); // Important - 0 to N-1 and not 1 to N!!
   int randomIndex = distribution(engineRandom);
   return randomIndex;
}

Individual GetIndividual(int numberOfVariables) {
   Individual tempInd;
   for(int i=0;i<numberOfVariables;i++) {
	tempInd.push_back(GetRandomDouble());
   }
   return tempInd;
}

Population InitialisePopulation(int populationSize, int numberOfVariables, Population& population){
   for(int i=0;i<populationSize;i++) {
	Individual tempInd = GetIndividual(numberOfVariables);	// Individual = std::vector<float> - vector of floats
        population.push_back(tempInd);
   }
   return population;
}

double RescaleVariable(double tempVariable, std::vector<double>& variableRange) {
   double variableLower = variableRange[0];
   double variableUpper = variableRange[1];
   double rescaledVariable = variableLower+(tempVariable*(variableUpper-variableLower));
   return rescaledVariable;
}

double EvaluateIndividualFitness(Individual tempInd, std::vector<std::vector<double>>& rangeAllVariables) {
   double x,y, xTemp, yTemp;
   xTemp = tempInd[0];
   yTemp = tempInd[1];
   std::vector<double> xRange = rangeAllVariables[0];
   std::vector<double> yRange = rangeAllVariables[1];
   x = RescaleVariable(xTemp, xRange);
   y = RescaleVariable(yTemp, yRange);
   /*// Goldstein Price Function	
   double testFunctionValue = (1+(pow(x+y+1,2))*(19-14*x+3*(pow(x,2))-14*y+6*x*y+3*y*y))*
		   (30+(pow(2*x-3*y,2))*(18-32*x+12*x*x+48*y-36*x*y+27*y*y)); // Function tested. Check testGPRfunction.cpp; f(0,-1)=3*/

   /*// Booth's function
   double testFunctionValue = pow(x+2*y-7,2)+pow(2*x+y-5,2); // f(1,3)=0*/

   // Beale's function
   double testFunctionValue = pow(1.5-x+x*y,2)+pow(2.25-x+x*y*y,2)+
				pow(2.625-x+x*y*y*y,2); // f(3,0.5)=0*/

   double individualFitnessValue = -testFunctionValue;  // Better than 1/functionValue - avoid awkward divByZero situations
   return individualFitnessValue;
}

Fitnesses EvaluatePopulationFitness(Population& population, std::vector<std::vector<double>>& rangeAllVariables) {
   int populationSize = population.size();
   Fitnesses fitness;
   for(int i=0;i<populationSize;i++) {
           Individual tempInd = population[i];
           double tempFitness = EvaluateIndividualFitness(tempInd, rangeAllVariables);
           fitness.push_back(tempFitness);
   }
   return fitness;
}

int GetBestIndividualIndex(Fitnesses& fitness) { //, Population& population, std::vector<std::vector<double>>& rangeAllVariables) {
   int populationSize = fitness.size();
   long double maximumFitness = fitness[0];
   int bestIndividualIndex = 0;
   for(int i=1;i<populationSize;i++) {
       	if(fitness[i]>maximumFitness) {
             bestIndividualIndex = i;
        }
   }
   return bestIndividualIndex;
}

void PrintMembersOfPopulation(Population& population) {
   // Prints UNSCALED values of individuals only!
   int populationSize = population.size();
   for(int i=0;i<populationSize;i++) {
	std::cout<<"Individual "<<i<<": ";
	Individual individualToPrint = population[i];
	int numberOfVariables = individualToPrint.size();
	for(int j=0;j<numberOfVariables;j++)
	    std::cout<<individualToPrint[j]<<"	";
	std::cout<<std::endl;
   }
}

void PrintIndividual(Individual& individual) {
   int numberOfVariables = individual.size();
   for(int i=0;i<numberOfVariables;i++)
       std::cout<<individual[i]<<"	";
   std::cout<<std::endl;
}

void PrintRescaledIndividual(Individual& individual, std::vector<std::vector<double>>& rangeAllVariables) {
   int numberOfVariables = individual.size();
   for(int j=0;j<numberOfVariables;j++) {
       std::vector<double> variableRange = rangeAllVariables[j];
       std::cout<<RescaleVariable(individual[j], variableRange)<<"  ";
   }
   std::cout<<std::endl;
}

void PrintRescaledMembersOfPopulation(Population& population, std::vector<std::vector<double>>& rangeAllVariables) {
   // Prints SCALED values of individuals!
   int populationSize = population.size();
   for(int i=0;i<populationSize;i++) {
	std::cout<<"Individual "<<i<<": ";
	Individual individualToPrint = population[i];
	int numberOfVariables = individualToPrint.size();
	for(int j=0;j<numberOfVariables;j++) {
	    std::vector<double> variableRange = rangeAllVariables[j];
	    std::cout<<RescaleVariable(individualToPrint[j], variableRange)<<"	";
	}
	std::cout<<std::endl;
   }
}

int GetWeakestIndividual(std::vector<int>& randomIndividualIndex, std::vector<double>& randomIndividualFitness) {
   int tournamentSize = randomIndividualFitness.size();
   double minimumFitness = randomIndividualFitness[0];
   int minimumFitnessIndex = randomIndividualIndex[0];
   for(int i=1;i<tournamentSize;i++) {
	if(randomIndividualFitness[i]<minimumFitness) {
	   minimumFitness = randomIndividualFitness[i];
	   minimumFitnessIndex=randomIndividualIndex[i];
	}
   }
   return minimumFitnessIndex;
}

int GetFittestIndividual(std::vector<int>& randomIndividualIndex, std::vector<double>& randomIndividualFitness) {
   int tournamentSize = randomIndividualFitness.size();
   double maximumFitness = randomIndividualFitness[0];
   int maximumFitnessIndex = randomIndividualIndex[0];
   for(int i=1;i<tournamentSize;i++) {
	if(randomIndividualFitness[i]>maximumFitness) {
	   maximumFitness = randomIndividualFitness[i];
	   maximumFitnessIndex=randomIndividualIndex[i];
	}
   }
   return maximumFitnessIndex;
}

int TournamentSelection(Fitnesses& fitnessForSelection, float tournamentSelectionParameter, int tournamentSize) {
  std::vector<int> randomIndividualIndex;
  int selectedIndividualIndex;
  std::vector<double> randomIndividualFitness;
  int populationSize = fitnessForSelection.size();

  // Select two individuals to participate in the tournament
  for(int i=0;i<tournamentSize;i++) {
	// Contains the indices of two individuals participating in the tournament
	int temporaryIndex = GetRandomIndex(populationSize);
	randomIndividualIndex.push_back(temporaryIndex); // Index needed to pass to main()
	//std::cout<<"Randomly generated individual number" <<i+1<<" is: "<<randomIndividualIndex[i]<<" ";
	//Contains fitnesses of the two participating individuals
	double temporaryFitness = fitnessForSelection[temporaryIndex];
	randomIndividualFitness.push_back(temporaryFitness);
	//std::cout<<" with fitness: "<<temporaryFitness<<std::endl;
   }
  
  // Generate random floating point number (double) for tournament selection
  double randomDouble = GetRandomDouble();
  //std::cout<<"Random double generated for tournament selection: "<<randomDouble<<std::endl;

  int fittestIndividualIndex, weakestIndividualIndex;

  // Choose fitter individual if random double is lesser than tournament selection parameter
  if(randomDouble < tournamentSelectionParameter) {
	// Return fittest individual 
	//std::cout<<"Random number less than TS parameter. Returning fittest individual"<<std::endl;
	fittestIndividualIndex = GetFittestIndividual(randomIndividualIndex, randomIndividualFitness);
	//std::cout<<"Fittest individual number "<<fittestIndividualIndex<<" has function value "<<
                    //fitnessForSelection[fittestIndividualIndex]<<std::endl;
	return fittestIndividualIndex;
	/*if(randomIndividualFitness[0]>randomIndividualFitness[1])
	    return randomIndividualIndex[0];
	else
	    return randomIndividualIndex[1];*/
  }
  else { 
	// Return weakest individual
	//std::cout<<"Random number greater than TS parameter. Returning weakest individual"<<std::endl;
	weakestIndividualIndex = GetWeakestIndividual(randomIndividualIndex, randomIndividualFitness);
	//std::cout<<"Weakest individual number "<<weakestIndividualIndex<<" has function value "<<
                    //fitnessForSelection[weakestIndividualIndex]<<std::endl;
	return weakestIndividualIndex;
	/*if(randomIndividualFitness[0]>randomIndividualFitness[1]) 
	    return randomIndividualIndex[1];
	else
	    return randomIndividualIndex[0];*/
  }
}

Population CrossOver(Population& individualsToCrossover) {
   int crossoverPoint=1;	// In this case, the only crossover point is at position 1
   Individual individual1 = individualsToCrossover[0];
   Individual individual2 = individualsToCrossover[1];
   Individual crossedOverIndividual1, crossedOverIndividual2;
   //std::vector<Individual> crossedOverIndividuals;
   int numberOfVariables = individual1.size();  
 
   // Perform crossover
   for(int i=0;i<numberOfVariables;i++) {
	if(i<crossoverPoint) {
	   crossedOverIndividual1.push_back(individual1[i]);
	   crossedOverIndividual2.push_back(individual2[i]);
	}
	else {
	   crossedOverIndividual1.push_back(individual2[i]);
	   crossedOverIndividual2.push_back(individual1[i]);
	}
   }
   // Place crossed over individuals in a single vector to be returned to main()
   std::vector<Individual> crossedOverIndividuals{crossedOverIndividual1, crossedOverIndividual2};
//   crossedOverIndividuals.push_back(crossedOverIndividual1); 	   
//   crossedOverIndividuals.push_back(crossedOverIndividual2); 	   
   return crossedOverIndividuals;
}

void MutatePopulation(Population& tempPopulation, double mutationProbability, float creepRateMutation) {
   int populationSize = tempPopulation.size();
   for(int i=0;i<populationSize;i++) {
	Individual individualToMutate = tempPopulation[i]; // Replace tempPopulation[i] with the altered individual later
	//std::cout<<"Considering individual "<<i<<" for mutation: ";
	//PrintIndividual(individualToMutate);
	for(int j=0;j<individualToMutate.size();j++) {
	   //std::cout<<"Should gene "<<j<<" of the chosen individual be mutated?"<<std::endl;
	   double currentGene = individualToMutate[j];    // Replace individualToMutate[j] with the mutated gene later	
	   double randomDouble = GetRandomDouble();
	   //std::cout<<"Random number generated for mutation test is: "<<randomDouble<<std::endl;
	   if(randomDouble<mutationProbability) {
		//std::cout<<"Yes, mutating gene "<<j<<" of Individual "<<i<<std::endl;
		//std::cout<<"Current value of gene: "<<currentGene<<std::endl;
		individualToMutate[j]=currentGene-(creepRateMutation/2)+(creepRateMutation*GetRandomDouble()); // Gene mutated
		if(individualToMutate[j]>1)
 		    individualToMutate[j]=1;
		if(individualToMutate[j]<0)
		    individualToMutate[j]=0;
		//std::cout<<"New value of gene: "<<individualToMutate[j]<<std::endl;
	   }
	}
	tempPopulation[i]=individualToMutate; // Individual replaced with mutated one   
	//std::cout<<"Individual "<<i<<" has, after mutation, changed to ";
	//PrintIndividual(tempPopulation[i]);
	//std::cout<<std::endl;
   }  
}

void StoreCurrentPopulationInFile(Population& population, std::fstream& populationEachGeneration, std::vector<std::vector<double>>& rangeAllVariables) {
   populationEachGeneration.open("PopulationEachGeneration.txt", std::fstream::in | std::fstream::out | std::fstream::app);
   int populationSize = population.size();
   for(int i=0;i<populationSize;i++) {
	Individual tempIndividual = population[i];
	int individualSize = tempIndividual.size();
	//std::cout<<"Storing individual in file"<<std::endl;
	//PrintIndividual(tempIndividual);
   	std::vector<double> xRange = rangeAllVariables[0];
   	std::vector<double> yRange = rangeAllVariables[1];

	for(int j=0;j<individualSize;j++) {
	    std::vector<double> variableRange = (j==0)?xRange:yRange;
	    populationEachGeneration << RescaleVariable(tempIndividual[j], variableRange) <<'\n';
	}
   }
   populationEachGeneration.close();
}

void StoreCurrentFitnessInPopulation(Fitnesses& fitness, std::fstream& fitnessEachGeneration) {
   fitnessEachGeneration.open("FitnessEachGeneration.txt", std::fstream::in | std::fstream::out | std::fstream::app);
   int populationSize = fitness.size();
   for(int i=0;i<populationSize;i++) {
	fitnessEachGeneration << fitness[i] <<'\n';
   }	
   fitnessEachGeneration.close();
}

int main() {

   // Declare GA parameters - pop. size, variables, etc
   const int populationSize = 30;
   const int numberOfGenerations = 500;
   const int numberOfVariables = 2;
   const int tournamentSize = 2;
   const double tournamentSelectionParameter = 0.75;
   const float crossoverProbability = 0.7;
   const float creepRateMutation = 0.1;
   const double mutationProbability = 0.25;

   // Initialise the population
   Population temporaryPopulation, population;
   population = InitialisePopulation(populationSize,numberOfVariables, temporaryPopulation);
   // Check if population is correctly initialised
   std::cout<<"Population initialised"<<std::endl;
   PrintMembersOfPopulation(population);

   // Manually enter variable ranges below here
   double variableLower[2]={-3,-3};
   double variableUpper[2]={3,3};
   // Assign all the individual variable ranges in a vector 'rangeAllVariables'
   std::vector<std::vector<double>> rangeAllVariables;
   for(int i=0;i<numberOfVariables;i++) {
   	std::vector<double> variableRange;
        variableRange.push_back(variableLower[i]);
        variableRange.push_back(variableUpper[i]);
	rangeAllVariables.push_back(variableRange);
   }
   /*std::cout<<"Variable ranges: "<<std::endl;
   PrintMembersOfPopulation(rangeAllVariables);*/

   // Create files for storing population of each generation & fitnesses of each individual in each generation
   std::fstream populationEachGeneration;
   std::fstream fitnessEachGeneration;   

   //Delete any existing instances of the files to be created
   remove("PopulationEachGeneration.txt");
   remove("FitnessEachGeneration.txt");
  
   //std::cout<<"GENERATION NUMBER : "<<std::endl;
   std::vector<double> bestFitnessOverGenerations;
   // Loop begins - generation evolution start
   for(int generationIndex=0;generationIndex<numberOfGenerations;generationIndex++) {
	//std::cout<<generationIndex+1<<std::endl;
   	//PrintMembersOfPopulation(population);
   	//PrintRescaledMembersOfPopulation(population,rangeAllVariables);
	//std::cout<<std::endl;

	// Store current population in file PopulationEachGeneration.txt
	StoreCurrentPopulationInFile(population, populationEachGeneration, rangeAllVariables);
   	
	// Declare variables required for fitness evaluation
   	Fitnesses fitness;	// Keep inside scope of each generation
   	double tempFitness;     // Keep inside scope of each generation
	
	// Evaluate the fitness of each member in the population and assign to 'fitness' vector
	fitness = EvaluatePopulationFitness(population, rangeAllVariables);
	StoreCurrentFitnessInPopulation(fitness, fitnessEachGeneration);

	// Print fitnesses of the current population
	/*std::cout<<"Fitnesses are: "<<std::endl;
	PrintIndividual(fitness);*/

	// Declare variables required for best individual search
  	long double maximumFitness;
   	int bestIndividualIndex;
	Individual bestIndividual;       
 
	// Find best individual in the current population
	bestIndividualIndex = GetBestIndividualIndex(fitness); //, population, rangeAllVariables); 
	bestIndividual = population[bestIndividualIndex];
	maximumFitness = fitness[bestIndividualIndex];
        bestFitnessOverGenerations.push_back(maximumFitness);

	// Check if the best individual is being correctly calculated
	/*std::cout<<"Best Individual is "<<bestIndividualIndex<<" with fitness "<<maximumFitness<<std::endl;
	std::cout<<"The best individual is ";
	PrintIndividual(population[bestIndividualIndex]);
	std::cout<<std::endl;*/
   	
   	// Based on fitness, perform selection & crossover without replacement
   	Population tempPopulation;
	tempPopulation = population;
   	/*std::cout<<"Members of tempPopulation are: "<<std::endl;
   	PrintMembersOfPopulation(tempPopulation);
	std::cout<<std::endl;*/
	
	//std::cout<<"BEGINNING TOURNAMENT SELECTION AND CROSSOVER"<<std::endl<<std::endl;
   	for(int i=0;i<populationSize;i=i+2) { // i=i+2 since 2 individuals are crossed over
	   // Select two random individuals using tournament selection
	   std::vector<Individual> individualsToCrossover;  // Both individuals to be crossed over placed in a vector
	   for(int j=0;j<tournamentSize;j++) {
	       int temporaryIndex=TournamentSelection(fitness, tournamentSelectionParameter, tournamentSize);
	       individualsToCrossover.push_back(population[temporaryIndex]);
	   } // end of Tournament Selection
	
	   // Perform crossover
	   double randomDouble = GetRandomDouble();
	   if(randomDouble < crossoverProbability) {
	       // Send pair of chromosomes to be crossed-over to function. Get crossed-over pair
	       /*std::cout<<"Individuals will be crossed over"<<std::endl;
	       std::cout<<"Individuals before crossover: "<<std::endl;
	       PrintMembersOfPopulation(individualsToCrossover);*/	
	       std::vector<Individual> crossedOverIndividuals = CrossOver(individualsToCrossover);
	       /*std::cout<<"Individuals after crossover: "<<std::endl;
	       PrintMembersOfPopulation(crossedOverIndividuals);*/
	       // Place crossed over individuals in corresponding locations in tempPopulation
	       tempPopulation[i]=crossedOverIndividuals[0];
	       tempPopulation[i+1]=crossedOverIndividuals[1];
	       /*std::cout<<"The new tempPopulation is now: "<<std::endl;
	       PrintMembersOfPopulation(tempPopulation);
	       std::cout<<std::endl;*/
           } 
	   else {
	       // Place original pair of individuals in corresponding (consecutive) locations in tempPopulation
	       //std::cout<<"Individuals will not be crossed over"<<std::endl;
	       tempPopulation[i]=individualsToCrossover[0];
	       tempPopulation[i+1]=individualsToCrossover[1];
	       /*std::cout<<"The new tempPopulation is now unchanged: "<<std::endl;
	       PrintMembersOfPopulation(tempPopulation);
	       std::cout<<std::endl;*/ 
	   }  // end of Crossover
   	} // End of Selection and Crossover
	//std::cout<<"END OF TOURNAMENT SELECTION AND CROSSOVER"<<std::endl<<std::endl;  // End of Selection and Crossover

   	// Mutate each individual
	//std::cout<<"BEGINNING MUTATION"<<std::endl<<std::endl;
   	MutatePopulation(tempPopulation, mutationProbability, creepRateMutation); // Population mutated
	/*std::cout<<"tempPopulation after mutation is now: "<<std::endl;
	PrintMembersOfPopulation(tempPopulation);
	std::cout<<std::endl<<"END OF MUTATION"<<std::endl;*/

   	// Elitism
   	tempPopulation[0]=population[bestIndividualIndex];
	/*std::cout<<std::endl<<"tempPopulation after implementing elitism is now: "<<std::endl;
	PrintMembersOfPopulation(tempPopulation);
	std::cout<<std::endl<<"ELITISM IMPLEMENTED."<<std::endl<<std::endl;*/

   	// Replace population with new population (tempPopulation)
	/*std::cout<<"So, to remind you, the earlier population was: "<<std::endl;
	PrintMembersOfPopulation(population);
	std::cout<<std::endl<<"CREATING NEW GENERATION BY REPLACING POPULATION WITH NEW POPULATION NOW"<<std::endl<<std::endl;*/
   	for(int i=0;i<tempPopulation.size();i++)
	    population[i]=tempPopulation[i];
	/*std::cout<<"REPLACEMENT COMPLETE. NEW GENERATION CREATED."<<std::endl<<std::endl;
	std::cout<<"Have a look at the new population now: "<<std::endl;
   	PrintMembersOfPopulation(population);
	std::cout<<std::endl;
	std::cout<<"The 'rescaled' population looks like this: "<<std::endl;
	PrintRescaledMembersOfPopulation(population,rangeAllVariables);
   	std::cout<<std::endl;*/
   } // end of generation loop

   /*std::cout<<std::endl;
   std::cout<<"Fitness improvement over generations:"<<std::endl;
   PrintIndividual(bestFitnessOverGenerations);
   std::cout<<std::endl;*/
   /*std::cout<<"The final 'rescaled' population looks like this: "<<std::endl;
   PrintRescaledMembersOfPopulation(population,rangeAllVariables);
   std::cout<<std::endl;*/

   // Declare variables required for final fitness evaluation
   Fitnesses finalFitness;      // Keep inside scope of each generation
   double tempFitness;     // Keep inside scope of each generation

   // Evaluate the fitness of each member in the population and assign to 'fitness' vector
   finalFitness = EvaluatePopulationFitness(population, rangeAllVariables);

   // Declare variables required for best individual search
   long double finalMaximumFitness;
   int finalBestIndividualIndex;
   Individual finalBestIndividual;

   // Find best individual in the current population
   finalBestIndividualIndex = GetBestIndividualIndex(finalFitness); //, population, rangeAllVariables); 
   finalBestIndividual = population[finalBestIndividualIndex];
   finalMaximumFitness = finalFitness[finalBestIndividualIndex];

   // Check if the best individual is being correctly calculated
   std::cout<<"The best Individual is ";
   PrintRescaledIndividual(population[finalBestIndividualIndex], rangeAllVariables);
   std::cout<<"The optimal function value is "<<-finalMaximumFitness<<std::endl;
   std::cout<<std::endl;

   // Storing output values into file
   /* Outputs required to be sent to MATLAB for plotting:
        1. Number of generations
        2. rangeAllVariables
        3. All individuals (x,y) in current generation
        4. All fitnesses in current generation
        5. Best fitness in each generation
        6. Best individual (x,y) for each generation
        7. Population size, maybe (this can be handled within MATLAB as size(fitness, 1). Think.

      No inputs are required as of now. In future, when a GUI has been made, specific GA parameters, functions etc. can be inputs.
   */

   std::ofstream fileOperation;
   remove("fitness_vs_generation.txt");
   fileOperation.open("fitness_vs_generation.txt");
   if (fileOperation.is_open()) {
	for(int i=0;i<bestFitnessOverGenerations.size();i++) {
	    fileOperation << bestFitnessOverGenerations[i] << '\n';
	}
        fileOperation.close();
   }
}// end of main()
