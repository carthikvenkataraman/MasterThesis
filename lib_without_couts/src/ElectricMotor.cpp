#include "ElectricMotor.h"
#include "MotorTransmission.h"

ElectricMotor::ElectricMotor(int currentUnitIndex, Genes machineGenes, std::shared_ptr<Buffer> bufferInUnit, int axleIndex) 
				: Machine(currentUnitIndex, machineGenes, bufferInUnit, axleIndex) {
	
	transmissionForMachine.reset(new MotorTransmission(currentUnitIndex));

	switch(currentUnitIndex) {
	 case 0:
	  unitPowertrainPrice = 1000;
	  unitPowertrainMass = 0.2*120;
	  break;
	 case 1:
	  unitPowertrainPrice = 2500;
	  unitPowertrainMass = 0.2*173;
	  break;
	 case 2:
	  unitPowertrainPrice = 4000;
	  unitPowertrainMass = 0.2*180;
	  break;
	}
}

ElectricMotor::~ElectricMotor() {
}