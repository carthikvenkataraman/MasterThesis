#include "Transmission.h"

typedef std::vector<int> Genes;

Transmission::Transmission(int currentUnitIndex) {
  std::cout<<"Transmission object creation begun"<<std::endl;
  transmissionTypeIndex = (currentUnitIndex==0)? 0 : 1;
  transmissionName = (transmissionTypeIndex==0)? "MechanicalGearbox" : "MotorTransmission";
  GetTransmissionFileName();
  //LoadTransmission();
  //PrintTransmissionData();
  //std::cout<<"Transmission object created"<<std::endl;
}


void Transmission::GetTransmissionFileName() {
  std::string fileLocation = "data/"
  std::string fileTypeString = ".mat";
  std::string transmissionFileNameString = fileLocation+transmissionName+fileTypeString;
  transmissionFileName = new char[transmissionFileNameString.size()+1];
  memcpy(transmissionFileName,transmissionFileNameString.c_str(),transmissionFileNameString.size());
}

std::string Transmission::ReturnTransmissionName() {
   return transmissionName;
}

void Transmission::LoadTransmission() {
  MATFile *matFileHandle;
  matFileHandle = matOpen(transmissionFileName, "r");
  int numberOfDirectories;
  const char** directoryField= (const char** )matGetDir(matFileHandle, &numberOfDirectories);
  for(int i=0; i < numberOfDirectories; i++) {
    std::cout<<directoryField[i]<<std::endl;
    mxArray* matArrayHandle = matGetVariable(matFileHandle, directoryField[i]);
    int numberOfRows = mxGetM(matArrayHandle);
    int numberOfColumns = mxGetN(matArrayHandle);
    double* matArrayAddress = mxGetPr(matArrayHandle);
    std::vector<double> transmissionDataEntry;
    for(int j=0;j<numberOfColumns;j++) {
      double matrixEntry = *(matArrayAddress+j);
      transmissionDataEntry.push_back(matrixEntry);
    }
    transmissionData.push_back(transmissionDataEntry);
  }
  std::cout<<"Transmission data successfully loaded"<<std::endl<<std::endl;
}

void Transmission::PrintTransmissionData(std::vector<std::vector<double>>& transmissionData) {
  for(int i=0;i<transmissionData.size();i++) {
	 std::vector<double> transmissionDataRow = transmissionData[i];
	 for(int j=0;j<transmissionDataRow.size();j++)
	   std::cout<<transmissionDataRow[j]<<"	";
	 std::cout<<std::endl;
  }
}

Transmission::~Transmission() {
}