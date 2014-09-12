#include <iostream>
#include <random>
#include <vector>
#include <utility>
#include <memory>
#include <math.h>
#include <fstream>
#include "/home/karthik/MATLAB/R2014a/extern/include/mat.h"
#include "Combination.h"

typedef std::vector<int> Genes;             // Motor, buffer, or propelled axle set - one of these.
typedef std::vector<Genes> UnitGene;	    // Each unit gene is made of sets of gene-sets (engine, motor, etc.) 	
typedef std::vector<UnitGene> Individual;   // Each individual is made of unit genes for each unit (tractor, ST1, Dolly, ST2...) 
typedef std::vector<Individual> Population; // Many individuals make up a population
typedef std::vector<double> Fitnesses;      // Vector containing the fitness of each individual in the population

double GetRandomDouble() {
 std::random_device rdev{};
 std::default_random_engine engineRandom{rdev()};
 std::uniform_real_distribution<double> distribution(0.0,1.0);
 double randomDouble =  distribution(engineRandom);
 return randomDouble;
}

int GetRandomIndex(int vectorSize) {
 std::random_device rdev{};
 std::default_random_engine engineRandom{rdev()};
 std::uniform_int_distribution<int> distribution(0,vectorSize-1); // Important - 0 to N-1 and not 1 to N!!
 int randomIndex = distribution(engineRandom);
 return randomIndex;
}

Genes CreateMachineGenes(int numberOfMachines) {
 Genes machineGenes;
 for(int i=0;i<numberOfMachines;i++) 
   machineGenes.push_back(0);
 int machineIndexRandomlyGenerated = GetRandomIndex(numberOfMachines);
 machineGenes[machineIndexRandomlyGenerated] = 1;
 return machineGenes;
}

Genes CreatePropelledAxleGenes(int numberOfPropelledAxlesMax) {
 Genes propelledAxleGenes;
 for(int i=0;i<numberOfPropelledAxlesMax;i++) 
   propelledAxleGenes.push_back(0);
 int numberOfAxlesPropelledRandomlyGenerated = GetRandomIndex(numberOfPropelledAxlesMax);
 for(int i=0;i<numberOfAxlesPropelledRandomlyGenerated;i++) {
   int propelledAxleIndexRandomlyGenerated = GetRandomIndex(numberOfPropelledAxlesMax);
   propelledAxleGenes[propelledAxleIndexRandomlyGenerated] = 1;
 }
 return propelledAxleGenes;
}

UnitGene CreateTractorUnitGene(int numberOfEngines, int numberOfAxlesInUnit) {
 Genes engineGenes = CreateMachineGenes(numberOfEngines);
 Genes bufferGenes{0,0,0}; // Dummy gene set added to make decoding simpler
 Genes propelledAxleGenes{0,1,1}; // FAA10, RAA22 - IMPORTANT - THIS MUST BE MANUALLY ALTERED IF FAA/RAA CHANGES
 UnitGene tractorUnitGene{engineGenes, bufferGenes, propelledAxleGenes};
 return tractorUnitGene;
}

UnitGene CreateTrailingUnitGene(int numberOfAxlesInUnit, int numberOfElectricMotors, int numberOfElectricBuffers) {
 Genes motorGenes = CreateMachineGenes(numberOfElectricMotors);
 Genes bufferGenes = CreateMachineGenes(numberOfElectricBuffers);
 Genes propelledAxleGenes = CreatePropelledAxleGenes(numberOfAxlesInUnit);
 UnitGene trailingUnitGene{motorGenes, bufferGenes, propelledAxleGenes};
 return trailingUnitGene;
}

Individual GetIndividual(std::vector<std::vector<int>>& vehicleConfigurationParameters) {
 std::vector<int> machineParameters = vehicleConfigurationParameters[0];  // machineParameters contains number of engines, motors & buffers
 std::vector<int> numberOfAxlesOnEachUnit = vehicleConfigurationParameters[1];
 UnitGene tractorUnitGene = CreateTractorUnitGene(machineParameters[0], numberOfAxlesOnEachUnit[0]); // No. of engines & axles data
 Individual individual{tractorUnitGene};  // Store tractor unit gene in the individual chromosome
 for(int i=1;i<numberOfAxlesOnEachUnit.size();i++) {
  UnitGene trailingUnitGene = CreateTrailingUnitGene(numberOfAxlesOnEachUnit[i], machineParameters[1], machineParameters[2]);
	individual.push_back(trailingUnitGene); // Store each trailing unit gene in the individual chromosome
 }
 return individual;
}

Population InitialisePopulation(int populationSize, std::vector<std::vector<int>>& vehicleConfigurationParameters) {
 Population population;
 for(int i=0;i<populationSize;i++) {
	Individual tempInd = GetIndividual(vehicleConfigurationParameters);	// Individual = std::vector<float> - vector of floats
  population.push_back(tempInd);
 }
 return population;
}

void PrintGeneSet(Genes& geneSet) {
  int sizeOfGeneSet = geneSet.size();
  for(int i=0;i<sizeOfGeneSet;i++) 
    std::cout<<geneSet[i];
}

void PrintUnitGene(UnitGene& unitGene) {
  int numberOfGeneSets = unitGene.size(); // Normally 2 for tractor unit & 3 for trailing unit
  for(int j=0;j<numberOfGeneSets;j++) {
    Genes geneSet = unitGene[j];
    PrintGeneSet(geneSet);
    std::cout<<"\\";
  }
}

void PrintIndividual(Individual& individual) {
  int numberOfUnits = individual.size();
  for(int i=0;i<numberOfUnits;i++) {
    UnitGene unitGene = individual[i];      // Tractor or Trailing Unit Genes 
    PrintUnitGene(unitGene);
    std::cout<<"  ";
  }
  std::cout<<std::endl;
}

void PrintMembersOfPopulation(Population& population) {
  int populationSize = population.size();
  for(int i=0;i<populationSize;i++) {
    std::cout<<"Individual "<<i<<": ";
    Individual individualToPrint = population[i];
    PrintIndividual(individualToPrint);
  }
}

void PrintFitness(Fitnesses& fitness) {
  int populationSize = fitness.size();
  for(int i=0;i<populationSize;i++)
    std::cout<<"Individual "<<i<<": "<<fitness[i]<<std::endl;
}

double EvaluateIndividualFitness(Individual individual, std::vector<std::vector<double>>& missionData) {
 double fitness=0;
 /*int numberOfUnits = individual.size();
 for(int i=0;i<numberOfUnits;i++) {
	UnitGene unitGenes = individual[i];      // Tractor or Trailing Unit Genes 
  int numberOfGeneSets = unitGenes.size(); // Normally 2 for tractor unit & 3 for trailing unit
  for(int j=0;j<numberOfGeneSets;j++) {
    Genes genesSingleSet = unitGenes[j];
    int numberOfGenes = genesSingleSet.size();
    for(int k=0;k<numberOfGenes;k++)
     fitness+=genesSingleSet[k];
    }
  }//*/
  PrintIndividual(individual);
  std::shared_ptr<Combination> newCombination(new Combination(individual, missionData));
  newCombination->RunMission();
  fitness = newCombination->missionProductivity;
  return fitness;
}

Fitnesses EvaluatePopulationFitness(Population& population, std::vector<std::vector<double>>& missionData) {
  int populationSize = population.size();
  Fitnesses fitness;
  for(int i=0;i<populationSize;i++) {
    Individual tempInd = population[i];
    double tempFitness = EvaluateIndividualFitness(tempInd, missionData);
    fitness.push_back(tempFitness);
  }
  return fitness;
}

int GetBestIndividualIndex(Fitnesses& fitness) {
  int populationSize = fitness.size();
  double maximumFitness = fitness[0];
  int bestIndividualIndex = 0;
  for(int i=1;i<populationSize;i++) {
    if(fitness[i]>maximumFitness) {
      bestIndividualIndex = i;
      maximumFitness = fitness[i];
    }
  }
  return bestIndividualIndex;
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
  std::vector<double> randomIndividualFitness;
  int populationSize = fitnessForSelection.size();

  // Select two individuals to participate in the tournament
  for(int i=0;i<tournamentSize;i++) {
    // Contains the indices of two individuals participating in the tournament
    int temporaryIndex = GetRandomIndex(populationSize);
    randomIndividualIndex.push_back(temporaryIndex); // Index needed to pass to main()
    double temporaryFitness = fitnessForSelection[temporaryIndex];
    randomIndividualFitness.push_back(temporaryFitness);
  }

  // Generate random floating point number (double) for tournament selection
  double randomDouble = GetRandomDouble();
  int fittestIndividualIndex, weakestIndividualIndex;

  // Choose fitter individual if random double is lesser than tournament selection parameter
  if(randomDouble < tournamentSelectionParameter) {
    // Return fittest individual 
    fittestIndividualIndex = GetFittestIndividual(randomIndividualIndex, randomIndividualFitness);
    return fittestIndividualIndex;
  }
  else {
    // Return weakest individual
    weakestIndividualIndex = GetWeakestIndividual(randomIndividualIndex, randomIndividualFitness);
    return weakestIndividualIndex;
  }
}

UnitGene CrossOverMachineGeneSet(UnitGene& geneSetsToBeCrossedOver){
  //std::cout<<"Inside Machine Cross Over"<<std::endl;
  Genes crossedOverGeneSet1, crossedOverGeneSet2;
  crossedOverGeneSet1 = geneSetsToBeCrossedOver[1];
  crossedOverGeneSet2 = geneSetsToBeCrossedOver[0];
  UnitGene crossedOverGeneSets{crossedOverGeneSet1, crossedOverGeneSet2};
  return crossedOverGeneSets;
}

UnitGene CrossOverAxleGeneSet(UnitGene& geneSetsToBeCrossedOver){
  //std::cout<<"Inside Axle Cross Over"<<std::endl;
  Genes genesToBeCrossedOver1 = geneSetsToBeCrossedOver[0];
  Genes genesToBeCrossedOver2 = geneSetsToBeCrossedOver[1];
  Genes crossedOverGeneSet1, crossedOverGeneSet2;
  int numberOfGenes = genesToBeCrossedOver1.size();
  int crossOverPoint = GetRandomIndex(numberOfGenes);
  //std::cout<<"Crossover Point: "<<crossOverPoint<<std::endl;
  for(int i=0;i<numberOfGenes;i++) {
    if(i<crossOverPoint) {
      crossedOverGeneSet1.push_back(genesToBeCrossedOver1[i]);
      crossedOverGeneSet2.push_back(genesToBeCrossedOver2[i]);
    }
    else {
      crossedOverGeneSet1.push_back(genesToBeCrossedOver2[i]);
      crossedOverGeneSet2.push_back(genesToBeCrossedOver1[i]);
    }
  }
  UnitGene crossedOverGeneSets{crossedOverGeneSet1, crossedOverGeneSet2};
  return crossedOverGeneSets;
}

Population CrossOver(Population& individualsToCrossover, double crossOverProbability) {
  Individual individual1 = individualsToCrossover[0];
  Individual individual2 = individualsToCrossover[1];
  Individual crossedOverIndividual1, crossedOverIndividual2;

  int crossOverUnitIndex = GetRandomIndex(individual1.size()); // In which Unit does crossover occur?
  UnitGene crossOverUnitGene1 = individual1[crossOverUnitIndex];
  UnitGene crossOverUnitGene2 = individual2[crossOverUnitIndex];
  int crossOverGeneSetIndex = GetRandomIndex(crossOverUnitGene1.size()); // Till what gene set in the chosen Unit is the individual preserved?

  for(int i=0;i<individual1.size();i++) {
    if(i<crossOverUnitIndex) {
    crossedOverIndividual1.push_back(individual1[i]);
    crossedOverIndividual2.push_back(individual2[i]);
    }
    else if(i==crossOverUnitIndex) {
      UnitGene tempUnitGene1, tempUnitGene2;
      for(int j=0;j<crossOverUnitGene1.size();j++) {
        if(j<crossOverGeneSetIndex) {
          tempUnitGene1.push_back(crossOverUnitGene1[j]);
          tempUnitGene2.push_back(crossOverUnitGene2[j]);
        }
        else {
          tempUnitGene2.push_back(crossOverUnitGene1[j]);
          tempUnitGene1.push_back(crossOverUnitGene2[j]);
        }
      }
      crossedOverIndividual1.push_back(tempUnitGene1);
      crossedOverIndividual2.push_back(tempUnitGene2);
    }
    else {
      crossedOverIndividual1.push_back(individual2[i]);
      crossedOverIndividual2.push_back(individual1[i]);
    }
  }
  Population crossedOverIndividuals{crossedOverIndividual1, crossedOverIndividual2};
  return crossedOverIndividuals;
}

/*Population CrossOver(Population& individualsToCrossover, double crossOverProbability) {
  //std::cout<<"Main CrossOver Function."<<std::endl;
  Individual individual1 = individualsToCrossover[0];
  Individual individual2 = individualsToCrossover[1];
  Individual crossedOverIndividual1, crossedOverIndividual2;

  for(int i=0;i<individual1.size();i++) {  // Choose unit whose information is to be crossed over
    UnitGene unitGene1 = individual1[i];   
    UnitGene unitGene2 = individual2[i];
    UnitGene crossedOverUnitGene1, crossedOverUnitGene2;
    for(int j=0;j<unitGene1.size();j++) { // Choose gene set within unit
      UnitGene geneSetsToBeCrossedOver{unitGene1[j], unitGene2[j]};
      if(j<unitGene1.size()-1) { // Last gene set contains axles. Previous units refer to machines. 
        if(GetRandomDouble() < crossOverProbability) {  
          UnitGene crossedOverGeneSets = CrossOverMachineGeneSet(geneSetsToBeCrossedOver);
          crossedOverUnitGene1.push_back(crossedOverGeneSets[0]);
          crossedOverUnitGene2.push_back(crossedOverGeneSets[1]);
        }
        else {
          //std::cout<<"No Machine Crossover"<<std::endl;
          crossedOverUnitGene1.push_back(geneSetsToBeCrossedOver[0]);
          crossedOverUnitGene2.push_back(geneSetsToBeCrossedOver[1]);
        }
      }
      else {
        if(GetRandomDouble() < crossOverProbability) {  
          UnitGene crossedOverGeneSets = CrossOverAxleGeneSet(geneSetsToBeCrossedOver);
          crossedOverUnitGene1.push_back(crossedOverGeneSets[0]);
          crossedOverUnitGene2.push_back(crossedOverGeneSets[1]);
        }
        else {
          //std::cout<<"No Axle Crossover"<<std::endl;
          crossedOverUnitGene1.push_back(geneSetsToBeCrossedOver[0]);
          crossedOverUnitGene2.push_back(geneSetsToBeCrossedOver[1]);
        }
      }
    } // All Gene sets in given Unit have been considered for crossover
    crossedOverIndividual1.push_back(crossedOverUnitGene1);
    crossedOverIndividual2.push_back(crossedOverUnitGene2);
  } // All Units in both individuals have been considered for crossover
  std::vector<Individual> crossedOverIndividuals{crossedOverIndividual1, crossedOverIndividual2};
  return crossedOverIndividuals;
}//*/

void MutateMachineGeneSet(Genes& geneSetForMutation) {
  //std::cout<<"Inside machine mutation"<<std::endl;
  int numberOfGenes = geneSetForMutation.size();
  int randomIndex = GetRandomIndex(numberOfGenes);
  for(int k=0;k<numberOfGenes;k++)
    geneSetForMutation[k]=0;
  geneSetForMutation[randomIndex]=1;
}

void MutateAxleGeneSet(Genes& geneSetForMutation, double mutationProbability) {
  int numberOfGenes = geneSetForMutation.size();
  for(int i=0;i<numberOfGenes;i++) {
    if(GetRandomDouble()<mutationProbability){
      //std::cout<<"Inside axle mutation"<<std::endl;
      geneSetForMutation[i]=1-geneSetForMutation[i];
    }
  }
}

void MutateIndividual(Individual& individualToBeMutated, double mutationProbability) {
  int numberOfUnits = individualToBeMutated.size();
  for(int i=0;i<numberOfUnits;i++) {
    UnitGene tempUnit = individualToBeMutated[i]; // Choose one unit
    int numberOfGeneSets = tempUnit.size();
    for(int j=0;j<numberOfGeneSets;j++) {
      Genes geneSetForMutation = tempUnit[j];  // Choose one gene set
      if(j<numberOfGeneSets-1) { // MUTATION FOR WHOLE GENE SETS - MACHINE GENE SET MUTATION
        if(i!=0 && j!=1) { // To avoid mutation for the dummy buffer gene set
          if(GetRandomDouble()<mutationProbability) {
            MutateMachineGeneSet(geneSetForMutation);
            tempUnit[j] = geneSetForMutation; // Current Unit is replaced with new mutated machine gene set
          } // Mutation complete. If no mutation, current unit maintains old machine gene set
        }
      }
      else { // NORMAL MUTATION FOR INDIVIDUAL GENES - AXLE GENE MUTATION
        if(i!=0) { // To avoid mutation for the tractor axles gene set
          MutateAxleGeneSet(geneSetForMutation, mutationProbability);
          tempUnit[j] = geneSetForMutation; // Current Unit is replaced with new mutated axle gene set
        }
      } // Mutation complete. If no mutation, current unit maintains old axle gene set
    } // All gene sets in current unit have undergone mutation
    individualToBeMutated[i] = tempUnit; // Mutated unit is now placed in the individual
  } // All units in current individual have undergone mutation
}

void MutatePopulation(Population& tempPopulation, double mutationProbability) {
  int populationSize = tempPopulation.size();
  for(int i=0;i<populationSize;i++) {
    MutateIndividual(tempPopulation[i], mutationProbability); // Individual replaced with mutated one   
  }
}

void StoreCurrentPopulationInFile(Population& population, std::fstream& populationEachGeneration, std::vector<std::vector<double>>& rangeAllVariables) {
  populationEachGeneration.open("PopulationEachGeneration.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  int populationSize = population.size();
  for(int i=0;i<populationSize;i++) {
    Individual tempIndividual = population[i]; // Choose one individual
    int numberOfUnits = tempIndividual.size();
    for(int i=0;i<numberOfUnits;i++) {
      UnitGene tempUnit = tempIndividual[i]; // Choose one unit
      int numberOfGeneSets = tempUnit.size();
      for(int j=0;j<numberOfGeneSets;j++) {
        Genes tempGeneSet = tempUnit[j];
        int numberOfGenes = tempGeneSet.size(); // Choose one gene set
        for(int k=0;k<numberOfGenes;k++) {
          populationEachGeneration << tempGeneSet[k];
        } // All genes in current gene set have been stored
      } // All gene sets in current unit have been stored
    } // All units in current individual have been stored
    populationEachGeneration << '\n';
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

void LoadMission(std::vector<std::vector<double>>& missionData) {
  MATFile *matFileHandle;
  matFileHandle = matOpen("data/MissionData70.mat", "r");
  int numberOfDirectories;
  const char** directoryField= (const char** )matGetDir(matFileHandle, &numberOfDirectories);
  //std::cout<<"Number of directories: "<<numberOfDirectories<<std::endl;
  for(int i=0; i < numberOfDirectories; i++) {
    //std::cout<<directoryField[i]<<std::endl;
    mxArray* matArrayHandle = matGetVariable(matFileHandle, directoryField[i]);
    int numberOfRows = mxGetM(matArrayHandle);
    int numberOfColumns = mxGetN(matArrayHandle);
    double* matArrayAddress = mxGetPr(matArrayHandle);
    std::vector<double> missionDataEntry;
    for(int j=0;j<numberOfColumns;j++) {
      double matrixEntry = *(matArrayAddress+j);
      missionDataEntry.push_back(matrixEntry);
    }
    missionData.push_back(missionDataEntry);
  }
  std::cout<<"Mission data successfully loaded"<<std::endl<<std::endl;
  if (matClose(matFileHandle) != 0) {
    std::cout<<"Error closing file"<<'\n';
  }
}

int main() {
  // Declare GA parameters - pop. size, variables, etc
  const int populationSize = 30;
  const int numberOfGenerations = 100;
  const int tournamentSize = 2;
  const double tournamentSelectionParameter = 0.75;
  const float crossOverProbability = 0.7;
  const float creepRateMutation = 0.1;
  const double mutationProbability = 0.05;

  // Vehicle related parameters for chromosome construction
  int numberOfUnits = 4; 			// Including Tractor
  std::vector<int> numberOfAxlesOnEachUnit{3,3,2,3};   // Can be made inputs later
  int numberOfEngines = 3;			// D11, D13, D16
  int numberOfElectricMotors = 3;
  int numberOfElectricBuffers = 3;

  std::vector<int> machineParameters{numberOfEngines, numberOfElectricMotors, numberOfElectricBuffers};
  std::vector<std::vector<int>> vehicleConfigurationParameters{machineParameters, numberOfAxlesOnEachUnit};

  // Initialise the population
  Population population = InitialisePopulation(populationSize, vehicleConfigurationParameters);
  // Check if population is correctly initialised
  std::cout<<"Population initialised"<<std::endl;
  PrintMembersOfPopulation(population);

  // Create files for storing population of each generation & fitnesses of each individual in each generation
  std::fstream populationEachGeneration;
  std::fstream fitnessEachGeneration;   

  //Delete any existing instances of the files to be created
  remove("PopulationEachGeneration.txt");
  remove("FitnessEachGeneration.txt");

  std::vector<double> bestFitnessOverGenerations;

  std::vector<std::vector<double>> missionData;
  LoadMission(missionData);

  // Loop begins - generation evolution start
  for(int generationIndex=0;generationIndex<numberOfGenerations;generationIndex++) {
    std::cout<<"GENERATION NUMBER : "<<generationIndex<<std::endl;
    // Store current population in file PopulationEachGeneration.txt
    //StoreCurrentPopulationInFile(population, populationEachGeneration, rangeAllVariables);

    // Declare variables required for fitness evaluation
    Fitnesses fitness;	// Keep inside scope of each generation
    double tempFitness;     // Keep inside scope of each generation

    // Evaluate the fitness of each member in the population and assign to 'fitness' vector
    fitness = EvaluatePopulationFitness(population, missionData);
    StoreCurrentFitnessInPopulation(fitness, fitnessEachGeneration);

    // Declare variables required for best individual search
    long double maximumFitness;
    int bestIndividualIndex;
    Individual bestIndividual;       

    // Find best individual in the current population
    bestIndividualIndex = GetBestIndividualIndex(fitness); //, population, rangeAllVariables); 
    bestIndividual = population[bestIndividualIndex];
    maximumFitness = fitness[bestIndividualIndex];
    bestFitnessOverGenerations.push_back(maximumFitness);

    //PrintFitness(fitness);
    //std::cout<<"The best Individual is "<<bestIndividualIndex<<std::endl;
    std::cout<<"The optimal function value is "<<maximumFitness<<std::endl;

    // Based on fitness, perform selection & crossover without replacement
    Population tempPopulation;
    tempPopulation = population;

    for(int i=0;i<populationSize;i=i+2) { // i=i+2 since 2 individuals are crossed over
      // Select two random individuals using tournament selection
      std::vector<Individual> individualsToCrossover;  // Both individuals to be crossed over placed in a vector
      for(int j=0;j<tournamentSize;j++) {
        int temporaryIndex=TournamentSelection(fitness, tournamentSelectionParameter, tournamentSize);
        //std::cout<<"Chosen Individual :"<<temporaryIndex<<std::endl;
        individualsToCrossover.push_back(population[temporaryIndex]);
      } // end of Tournament Selection

      // Perform crossover
      // ALGORITHM ONE - INDIVIDUAL UNIT GENE CONSIDERATION FOR CROSSOVER
      std::vector<Individual> crossedOverIndividuals = CrossOver(individualsToCrossover, crossOverProbability);
      tempPopulation[i]=crossedOverIndividuals[0];
      tempPopulation[i+1]=crossedOverIndividuals[1];
	   
      // ALGORITHM TWO - CROSSOVER AT UNIT GENE BOUNDARIES FOR WHOLE INDIVIDUALS
      /*double randomDouble = GetRandomDouble();
      if(randomDouble < crossOverProbability) {
        //std::cout<<"CrossOver Initiated"<<std::endl;
        std::vector<Individual> crossedOverIndividuals = CrossOver(individualsToCrossover);
        tempPopulation[i]=crossedOverIndividuals[0];
        tempPopulation[i+1]=crossedOverIndividuals[1];
      } 
      else {
        // Place original pair of individuals in corresponding (consecutive) locations in tempPopulation
        //std::cout<<"No Crossover"<<std::endl;	
        tempPopulation[i]=individualsToCrossover[0];
        tempPopulation[i+1]=individualsToCrossover[1];
      }  // end of Crossover*/
    } // End of Selection and Crossover

    //std::cout<<std::endl<<"After Crossover"<<std::endl;
    //PrintMembersOfPopulation(tempPopulation);

    // Mutate each individual
    MutatePopulation(tempPopulation, mutationProbability); // Population mutated

    //std::cout<<std::endl<<"After Mutation"<<std::endl;
    //PrintMembersOfPopulation(tempPopulation);

    // Elitism
    tempPopulation[0]=population[bestIndividualIndex];

    for(int i=0;i<tempPopulation.size();i++)
      population[i]=tempPopulation[i];
  } // end of generation loop


  // Declare variables required for final fitness evaluation
  Fitnesses finalFitness;      // Keep inside scope of each generation
  double tempFitness;     // Keep inside scope of each generation

  // Evaluate the fitness of each member in the population and assign to 'fitness' vector
  finalFitness = EvaluatePopulationFitness(population, missionData);
  //PrintFitness(finalFitness);

  // Declare variables required for best individual search
  long double finalMaximumFitness;
  int finalBestIndividualIndex;
  Individual finalBestIndividual;

  // Find best individual in the current population
  finalBestIndividualIndex = GetBestIndividualIndex(finalFitness); //, population, rangeAllVariables); 
  finalBestIndividual = population[finalBestIndividualIndex];
  finalMaximumFitness = finalFitness[finalBestIndividualIndex];

  PrintMembersOfPopulation(population);
  // Check if the best individual is being correctly calculated
  std::cout<<"The best Individual is "<<finalBestIndividualIndex<<std::endl;
  PrintIndividual(population[finalBestIndividualIndex]);
  std::cout<<"The optimal function value is "<<finalMaximumFitness<<std::endl;
  //std::cout<<std::endl;

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