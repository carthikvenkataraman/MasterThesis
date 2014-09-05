#include "ElectricMotor.h"
#include "MotorTransmission.h"

ElectricMotor::ElectricMotor(int currentUnitIndex, Genes machineGenes, std::shared_ptr<Buffer> bufferInUnit, int axleIndex) 
				: Machine(currentUnitIndex, machineGenes, bufferInUnit, axleIndex) {
	
	transmissionForMachine.reset(new MotorTransmission(currentUnitIndex));

	std::vector<int>::iterator it = std::find(machineGenes.begin(), machineGenes.end(),1);
	int machineSizeIndex = std::distance(machineGenes.begin(), it);
	//std::cout<<"machineSizeIndex "<<machineSizeIndex<<std::endl;//*/

	switch(machineSizeIndex) {
	 case 0:
	  unitPowertrainPrice = 30000;
	  unitPowertrainMass = 43;
	  break;
	 case 1:
	  unitPowertrainPrice = 30000;
	  unitPowertrainMass = 43;
	  break;
	 case 2:
	  unitPowertrainPrice = 30000;
	  unitPowertrainMass = 43;
	  break;
	}

	//std::cout<<"unitPowertrainMass"<<unitPowertrainMass<<'\n'<<"unitPowertrainPrice "<<unitPowertrainPrice<<'\n';
}

ElectricMotor::~ElectricMotor() {
}