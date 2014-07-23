#include "Combination.h"

typedef std::vector<int> Genes;
typedef std::vector<Genes> UnitGene;
typedef std::vector<UnitGene> Individual;

int main() {
  Genes machineGenes0{0,0,1};
  Genes bufferGenes0{0,0,1};
  Genes axleGenes0{0,1,1};
  UnitGene unitGenes0{machineGenes0, bufferGenes0, axleGenes0};

  Genes machineGenes1{0,1,0};
  Genes bufferGenes1{0,0,1};
  Genes axleGenes1{1,1,1};
  UnitGene unitGenes1{machineGenes1, bufferGenes1, axleGenes1};

  Genes machineGenes2{0,1,0};
  Genes bufferGenes2{0,0,1};
  Genes axleGenes2{1,1};
  UnitGene unitGenes2{machineGenes2, bufferGenes2, axleGenes2};

  Genes machineGenes3{0,1,0};
  Genes bufferGenes3{0,0,1};
  Genes axleGenes3{1,1,1};
  UnitGene unitGenes3{machineGenes3, bufferGenes3, axleGenes3};

  Individual individual{unitGenes0, unitGenes1, unitGenes2, unitGenes3};

  std::vector<std::vector<double>> missionData;
  void LoadMission(std::vector<std::vector<double>>& );
  LoadMission(missionData);

  std::shared_ptr<Combination> newCombination(new Combination(individual, missionData));
  newCombination->RunMission();
  return 0;
}

void LoadMission(std::vector<std::vector<double>>& missionData) {
  MATFile *matFileHandle;
  matFileHandle = matOpen("data/MissionData.mat", "r");
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
}