#include "Unit.h"

typedef std::vector<int> Genes;
typedef std::vector<Genes> UnitGene;

Unit::Unit(int currentUnitIndex, UnitGene unitGenes) {
  unitIndex = currentUnitIndex;
  machineGenes = unitGenes[0];
  bufferGenes = unitGenes[1];
  axleGenes = unitGenes[2];
  numberOfAxles = axleGenes.size();
  GetNumberOfDrivenAxles();   

  // Creation Order - Buffer, Machine for each driven axle (buffer as reference), Axle (machine as reference, if propelled)

  unitBufferCount = 0;
  if(numberOfDrivenAxles!=0) {
    BuildUnitBuffer();
    //std::cout<<"Buffer created: "<<bufferInUnit->ReturnBufferName()<<std::endl;
  }

  unitEngineCount = 0;
  unitElectricMachineCount = 0;

  if(unitIndex == 0) {
	  //BuildTractorPowertrain();  // Note that the pointer to buffer is passed. Not actual object.
    BuildTractorAxles();
  } else {
    //std::cout<<"Entering trailer powertrain and axle creation"<<std::endl;
	  //BuildTrailerPowertrain(); // Note that the pointer to buffer is passed. Not actual object.
    BuildTrailerAxles();
  }
}

int Unit::GetNumberOfAxles() {
  return numberOfAxles;
}

void Unit::GetNumberOfDrivenAxles() {
  numberOfDrivenAxles = 0;
  for(int i=0;i<numberOfAxles;i++) {
    if(axleGenes[i]==1) {
      numberOfDrivenAxles+=1;
    }
  }
}

void Unit::BuildUnitBuffer() {
  // Buffer should be built IFF there is at least one powered axle! Taken care of in the Unit constructor.
  if(unitIndex==0) {
    bufferInUnit.reset(new FuelTank(unitIndex, bufferGenes)); 
  } else {
    bufferInUnit.reset(new Battery(unitIndex, bufferGenes)); 
  }
  unitBufferCount = 1;
}

/*void Unit::BuildTractorPowertrain() {
  std::shared_ptr<Machine> newMachine(new CombustionEngine(unitIndex, machineGenes, bufferInUnit));
  //std::cout<<newMachine->transmissionForMachine<<std::endl;
  //std::cout<<"Machine created: "<<newMachine->ReturnMachineName()<<std::endl;
  machinesInUnit.push_back(newMachine);
  unitEngineCount+=1;
}

void Unit::BuildTrailerPowertrain() {
  for(int i=0;i<numberOfDrivenAxles;i++) {
	  std::shared_ptr<Machine> newMachine(new ElectricMotor(unitIndex, machineGenes, bufferInUnit)); 
    //std::cout<<"Machine created ("<<i+1<<") : "<<newMachine->ReturnMachineName()<<std::endl;
	  machinesInUnit.push_back(newMachine);
    unitElectricMachineCount+=1;
  }
}//*/

void Unit::BuildTractorAxles() {
  std::shared_ptr<Machine> newMachine(new CombustionEngine(unitIndex, machineGenes, bufferInUnit,0));
  for(int i=0;i<numberOfAxles;i++) {
    std::shared_ptr<Axle> newAxle;
    int axleIndex = i;
    int axlePropulsionFlag = axleGenes[i];
    if(axlePropulsionFlag==1) {
      newAxle.reset(new Axle(axleIndex, axlePropulsionFlag, newMachine));
      //std::cout<<"Axle number "<<newAxle->GetAxleIndex()<<" created with machine "<<
                                //newAxle->ReturnMachineForAxle()->ReturnMachineName()<<std::endl;
    } else {
      newAxle.reset(new Axle(axleIndex, axlePropulsionFlag, nullptr));
      //std::cout<<"Axle number "<<newAxle->GetAxleIndex()<<" created with no machine. Non-propelled axle."<<std::endl;
    }
    axlesInUnit.push_back(newAxle);
  }
}

void Unit::BuildTrailerAxles() {
  for(int i=0;i<numberOfAxles;i++) {
    //std::cout<<"Trailer axle number "<<i<<" being created"<<std::endl;
    std::shared_ptr<Axle> newAxle;
    int axleIndex = i;
    int machineCount = 0;
    int axlePropulsionFlag = axleGenes[i];
    if(axlePropulsionFlag==1) {
      std::shared_ptr<Machine> newMachine(new ElectricMotor(unitIndex, machineGenes, bufferInUnit,i));
      newAxle.reset(new Axle(axleIndex, axlePropulsionFlag, newMachine));
      //std::cout<<"Axle number "<<newAxle->GetAxleIndex()<<" created with machine "<<
                                //newAxle->ReturnMachineForAxle()->ReturnMachineName()<<std::endl;
      machineCount++;
    } else {
      newAxle.reset(new Axle(axleIndex, axlePropulsionFlag, nullptr));
      //std::cout<<"Axle number "<<newAxle->GetAxleIndex()<<" created with no machine. Non-propelled axle."<<std::endl;
    }
    axlesInUnit.push_back(newAxle);
  }   
}

void Unit::AssignAxleParameters() {
  double axleDriveRatio;
  //std::cout<<"Assigning axle parameters initiated"<<std::endl;
  if(unitIndex==0) {
    axleDriveRatio=3.14;
  } else {
    axleDriveRatio=1;
  }
  for(int i=0;i<numberOfAxles;i++) {
    axlesInUnit[i]->DefineAxleParameters(loadEachAxle[i], unitTireRadius, unitCoefficientOfFriction, axleDriveRatio);
  }
  //std::cout<<"Assigning axle parameters completed"<<std::endl;
}

void Unit::GetMaximumTractivePower(double instantaneousSpeed) {
  //std::cout<<'\n'<<"Finding Unit "<<unitIndex<<"'s max. tractive power. GetMaximumTractivePower()"<<std::endl;
  unitInstantaneousSpeed = instantaneousSpeed;
  maximumInstantaneousPower=0;
  maximumInstantaneousTractiveForce=0;
   
  if(unitIndex!=0) { // Trailing Units      
    for(int i=0;i<numberOfAxles;i++) { 
      std::shared_ptr<Axle> currentAxle = axlesInUnit[i];
      if(currentAxle->machineForAxle!=nullptr) {   // Not all axles may be propelled        
        auto currentMachine = currentAxle->machineForAxle;
        //std::cout<<'\n'<<"Current machine's axle index "<<currentMachine->machineAxleIndex<<std::endl;

        currentAxle->UpdateAxleWheelSpeed(unitInstantaneousSpeed); // GetRpmBeforeAxleDifferential()
        currentMachine->GetPossibleMachineSpeeds(currentAxle->RPMToAxleDifferential);

        //std::cout<<"permittedMachineSpeeds.size() "<<currentMachine->permittedMachineSpeeds.size()<<std::endl;
        if(bufferInUnit->GetBufferAvailabilityRatio()>0) { // IFF buffer is useable! Don't remove this. This is for calculating max. possible traction. Not for allocation.
          //std::cout<<"Buffer is useable"<<std::endl;
          currentMachine->GetMaximumInstantaneousTorque();
        } else {
          //std::cout<<"Buffer is not useable"<<std::endl;
          currentMachine->maximumInstantaneousMachinePower=0; // This is actually the power required to keep the m/c & trans. running without energy, inertial running. (-ve)
          currentMachine->maximumInstantaneousPowertrainTorque=0;
        }

        maximumInstantaneousPower += currentMachine->maximumInstantaneousMachinePower; // Max. power cumulative for all axles
        //std::cout<<"Machine "<<i<<"'s machineMaximumInstantaneousPower "<<currentMachine->maximumInstantaneousMachinePower<<std::endl;
        double maximumTorqueAtAxleDifferential = currentMachine->maximumInstantaneousPowertrainTorque;
        //std::cout<<"maximumTorqueAtAxleDifferential "<<maximumTorqueAtAxleDifferential<<" N"<<std::endl;
        double maximumTractionFromSingleAxle = currentAxle->GetTractiveForceAtWheels(maximumTorqueAtAxleDifferential);
        //std::cout<<"maximumTractionFromSingleAxle "<<maximumTractionFromSingleAxle<<std::endl;
        maximumInstantaneousTractiveForce += maximumTractionFromSingleAxle;
      } 
    }
    //std::cout<<'\n'<<"maximumInstantaneousTractiveForce "<<maximumInstantaneousTractiveForce<<std::endl;
  } else {  // Tractor Unit
    std::shared_ptr<Axle> currentAxle = axlesInUnit[1]; // Rear axle
    std::shared_ptr<Machine> currentMachine = currentAxle->machineForAxle;
    auto transmission = currentMachine->transmissionForMachine;
    auto gearCount = transmission->gearRatios.size();

    currentAxle->UpdateAxleWheelSpeed(unitInstantaneousSpeed); 
    currentMachine->GetPossibleMachineSpeeds(currentAxle->RPMToAxleDifferential);
    
    if(bufferInUnit->GetBufferAvailabilityRatio()>0) { 
      currentMachine->GetMaximumInstantaneousTorque();
    } else {
      currentMachine->maximumInstantaneousMachinePower=0; 
      currentMachine->maximumInstantaneousPowertrainTorque=0;
    }

    maximumInstantaneousPower = currentMachine->maximumInstantaneousMachinePower; // Max. power of engine at current speed
    //std::cout<<"machineMaximumInstantaneousPower "<<maximumInstantaneousPower<<std::endl;
    double maximumTorqueAtAxleDifferential = currentMachine->maximumInstantaneousPowertrainTorque/2;  // Assuming equal torque split between two rear axles
    double maximumTractionFromSingleAxle = currentAxle->GetTractiveForceAtWheels(maximumTorqueAtAxleDifferential);
    //std::cout<<"maximumTractionFromSingleAxle "<<maximumTractionFromSingleAxle<<std::endl;
    maximumInstantaneousTractiveForce = maximumTractionFromSingleAxle*2;
    //std::cout<<"maximumInstantaneousTractiveForce "<<maximumInstantaneousTractiveForce<<std::endl;
  }
}

void Unit::GetMaximumRegenerativePower(double instantaneousSpeed) {
  //std::cout<<"Finding Unit "<<unitIndex<<"'s max. regen. tractive power. GetMaximumRegenerativePower()"<<std::endl;
  unitInstantaneousSpeed = instantaneousSpeed;
  maximumRegenerativePower=0;
  maximumInstantaneousRegenerativeTractiveForce=0;

  if(bufferInUnit!=nullptr) {
    for(int i=0;i<numberOfAxles;i++) {
      if(axlesInUnit[i]->machineForAxle!=nullptr) {
        //std::cout<<"Finding Unit "<<unitIndex<<"'s Axle "<<i<<"'s max. regen. tractive power"<<std::endl;
        std::shared_ptr<Axle> currentAxle = axlesInUnit[i];
        std::shared_ptr<Machine> currentMachine = currentAxle->machineForAxle;

        currentAxle->UpdateAxleWheelSpeed(unitInstantaneousSpeed); 
        currentMachine->GetPossibleMachineSpeeds(currentAxle->RPMToAxleDifferential);

        if(bufferInUnit->GetBufferAvailabilityRatio()<1) {
          currentMachine->GetMaximumInstantaneousRegenerativeTorque();
        } else {
          currentMachine->maximumInstantaneousRegenerativeMachinePower=0;
          currentMachine->maximumInstantaneousRegenerationPowertrainTorque=0;
        }
        maximumRegenerativePower += currentMachine->maximumInstantaneousRegenerativeMachinePower; // Max. power cumulative for all axles
        double maximumRegenerativeTorqueAtAxleDifferential = currentMachine->maximumInstantaneousRegenerationPowertrainTorque;
        double maximumAxleRegenerativeTractiveForce = currentAxle->GetTractiveForceAtWheels(maximumRegenerativeTorqueAtAxleDifferential);
        //std::cout<<"maximumAxleRegenerativeTractiveForce "<<maximumAxleRegenerativeTractiveForce<<" N"<<std::endl;
        maximumInstantaneousRegenerativeTractiveForce += maximumAxleRegenerativeTractiveForce;
      }
    }
    //std::cout<<"maximumInstantaneousRegenerativeTractiveForce "<<maximumInstantaneousRegenerativeTractiveForce<<" N"<<std::endl; 
  }
}

double Unit::GetUnitPowerAvailability() {  // BAR always lies between 0 & 1
  // This function is called iff currentUnit->bufferInUnit!=nullptr
  //std::cout<<"Finding unit "<<unitIndex<<"'s power availability"<<std::endl;
  instantaneousPowerAvailability = bufferInUnit->GetBufferAvailabilityRatio()*maximumInstantaneousPower;
  //std::cout<<"Unit "<<unitIndex<<"'s power availability is "<<instantaneousPowerAvailability<<std::endl;
  return instantaneousPowerAvailability;  
}

double Unit::GetUnitRegenerativeCapacity() { // BAR always lies between 0 & 1
  instantaneousRegenerativeCapacity = (1-bufferInUnit->GetBufferAvailabilityRatio())*maximumRegenerativePower;
  //std::cout<<"Unit "<<unitIndex<<"'s regenerative capacity is "<<instantaneousRegenerativeCapacity<<std::endl;
  return instantaneousRegenerativeCapacity;
}

void Unit::RunUnit(double assignedUnitTraction) {
  //std::cout<<"Running Unit "<<unitIndex<<std::endl; 
  //std::cout<<"assignedUnitTraction "<<assignedUnitTraction<<std::endl;
  unitTractiveForceDemandOverMission.push_back(assignedUnitTraction);
  if(assignedUnitTraction<0) {
    //std::cout<<"maximumInstantaneousRegenerativeTractiveForce "<<maximumInstantaneousRegenerativeTractiveForce<<std::endl; 
    if(assignedUnitTraction<maximumInstantaneousRegenerativeTractiveForce) {
      //std::cout<<"Regenerative tractive force requested from unit "<<unitIndex<<" is greater than max. unit regen.  traction"<<std::endl;
      demandedTractiveForce = maximumInstantaneousRegenerativeTractiveForce;
    } else {
      //std::cout<<"Regenerative tractive force requested from unit "<<unitIndex<<" does not exceed  max. unit regen. traction & can be met"<<std::endl;
      demandedTractiveForce = assignedUnitTraction;
    }
  } else if(assignedUnitTraction>0) {
    //std::cout<<"maximumInstantaneousTractiveForce "<<maximumInstantaneousTractiveForce<<std::endl; 
    if(assignedUnitTraction>maximumInstantaneousTractiveForce) {
      //std::cout<<"Tractive force requested from unit "<<unitIndex<<" is greater than max. unit traction"<<std::endl;
      demandedTractiveForce = maximumInstantaneousTractiveForce;
    } else {
      //std::cout<<"Tractive force requested from unit "<<unitIndex<<" does not exceed  max. unit traction & can be met"<<std::endl;
      demandedTractiveForce = assignedUnitTraction;
    }
  } else {
    //std::cout<<"Tractive force assigned = 0"<<std::endl;
      demandedTractiveForce = assignedUnitTraction;
  }

  if(bufferInUnit!=nullptr) { 
    if(demandedTractiveForce!=0) {
      //std::cout<<"Demanded tractive force not equal to zero"<<std::endl;
      if((demandedTractiveForce>0 && bufferInUnit->GetBufferAvailabilityRatio()>0) ||
                      (demandedTractiveForce<0 && bufferInUnit->GetBufferAvailabilityRatio()<1 &&
                                                       bufferInUnit->GetBufferAvailabilityRatio()>=0)) {      
        bufferInUnit->endOfStepPowerDemand=0;  // Reset step values of buffer states to zero        
        //std::cout<<"Buffer on unit "<<unitIndex<<" is available"<<std::endl;
        //std::cout<<"Splitting traction demand among "<<numberOfDrivenAxles<<" driven axles in unit "<<unitIndex<<std::endl; 
        double tractionDemandEachDrivenAxle = demandedTractiveForce/numberOfDrivenAxles;
        //std::cout<<"Each driven axle is assigned "<<tractionDemandEachDrivenAxle<<" N"<<std::endl;

        // Assign traction demand to each driven axle. Assign 0 traction to non-driven axles.
        for(int i=0;i<numberOfAxles;i++) {
          std::shared_ptr<Axle> currentAxle = axlesInUnit[i];
          
          // Perform grip limit check here and assign the right value to axle tractive force demand
          if(currentAxle->GetAxlePropulsionFlag()==1){
            //std::cout<<"Axle "<<i<<" is propelled"<<std::endl;
            if(std::abs(tractionDemandEachDrivenAxle)>currentAxle->gripLimitedTraction) {
              //std::cout<<"Traction demand assigned to axle "<<i<<" exceeds grip limit!!!"<<std::endl;
              currentAxle->axleTractiveForceDemand = 
                  tractionDemandEachDrivenAxle/std::abs(tractionDemandEachDrivenAxle)*currentAxle->gripLimitedTraction; 
              // Must assign negative torque in case of regeneration!
            } else {
              //std::cout<<"Traction demand assigned to axle "<<i<<" satisfies grip limit!!!"<<std::endl;
              currentAxle->axleTractiveForceDemand = tractionDemandEachDrivenAxle;
            }
          } else {
            //std::cout<<"Axle "<<i<<" is not propelled"<<std::endl;
            currentAxle->axleTractiveForceDemand = 0;
          }
          // With the right value of tractive force demand, calculate torque at axle differential
          currentAxle->GetTorqueToAxleDifferential(currentAxle->axleTractiveForceDemand);
        }

        // Assign the required torque & RPM at axle differential (calculated above) to the corresponding machines 
        if(unitIndex==0) {
          double propellerShaftTorque = axlesInUnit[1]->torqueToAxleDifferential*2;
          //std::cout<<"propellerShaftTorque "<<propellerShaftTorque<<" Nm"<<std::endl;
          double propellerShaftRPM = axlesInUnit[1]->RPMToAxleDifferential;
          //std::cout<<"propellerShaftRPM "<<propellerShaftRPM<<" RPM"<<std::endl;
          axlesInUnit[1]->machineForAxle->RunMachine(propellerShaftTorque, propellerShaftRPM);
        } else {
          for(int i=0;i<numberOfAxles;i++) {
            std::shared_ptr<Axle> currentAxle = axlesInUnit[i];
            if(currentAxle->GetAxlePropulsionFlag()==1) {
              std::shared_ptr<Machine> currentMachine = currentAxle->machineForAxle;
              currentMachine->RunMachine(currentAxle->torqueToAxleDifferential, currentAxle->RPMToAxleDifferential);
            }
          }
        }
        GetAchievedTractiveForce();
      } else {
        //std::cout<<"Buffer on unit "<<unitIndex<<" is NOT available!!!"<<std::endl;
        instantaneousTractiveForce=0;
      }
    } else { 
      //std::cout<<"Demanded tractive force equal to zero"<<std::endl;
      instantaneousTractiveForce=0;
    }
  } else { 
    //std::cout<<"No buffer on unit "<<unitIndex<<" . No axles propelled."<<std::endl;
    instantaneousTractiveForce=0;
  }
  /*bufferInUnit->bufferLevelOverMission.push_back(bufferInUnit->instantaneousBufferLevel);
  bufferInUnit->bufferPowerDemandOverMission.push_back(bufferInUnit->endOfStepPowerDemand);
  bufferInUnit->bufferAvailabilityRatioOverMission.push_back(bufferInUnit->bufferAvailabilityRatio);
  bufferInUnit->stateOfBufferOverMission.push_back(bufferInUnit->stateOfBuffer);//*/
  for(int i=0;i<axlesInUnit.size();i++) {
    auto axle = axlesInUnit[i];
    axle->axleWheelRPMOverMission.push_back(axle->axleTireRPM);
    axle->rpmAtAxleDifferentialOverMission.push_back(axle->RPMToAxleDifferential);
  }
}

void Unit::GetAchievedTractiveForce() {
  //std::cout<<std::endl<<"Getting unit "<<unitIndex<<"'s achieved tractive force"<<std::endl;
  instantaneousTractiveForce = 0;
  if(unitIndex==0) {
    std::shared_ptr<Axle> currentAxle = axlesInUnit[1];
    auto currentMachine = currentAxle->machineForAxle;
    auto currentTransmission = currentMachine->transmissionForMachine;

    double torqueToEachDrivenAxle = currentMachine->machineTorque*currentTransmission->operatingGearRatio/2;
    //std::cout<<"torqueToEachDrivenAxle "<<torqueToEachDrivenAxle<<" Nm"<<std::endl;
    double tractiveForceFromEachDrivenAxle = currentAxle->GetTractiveForceAtWheels(torqueToEachDrivenAxle);
    //std::cout<<"tractiveForceFromEachDrivenAxle "<<tractiveForceFromEachDrivenAxle<<" N"<<std::endl;
    instantaneousTractiveForce = tractiveForceFromEachDrivenAxle*2;
    //std::cout<<"instantaneousTractiveForce "<<instantaneousTractiveForce<<" N"<<std::endl;

    for(int i=1;i<axlesInUnit.size();i++) {
      std::shared_ptr<Axle> currentAxle = axlesInUnit[i];
      currentAxle->torqueAtAxleDifferentialOverMission.push_back(torqueToEachDrivenAxle);
      currentAxle->axleTractiveForceOverMission.push_back(instantaneousTractiveForce);
    }

  } else {
    for(int i=0;i<axlesInUnit.size();i++) {
      double axleTractiveForce=0;
      std::shared_ptr<Axle> currentAxle = axlesInUnit[i];
      if(currentAxle->GetAxlePropulsionFlag()==1) {
        auto currentMachine = currentAxle->machineForAxle;
        auto currentTransmission = currentMachine->transmissionForMachine;
        double torqueAtAxleDifferential = currentMachine->machineTorque*currentTransmission->operatingGearRatio;
        //std::cout<<"torqueAtAxleDifferential "<<torqueAtAxleDifferential<<" Nm"<<std::endl;
        axleTractiveForce = currentAxle->GetTractiveForceAtWheels(torqueAtAxleDifferential);

        currentAxle->torqueAtAxleDifferentialOverMission.push_back(torqueAtAxleDifferential);
        currentAxle->axleTractiveForceOverMission.push_back(axleTractiveForce);
      }
      instantaneousTractiveForce+=axleTractiveForce;
    }
  }      
  unitTractiveForceOverMission.push_back(instantaneousTractiveForce);
  //std::cout<<"Tractive force output of unit "<<unitIndex<<" accumulated over all axles is "<<instantaneousTractiveForce<<std::endl;
}

Unit::~Unit() {
   std::cout<<"Destroying Unit number "<<unitIndex<<std::endl;
}