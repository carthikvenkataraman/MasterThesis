#include "Machine.h"

typedef std::vector<int> Genes;

Machine::Machine(int currentUnitIndex, Genes currentMachineGenes, std::shared_ptr<Buffer> bufferInUnit, int axleIndex) {
  //std::cout<<"Machine object creation begun"<<std::endl;
  machineTypeIndex = (currentUnitIndex==0)? 0 : 1;
  machineName = (machineTypeIndex==0)? "Engine0" : "Motor0";
  machineGenes = currentMachineGenes;
  bufferForMachine = bufferInUnit;
  machineAxleIndex = axleIndex;
  GetMachineIndex();
  GetMachineFileName();
  LoadMachine();
  //std::cout<<"Machine object created"<<std::endl;
  /*double testPower=4;
  double testEfficiency=5;
  bufferForMachine->RunBuffer(testPower, testEfficiency);*/

}

void Machine::GetMachineIndex() {
  std::vector<int>::iterator it = std::find(machineGenes.begin(), machineGenes.end(),1);
  machineIndex = std::distance(machineGenes.begin(), it);
}

void Machine::GetMachineFileName() {
  std::string fileLocation = "data/";
  std::string machineNameString = machineName;
  std::stringstream machineIndexStringStream;
  machineIndexStringStream << machineIndex;
  std::string machineIndexString = machineIndexStringStream.str();
  std::string fileTypeString = ".mat";
  completeMachineName = machineNameString+machineIndexString;
  std::string machineFileNameString = fileLocation+completeMachineName+fileTypeString;
  machineFileName = new char[machineFileNameString.size()+1];
  std::copy(machineFileNameString.begin(), machineFileNameString.end(), machineFileName);
  machineFileName[machineFileNameString.size()] = '\0';
  //memcpy(machineFileName,machineFileNameString.c_str(),machineFileNameString.size());
  //std::cout<<machineFileName<<std::endl;
}

std::string Machine::ReturnMachineName() {
   return completeMachineName;
}

void Machine::LoadMachine() {
  MATFile *matFileHandle;
  matFileHandle = matOpen(machineFileName, "r");
  int numberOfDirectories;
  const char** directoryField= (const char** )matGetDir(matFileHandle, &numberOfDirectories);
  for(int i=0; i < numberOfDirectories; i++) {
    //std::cout<<directoryField[i]<<std::endl;
    mxArray* matArrayHandle = matGetVariable(matFileHandle, directoryField[i]);
    int numberOfRows = mxGetM(matArrayHandle);
    int numberOfColumns = mxGetN(matArrayHandle);
    double* matArrayAddress = mxGetPr(matArrayHandle);
	  std::vector<std::vector<double>> machineData;
    for(int j=0;j<numberOfRows;j++) {
      std::vector<double> singleRowInArray;
      for(int k=0;k<numberOfColumns;k++) {
        double matrixEntry = *((matArrayAddress+j)+(k*numberOfRows));
        singleRowInArray.push_back(matrixEntry);
      }
      machineData.push_back(singleRowInArray);
    }
	  switch(i) {
     case 0:
      machineRPMRange = machineData[0];
      //std::cout<<"machineRPMRange"<<std::endl;
      //PrintMachineData(machineRPMRange);
      break;
     case 1:
      machineTorqueRange = machineData[0];
      //std::cout<<"machineTorqueRange"<<std::endl;
      //PrintMachineData(machineTorqueRange);
      break;
     case 2:
      machineEfficiencyData = machineData;
      //std::cout<<"machineEfficiencyData"<<std::endl;
      //PrintMachineData(machineEfficiencyData);
      break;
     case 3:
      rpmRangeForMaximumBrakeTorque = machineData[0];
      //std::cout<<"rpmRangeForMaximumBrakeTorque"<<std::endl;
      //PrintMachineData(rpmRangeForMaximumBrakeTorque);
      break;
     case 4:
      machineMaximumBrakeTorque = machineData[0]; 
      //std::cout<<"machineMaximumBrakeTorque"<<std::endl;
      //PrintMachineData(machineMaximumBrakeTorque);
      break;
     case 5:
      machineMaximumRegenerationTorque = machineData[0];
      //std::cout<<"machineMaximumRegenerationTorque"<<std::endl;
      //PrintMachineData(machineMaximumRegenerationTorque);
      break;
    }
  }
  //std::cout<<"Data for machine "<<machineIndex<<" successfully loaded"<<std::endl;
}

void Machine::PrintMachineData(std::vector<std::vector<double>>& machineData) {
   for(int i=0;i<machineData.size();i++) {
	std::vector<double> machineDataRow = machineData[i];
	for(int j=0;j<machineDataRow.size();j++)
	   std::cout<<machineDataRow[j]<<"	";
	std::cout<<std::endl;
   }
}

void Machine::PrintMachineData(std::vector<double>& machineData) {
  for(int i=0;i<machineData.size();i++) {
    std::cout<<machineData[i]<<std::endl;
  }
  std::cout<<std::endl;
}

void Machine::GetMaximumInstantaneousTorque() {
  //std::cout<<"Finding machine "<<machineAxleIndex<<"'s max. inst. torque at current vehicle speed"<<std::endl;
  double maximumBrakeTorque, maximumBrakePower;

  for(int i=0;i<possibleMachineSpeeds.size();i++) { // Each i refers to the gear index
    // Here, if machineSpeed=0, maximumBrakePower will also be calculated to zero!!!
    // Avoided by:if the m/c Speed < min. RPM, then it is bumped up to lowest possible machineSpeed.
    if(possibleMachineSpeeds[i]<rpmRangeForMaximumBrakeTorque[0]) {
      //std::cout<<"Machine speed "<<possibleMachineSpeeds[i]<<" RPM less than lowest listed RPM. Adjusting."<<std::endl;
      possibleMachineSpeeds[i]=rpmRangeForMaximumBrakeTorque[0];
    }
    double machineSpeed = possibleMachineSpeeds[i];
    //std::cout<<"machineSpeed "<<machineSpeed<<std::endl;

    // IFF machine speed < max. possible machine RPM will it be 'permitted'! Else, both the m/c speed & gear no. will be eliminated.
    //std::cout<<"rpmRangeForMaximumBrakeTorque.back() "<<rpmRangeForMaximumBrakeTorque.back()<<" RPM"<<std::endl;
    if(machineSpeed<rpmRangeForMaximumBrakeTorque.back()) {
      maximumBrakeTorque = GetMaximumBrakeTorque(machineSpeed);  // Nm
      maximumBrakePower = maximumBrakeTorque*machineSpeed*(2*3.14/60);  // W
      //std::cout<<"maximumBrakePower "<<maximumBrakePower<<std::endl;

      //std::cout<<"Gear ratio permitted "<<i<<std::endl;
      permittedMachineSpeeds.push_back(machineSpeed);            // Permitted machine speeds
      permittedMaximumTorques.push_back(maximumBrakeTorque);     // Permitted brake torques
      permittedGearRatios.push_back(i);                          // Permitted gear ratios
      permittedMaximumBrakePower.push_back(maximumBrakePower);   // Permitted brake power
    } else {
      //std::cout<<"Speed "<<machineSpeed<<" RPM corresponding to gear #"<<i<<" eliminated. Higher than max. allowed machine RPM"<<std::endl;
    }
  }

  maximumInstantaneousMachineTorque = permittedMaximumTorques[0];
  maximumInstantaneousMachinePower = permittedMaximumBrakePower[0];
  gearNumberAtMaximumTorque = permittedGearRatios[0];
  for(int i=1;i<permittedMaximumBrakePower.size();i++) {
    if(permittedMaximumBrakePower[i]>maximumInstantaneousMachinePower) {
      maximumInstantaneousMachinePower = permittedMaximumBrakePower[i];
      maximumInstantaneousMachineTorque = permittedMaximumTorques[i];
      gearNumberAtMaximumTorque = permittedGearRatios[i];
    }
  }  
  maximumInstantaneousPowertrainTorque = 
            transmissionForMachine->GetOutputTorque(maximumInstantaneousMachineTorque, gearNumberAtMaximumTorque);
  /*std::cout<<"maximumInstantaneousMachineTorque "<<maximumInstantaneousMachineTorque<<" Nm"<<std::endl;
  std::cout<<"maximumInstantaneousMachinePower "<<maximumInstantaneousMachinePower/1000<<" kW"<<std::endl; 
  std::cout<<"gearNumberAtMaximumTorque "<<gearNumberAtMaximumTorque<<std::endl;
  std::cout<<"maximumInstantaneousPowertrainTorque "<<maximumInstantaneousPowertrainTorque<<" Nm"<<std::endl;//*/
}

double Machine::GetMaximumBrakeTorque(double machineSpeed) {
  //std::cout<<"Finding max. brake torque at "<<machineSpeed<<" RPM"<<std::endl;
  int rpmIndex=0;
  double maximumBrakeTorqueAtGivenMachineSpeed;
  while(machineSpeed>rpmRangeForMaximumBrakeTorque[rpmIndex]) {
    rpmIndex++;
  }
  //std::cout<<"rpmIndex "<<rpmIndex<<std::endl;
  if(rpmIndex==0) { // Override to set available brake torque to minimum brake torque for low RPMs
    //std::cout<<"Machine speed low. Max. brake torque at this speed set to machineMaximumBrakeTorque[0]"<<std::endl;
    machineSpeed = rpmRangeForMaximumBrakeTorque[0];
    maximumBrakeTorqueAtGivenMachineSpeed = machineMaximumBrakeTorque[0];
  } else {
    /*std::cout<<"Machine speed "<<machineSpeed<<" lies between "<<rpmRangeForMaximumBrakeTorque[rpmIndex-1]<<"RPM and "<<rpmRangeForMaximumBrakeTorque[rpmIndex]<<"RPM"<<std::endl;
    std::cout<<"Machine max. brake torque lies between "<<machineMaximumBrakeTorque[rpmIndex-1]<<"N and "<<machineMaximumBrakeTorque[rpmIndex]<<"N"<<std::endl;//*/
    double interpolationRatio = (machineSpeed-rpmRangeForMaximumBrakeTorque[rpmIndex-1])
                            /(rpmRangeForMaximumBrakeTorque[rpmIndex]-rpmRangeForMaximumBrakeTorque[rpmIndex-1]);
    //std::cout<<"Interpolation ratio "<<interpolationRatio<<std::endl;
    maximumBrakeTorqueAtGivenMachineSpeed = machineMaximumBrakeTorque[rpmIndex-1]+ 
                  interpolationRatio*(machineMaximumBrakeTorque[rpmIndex]-machineMaximumBrakeTorque[rpmIndex-1]);
  }
  //std::cout<<"Max. brake torque at "<<machineSpeed<<" RPM is "<<maximumBrakeTorqueAtGivenMachineSpeed<<" Nm"<<std::endl;
  return maximumBrakeTorqueAtGivenMachineSpeed;
}

void Machine::GetMaximumInstantaneousRegenerativeTorque() {
  //std::cout<<"Find machine's max. inst. regenerative torques for all possible machine speeds "<<std::endl;
  double maximumRegenerativeTorque, maximumRegenerativePower;

  for(int i=0;i<possibleMachineSpeeds.size();i++) {
    // Here, if machineSpeed=0, maximumBrakePower will also be calculated to zero!!!
    // Avoided by:if the m/c Speed < min. RPM, then it is bumped up to lowest possible machineSpeed.
    if(possibleMachineSpeeds[i]<rpmRangeForMaximumBrakeTorque[0]) {
      //std::cout<<"Machine speed less than lowest listed RPM. Adjusting."<<std::endl;
      possibleMachineSpeeds[i]=rpmRangeForMaximumBrakeTorque[0];
    }
    double machineSpeed = possibleMachineSpeeds[i];

    // IFF machine speed < max. possible machine RPM will it be 'possible'! Else, both the m/c speed & gear no. will be eliminated.
    if(machineSpeed<rpmRangeForMaximumBrakeTorque.back()) {      
      maximumRegenerativeTorque = GetMaximumRegenerativeTorque(machineSpeed);     // Nm negative
      maximumRegenerativePower = maximumRegenerativeTorque*machineSpeed*(2*3.14/60);  // W
      /*std::cout<<"Permitted gear ratio "<<i<<std::endl;
      std::cout<<"maximumRegenerativePower "<<maximumRegenerativePower<<" W"<<std::endl;//*/

      permittedMachineSpeeds.push_back(machineSpeed);               // Permitted machine speeds
      permittedMaximumTorques.push_back(maximumRegenerativeTorque); // Permitted regenerative torques
      permittedGearRatios.push_back(i);    
      permittedMaximumRegenerativePower.push_back(maximumRegenerativePower);   // Permitted brake power
    } else {
      //std::cout<<"Speed "<<machineSpeed<<" RPM corresponding to gear #"<<i<<" eliminated. Higher than max. allowed machine RPM"<<std::endl;
    }
  }

  maximumInstantaneousRegenerativeMachinePower = permittedMaximumRegenerativePower[0];
  maximumInstantaneousRegenerativeMachineTorque = permittedMaximumTorques[0];
  gearNumberAtMaximumRegenerativeTorque = permittedGearRatios[0];
  for(int i=1;i<permittedMaximumRegenerativePower.size();i++) {
    if(permittedMaximumRegenerativePower[i]<=maximumInstantaneousRegenerativeMachinePower) {  // negative, hence <
      maximumInstantaneousRegenerativeMachinePower = permittedMaximumRegenerativePower[i];
      maximumInstantaneousRegenerativeMachineTorque = permittedMaximumTorques[i];
      gearNumberAtMaximumRegenerativeTorque = permittedGearRatios[i];
    }
  }  

  maximumInstantaneousRegenerationPowertrainTorque = 
            transmissionForMachine->GetOutputTorque(maximumInstantaneousRegenerativeMachineTorque, gearNumberAtMaximumRegenerativeTorque);
  /*std::cout<<"maximumInstantaneousRegenerativeMachineTorque "<<maximumInstantaneousRegenerativeMachineTorque<<std::endl;
  std::cout<<"maximumInstantaneousRegenerativeMachinePower "<<maximumInstantaneousRegenerativeMachinePower<<std::endl; 
  std::cout<<"gearNumberAtMaximumRegenerativeTorque "<<gearNumberAtMaximumRegenerativeTorque<<std::endl;
  std::cout<<"maximumInstantaneousRegenerationPowertrainTorque "<<maximumInstantaneousRegenerationPowertrainTorque<<std::endl;//*/
}

double Machine::GetMaximumRegenerativeTorque(double machineSpeed) {
  //std::cout<<"Finding max. regenerative torque at "<<machineSpeed<<" RPM"<<std::endl;
  int rpmIndex=0;
  double maximumRegenerativeTorqueAtCurrentSpeed;  // negative
  while(machineSpeed>rpmRangeForMaximumBrakeTorque[rpmIndex]) {
    rpmIndex++;
  }
  if(rpmIndex==0) { // Override to set available brake torque to minimum brake torque for low RPMs
    //std::cout<<"Machine speed very low. Max. brake torque at this speed is manually adjusted to machineMaximumRegenerationTorque[0]"<<std::endl;
    machineSpeed = rpmRangeForMaximumBrakeTorque[0];
    maximumRegenerativeTorqueAtCurrentSpeed = machineMaximumRegenerationTorque[0];  // 
  } else {
    double interpolationRatio = (machineSpeed-rpmRangeForMaximumBrakeTorque[rpmIndex-1])
                            /(rpmRangeForMaximumBrakeTorque[rpmIndex]-rpmRangeForMaximumBrakeTorque[rpmIndex-1]);
    //std::cout<<"Interpolation ratio "<<interpolationRatio<<std::endl;
    maximumRegenerativeTorqueAtCurrentSpeed = machineMaximumRegenerationTorque[rpmIndex-1]+ 
                  interpolationRatio*(machineMaximumRegenerationTorque[rpmIndex]-machineMaximumRegenerationTorque[rpmIndex-1]);
  }
  //std::cout<<"Max. regen torque at "<<machineSpeed<<" RPM is "<<maximumRegenerativeTorqueAtCurrentSpeed<<" Nm"<<std::endl;
  return maximumRegenerativeTorqueAtCurrentSpeed;
}

void Machine::RunMachine(double powertrainOutputTorque, double powertrainOutputRPM) {
  //std::cout<<"Running machine "<<machineIndex<<std::endl;
  
  GetPossibleMachineTorques(powertrainOutputTorque);

  std::vector<double> allowedMachineSpeeds, allowedMachineTorques, machineEfficiencies, gearIndices;
  //std::cout<<"Checking if machine torques <= max. torque. If not, we eliminate this entry and retain only those that are permitted"<<std::endl;
  for(int i=0;i<permittedMachineSpeeds.size();i++) {
    /*std::cout<<"possibleMachineTorques["<<i<<"] "<<possibleMachineTorques[i]<<std::endl;
    std::cout<<"permittedMaximumTorques["<<i<<"] "<<permittedMaximumTorques[i]<<std::endl;//*/

    if(std::abs((int)possibleMachineTorques[i])<=std::abs((int)permittedMaximumTorques[i])) { // Abs to allow -ve torques too
      //std::cout<<"Entry #"<<i<<" allowed"<<std::endl;
      double currentMachineEfficiency = GetMachineEfficiency(permittedMachineSpeeds[i], possibleMachineTorques[i]);
      allowedMachineSpeeds.push_back(permittedMachineSpeeds[i]);
      allowedMachineTorques.push_back(possibleMachineTorques[i]);
      machineEfficiencies.push_back(currentMachineEfficiency);
      gearIndices.push_back(permittedGearRatios[i]);
      //std::cout<<"Machine efficiency at permitted speed #"<<i<<" is "<<currentMachineEfficiency<<std::endl;
    }
  }

  /*double optimalMachineEfficiency=machineEfficiencies[0];
  int temporarySelectedTransmissionIndex=gearIndices[0];
  machineRPM = allowedMachineSpeeds[0];
  machineTorque = allowedMachineTorques[0];
  transmissionForMachine->operatingGearIndex=temporarySelectedTransmissionIndex;
  transmissionForMachine->operatingGearRatio=transmissionForMachine->gearRatios[temporarySelectedTransmissionIndex];
        
  if(machineEfficiencies.size()>1) {
    //std::cout<<std::endl<<"ENTERED FINDING OPTIMAL EFFICIENCY"<<std::endl;  
    //std::cout<<"Let optimal efficiency = "<<optimalMachineEfficiency<<std::endl;
    for(int i=1;i<allowedMachineSpeeds.size();i++) {
      //std::cout<<"machineEfficiencies["<<i<<"] "<<machineEfficiencies[i]<<std::endl;
      //std::cout<<"Optimal efficiency = "<<optimalMachineEfficiency<<std::endl;
      if(powertrainOutputTorque>=0) { // Propulsion
        if(machineEfficiencies[i]<optimalMachineEfficiency) { // Efficiency here is BSFC (g/J)
          optimalMachineEfficiency=machineEfficiencies[i];
          temporarySelectedTransmissionIndex=gearIndices[i];
          machineRPM = allowedMachineSpeeds[i];
          machineTorque = allowedMachineTorques[i];
          //std::cout<<"temporarySelectedTransmissionIndex "<<temporarySelectedTransmissionIndex<<" ";
          transmissionForMachine->operatingGearIndex=temporarySelectedTransmissionIndex;
          transmissionForMachine->operatingGearRatio=transmissionForMachine->gearRatios[temporarySelectedTransmissionIndex];
        }
      } else {  // Regeneration
        if(machineEfficiencies[i]>optimalMachineEfficiency) { // Different for regeneration!!!
          optimalMachineEfficiency=machineEfficiencies[i];
          temporarySelectedTransmissionIndex=gearIndices[i];
          machineRPM = allowedMachineSpeeds[i];
          machineTorque = allowedMachineTorques[i];
          //std::cout<<"temporarySelectedTransmissionIndex "<<temporarySelectedTransmissionIndex<<" ";
          transmissionForMachine->operatingGearIndex=gearIndices[i];
          transmissionForMachine->operatingGearRatio=transmissionForMachine->gearRatios[transmissionForMachine->operatingGearIndex];
        }
      }
    }
  }//*/

  double optimalMachineEfficiency=machineEfficiencies.back();
  int temporarySelectedTransmissionIndex=gearIndices.back();
  machineRPM = allowedMachineSpeeds.back();
  machineTorque = allowedMachineTorques.back();
  //std::cout<<"temporarySelectedTransmissionIndex "<<temporarySelectedTransmissionIndex<<" ";
  transmissionForMachine->operatingGearIndex=temporarySelectedTransmissionIndex;
  transmissionForMachine->operatingGearRatio=transmissionForMachine->gearRatios[temporarySelectedTransmissionIndex];  //*/

  machineOperatingEfficiency = optimalMachineEfficiency;

  //std::cout<<"Current transmission ratio is: "<<transmissionForMachine->operatingGearIndex<<std::endl;
  //std::cout<<"Machine operates at "<<machineRPM<<" RPM and "<<machineTorque<<" N"<<std::endl;
  
  /*if(machineRPMOverMission.size()>1 && machineRPM-machineRPMOverMission.back()>100) {
    std::cout<<"Abrupt increase in machineRPM"<<std::endl;
    machineRPM = machineRPMOverMission.back();
    machineTorque = machineTorqueOverMission.back();
    transmissionForMachine->operatingGearIndex=transmissionForMachine->gearIndexOverMission.back();
    transmissionForMachine->operatingGearRatio=transmissionForMachine->gearRatios[transmissionForMachine->operatingGearIndex];
  }//*/
  
  machineRPMOverMission.push_back(machineRPM);
  machineTorqueOverMission.push_back(machineTorque);
  machineEfficiencyOverMission.push_back(machineOperatingEfficiency);
  transmissionForMachine->gearIndexOverMission.push_back(transmissionForMachine->operatingGearIndex);
  
  // Actual power demanded from Buffer is that produced by the machine
  double powerDemandedFromBuffer = machineTorque*machineRPM*2*3.14159/60;   // Watts (W) !!!
  //std::cout<<"powerDemandedFromBuffer "<<powerDemandedFromBuffer<<" W"<<std::endl;
  bufferForMachine->RunBuffer(powerDemandedFromBuffer, optimalMachineEfficiency);

  ClearMachineValues();
}

void Machine::GetPossibleMachineSpeeds(double powertrainOutputRPM) {
  //std::cout<<"Calculating possible machine speeds for powertrain output "<<powertrainOutputRPM<<" RPM"<<std::endl;
  //std::cout<<"Possible machine speeds: ";//*/
  for(int i=0;i<transmissionForMachine->gearRatios.size();i++) {
    double possibleMachineSpeed = powertrainOutputRPM*transmissionForMachine->gearRatios[i];
    //std::cout<<possibleMachineSpeed<<" RPM at gear number "<<i<<" (ratio = "<<transmissionForMachine->gearRatios[i]<<") "<<std::endl;
    possibleMachineSpeeds.push_back(possibleMachineSpeed);
  }
  std::cout<<std::endl;
}

void Machine::GetPossibleMachineTorques(double powertrainOutputTorque) {
  //std::cout<<"Calculating possible machine torques for powertrain output "<<powertrainOutputTorque<<" N"<<std::endl;
  //std::cout<<"permittedGearRatios.size() "<<permittedGearRatios.size()<<std::endl;
  for(int i=0;i<permittedGearRatios.size();i++) {
    int gearIndex = permittedGearRatios[i];
    //std::cout<<"gear index "<<gearIndex<<std::endl;
    double transmissionRatio = transmissionForMachine->gearRatios[gearIndex];
    //std::cout<<"transmissionRatio "<<transmissionRatio<<std::endl;
    double possibleMachineTorque = powertrainOutputTorque/transmissionRatio;
    //std::cout<<"Possible machine torque = "<<possibleMachineTorque<<std::endl;
    possibleMachineTorques.push_back(possibleMachineTorque);
  }
}

double Machine::GetMachineEfficiency(double machineSpeed, double machineTorque) {
  //std::cout<<"Finding machine efficiency at "<<machineSpeed<<" RPM and "<<machineTorque<<" N"<<std::endl;
  int rpmIndex=0;
  int torqueIndex=0;
  double machineEfficiency;
  while(machineSpeed>machineRPMRange[rpmIndex] && rpmIndex<machineRPMRange.size()) {
    rpmIndex++;
  }
  while(machineTorque>machineTorqueRange[torqueIndex] && torqueIndex<machineTorqueRange.size()) {
    torqueIndex++;
  }
  if(rpmIndex!=0) {
    rpmIndex--;
  }
  if(torqueIndex!=0) {
    torqueIndex--;
  }
  /*std::cout<<"RPM index "<<rpmIndex<<std::endl;
  std::cout<<"Torque index "<<torqueIndex<<std::endl;//*/

  machineEfficiency=machineEfficiencyData[torqueIndex][rpmIndex];
  if(machineTorque>0){
    machineEfficiency=std::abs(machineEfficiency);
  }
  //std::cout<<"Machine efficiency at "<<machineSpeed<<" RPM and "<<machineTorque<<" N is "<<machineEfficiency<<std::endl;
  return machineEfficiency; 
}

void Machine::ClearMachineValues() {
  //std::cout<<"Clearing machine values "<<std::endl;
  possibleMachineSpeeds.clear();
  permittedMachineSpeeds.clear();
  permittedMaximumTorques.clear();
  permittedGearRatios.clear();
  permittedMaximumBrakePower.clear();
  permittedMaximumRegenerativePower.clear();
  possibleMachineTorques.clear();   
}

Machine::~Machine() {
}