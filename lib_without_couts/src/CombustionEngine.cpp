#include "CombustionEngine.h"
#include "MechanicalGearbox.h"

CombustionEngine::CombustionEngine(int currentUnitIndex, Genes machineGenes, std::shared_ptr<Buffer> bufferInUnit, int axleIndex) 
				: Machine(currentUnitIndex, machineGenes, bufferInUnit, axleIndex) {
					
	transmissionForMachine.reset(new MechanicalGearbox(currentUnitIndex));
	SetEngineOptimumOperatingPoint();
	//std::cout<<"best operating Torque: "<<bestOperatingTorque<<std::endl<<"best Operating RPM: "<<bestOperatingRPM<<std::endl;
	//std::cout<<"Engine created"<<std::endl;
}

void CombustionEngine::SetEngineOptimumOperatingPoint() {
	// Look only at positive torques
	int torqueStartingIndex = 0;
	while(machineTorqueRange[torqueStartingIndex]<0){
		torqueStartingIndex++;
	}
	bestOperatingBSFC = machineEfficiencyData[torqueStartingIndex][0];

	for(int i=torqueStartingIndex;i<machineTorqueRange.size();i++) { // Torque index
		std::vector<double> temporaryBSFCVector = machineEfficiencyData[i];
		// IMPORTANT - RPM Range reduced by 1 to find the optimum BSFC value in mid RPM Ranges 
		// and not the one at max. RPM and max. torque
		for(int j=0;j<machineRPMRange.size()-1;j++) {  // RPM index
			if(temporaryBSFCVector[j]<bestOperatingBSFC) {
				bestOperatingBSFC = temporaryBSFCVector[j];
				//std::cout<<"machineEfficiencyData["<<i<<"]["<<j<<"] "<<bestOperatingBSFC<<std::endl;
				bestOperatingTorque = machineTorqueRange[i];
				bestOperatingRPM = machineRPMRange[j];
			}
		}
	}
}

CombustionEngine::~CombustionEngine() {
}