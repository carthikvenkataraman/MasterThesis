#include "Transmission.h"

typedef std::vector<int> Genes;

Transmission::Transmission(int currentUnitIndex) {
  //std::cout<<"Transmission object creation begun"<<std::endl;
  transmissionTypeIndex = (currentUnitIndex==0)? 0 : 1;
  transmissionName = (transmissionTypeIndex==0)? "Gearbox" : "MotorTransmission";
  GetTransmissionFileName();
  LoadTransmission();
  //PrintTransmissionData(gearRatios);
  //std::cout<<"Transmission object created"<<std::endl;
}


void Transmission::GetTransmissionFileName() {
  std::string fileLocation = "data/";
  std::string fileTypeString = ".mat";
  std::string transmissionFileNameString = fileLocation+transmissionName+fileTypeString;
  //std::cout<<transmissionFileNameString<<std::endl;
  transmissionFileName = new char[transmissionFileNameString.size()+1];
  std::copy(transmissionFileNameString.begin(), transmissionFileNameString.end(), transmissionFileName);
  transmissionFileName[transmissionFileNameString.size()] = '\0';
  //memcpy(transmissionFileName,transmissionFileNameString.c_str(),transmissionFileNameString.size());
  //std::cout<<transmissionFileName<<std::endl;
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
    //std::cout<<directoryField[i]<<std::endl;
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

  gearRatios = transmissionData[0];
  gearEfficiency = transmissionData[1];
  gearInertias = transmissionData[2];

  //std::cout<<"Transmission data successfully loaded"<<std::endl;
  if (matClose(matFileHandle) != 0) {
    std::cout<<"Error closing file"<<'\n';
  }
}

void Transmission::PrintTransmissionData(std::vector<double>& transmissionDataEntry) {
	 for(int i=0;i<transmissionDataEntry.size();i++)
	   std::cout<<transmissionDataEntry[i]<<"	";
	 //std::cout<<std::endl;
}

double Transmission::GetOutputTorque(double transmissionInputTorque, int gearNumber) {
  transmissionOutputTorque = transmissionInputTorque*gearRatios[gearNumber];
  return transmissionOutputTorque;
}

Transmission::~Transmission() {
}