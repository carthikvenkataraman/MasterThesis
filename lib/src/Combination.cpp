#include "Combination.h"

typedef std::vector<int> Genes;
typedef std::vector<Genes> UnitGene;
typedef std::vector<UnitGene> Individual;

Combination::Combination(Individual individual, std::vector<std::vector<double>> currentMissionData) {
	missionData = currentMissionData;
	AssignMissionData();
	numberOfUnits = individual.size();
	for(int i=0;i<numberOfUnits;i++) {
	   std::shared_ptr<Unit> newUnit;
	   int unitIndex = i;
	   UnitGene unitGenes = individual[i];
	   //std::cout<<"Creation of unit "<<unitIndex<<" initiated"<<std::endl;
	   newUnit.reset(new Unit(unitIndex, unitGenes));
	   unitsInCombination.push_back(newUnit);
	   //std::cout<<"Unit "<<unitIndex+1<<" created"<<std::endl<<std::endl;
	}
	AssignUnitParameters();
	//CheckUnitParameters();

	instantaneousAltitude = 0;	// Assumed to begin at h=0
	//altitudeOverMission.push_back(instantaneousAltitude);
	instantaneousSpeed = 0;	// Instantaneous Speed at time t=0 is set to 0
	//speedOverMission.push_back(instantaneousSpeed);
	instantaneousLongitudinalPosition = 0;	// Starting from x=0
	//positionOverMission.push_back(instantaneousLongitudinalPosition);
	timeInterval = 1.0;
	currentPositionIndex = 0;

	GetMaximumGripLimitedTraction();
	GetCombinationStartability();
}

void Combination::AssignMissionData() {
	targetSpeed = missionData[0];
	longitudinalPosition = missionData[1];
	roadGradientInRadians = missionData[2];

	revenuePerTonPerKM = missionData[3][0];            

	unitCosts = missionData[4];    
	powertrainPremium = missionData[5];
	motorCost = missionData[6];
	bufferCost = missionData[7];

	driverHourlyRate = missionData[8][0];
	fuelPricePerLitre = missionData[9][0];            
	electricityPricePerkWh = missionData[10][0];     

	kerbUnitWeight = missionData[11];
	std::vector<std::vector<double>> globalAxleLoads{missionData[12], missionData[13], missionData[14], missionData[15]};
	unitAxleLoads = globalAxleLoads;
	grossCombinationWeight = missionData[16][0];

	frontalArea = missionData[17][0];
	aerodynamicDragCoefficient = missionData[18][0];
	densityOfAir = missionData[19][0];
	rollingResistanceCoefficient = missionData[20][0];
	
	tireRadiusEachUnit = missionData[21];
	coefficientOfFriction = missionData[22];

	gravitationalAcceleration = missionData[23][0];

	tireInertia = missionData[24][0];
	axleInertia = missionData[25][0];
	propShaftInertia = missionData[26][0];
	clutchInertia = missionData[27][0];

	referenceSoC = missionData[28];

	motorMasses = missionData[29];
	batteryMasses = missionData[30];

	numberOfAnnualMissions = missionData[31][0];
	numberOfFirstOwnerYears = missionData[32][0];

	otherCostRatios = missionData[33];
}

void Combination::AssignUnitParameters() {
	for(int i=0;i<numberOfUnits;i++) {
		std::shared_ptr<Unit> currentUnit = unitsInCombination[i];
		currentUnit->unitTireRadius = tireRadiusEachUnit[i];
		currentUnit->loadEachAxle = unitAxleLoads[i];
		currentUnit->unitCoefficientOfFriction = coefficientOfFriction[i];
		currentUnit->gravitationalAcceleration = gravitationalAcceleration;
		currentUnit->AssignAxleParameters();
	}
}

void Combination::CheckUnitParameters() {
	/*for(int i=0;i<longitudinalPosition.size();i++) {
		std::cout<<longitudinalPosition[i]<<"	";
	}//*/
	////std::cout<<std::endl;
	for(int i=0;i<numberOfUnits;i++) {
		std::shared_ptr<Unit> currentUnit = unitsInCombination[i];
		std::cout<<"Unit "<<i<<"'s tire radius "<<currentUnit->unitTireRadius<<std::endl;
		for(int j=0;j<currentUnit->GetNumberOfAxles();j++) {
			std::cout<<"Axle"<<j<<"'s  load"<<currentUnit->loadEachAxle[j]<<std::endl;
		}
		std::cout<<"Unit "<<i<<"'s friction coefficient "<<currentUnit->unitCoefficientOfFriction<<std::endl;
		std::cout<<"G= "<<currentUnit->gravitationalAcceleration<<std::endl;
	}
}

void Combination::PrintMissionDataEntry(std::vector<double>& missionDataEntry) {
	for(int i=0;i<missionDataEntry.size();i++) {
			std::cout<<missionDataEntry[i]<<"	";
	}
	////std::cout<<std::endl<<std::endl;
}

void Combination::RunMission() {
	while(instantaneousLongitudinalPosition < longitudinalPosition.back()-1 && instantaneousSpeed>=0) {
		//////std::cout<<std::endl<<"STEP "<<testCount<<"---------------------------------------------------------------------------------------------"<<std::endl;
		//////std::cout<<std::endl<<"STEP "<<testCount<<std::endl;
		// Calculate exact instantaneous position and gradient
		double interpolationRatio;
		for(int i=currentPositionIndex+1;i<=longitudinalPosition.size();i++) {
			if(instantaneousLongitudinalPosition < longitudinalPosition[i]) {
				currentPositionIndex = i-1;
				if(currentPositionIndex==0) {
					interpolationRatio=0;
				} else {
					interpolationRatio = (instantaneousLongitudinalPosition - longitudinalPosition[i-1])/
												 (longitudinalPosition[i]-longitudinalPosition[i-1]);
				}
				break;
			}
		}	
		instantaneousGradient = roadGradientInRadians[currentPositionIndex] ;
		instantaneousTargetSpeed = targetSpeed[currentPositionIndex] + 
						interpolationRatio*(targetSpeed[currentPositionIndex+1]-targetSpeed[currentPositionIndex]);
		instantaneousReferenceSoC = referenceSoC[currentPositionIndex] + 
						interpolationRatio*(referenceSoC[currentPositionIndex+1]-referenceSoC[currentPositionIndex]);
		/*std::cout<<"instantaneousTargetSpeed "<<instantaneousTargetSpeed<<" m/s"<<std::endl;//*/
		/*std::cout<<"instantaneousSpeed "<<instantaneousSpeed<<" m/s"<<std::endl;//*/
		/*std::cout<<"instantaneousLongitudinalPosition "<<instantaneousLongitudinalPosition<<" m"<<std::endl;
		/*std::cout<<"instantaneousReferenceSoC "<<instantaneousReferenceSoC<<std::endl;//*/

		// Assign Reference SoC to each buffer
		for(int i=1;i<unitsInCombination.size();i++) {
			if(unitsInCombination[i]->bufferInUnit!=nullptr) {
				unitsInCombination[i]->bufferInUnit->UpdateReferenceSoC(instantaneousReferenceSoC);
			}
		}

		// Total Resistive Forces
		GetEquivalentMassDueToInertia();
		effectiveGrossCombinationWeight = grossCombinationWeight+equivalentMassDueToInertia;
		//std::cout<<"effectiveGrossCombinationWeight "<<effectiveGrossCombinationWeight<<" kg"<<std::endl;
		aerodynamicDragForce = 0.5*densityOfAir*aerodynamicDragCoefficient*frontalArea*pow(instantaneousSpeed,2);
		aerodynamicDragForceOverMission.push_back(aerodynamicDragForce);
		//std::cout<<"aerodynamicDragForce "<<aerodynamicDragForce<<" N"<<std::endl;
		// Only GCW used for rolling resistance force
		rollingResistanceForce = rollingResistanceCoefficient*grossCombinationWeight*gravitationalAcceleration*cos(instantaneousGradient);
		rollingResistanceForceOverMission.push_back(rollingResistanceForce);
		//std::cout<<"rollingResistanceForce "<<rollingResistanceForce<<" N"<<std::endl;
		gradientResistanceForce = effectiveGrossCombinationWeight*gravitationalAcceleration*sin(instantaneousGradient);
		gradientResistanceForceOverMission.push_back(gradientResistanceForce);
		//std::cout<<"gradientResistanceForce "<<gradientResistanceForce<<" N"<<std::endl;
		totalRoadResistanceForce = aerodynamicDragForce+rollingResistanceForce+gradientResistanceForce;
		totalRoadResistanceForceOverMission.push_back(totalRoadResistanceForce);
		//std::cout<<"Road resistance force "<<totalRoadResistanceForce<<" N at gradient "<<instantaneousGradient<<std::endl;

		// Acceleration Force
		requestedAcceleration = (instantaneousTargetSpeed-instantaneousSpeed)/timeInterval; // m/s^2
		//std::cout<<"Requested Acceleration "<<requestedAcceleration<<" m/s2"<<std::endl;
		requestedAccelerationForce = effectiveGrossCombinationWeight*requestedAcceleration;		// N
		requestedAccelerationForceOverMission.push_back(requestedAccelerationForce);

		// Total traction requested
		totalRequestedTractiveForce = totalRoadResistanceForce+requestedAccelerationForce;
		//std::cout<<"Total Requested Tractive Force : "<<totalRequestedTractiveForce<<" N"<<std::endl;
		double powerDemand = totalRequestedTractiveForce*(instantaneousSpeed+instantaneousTargetSpeed)/2;
		powerDemandOverMission.push_back(powerDemand);

		// Important step to avoid previous step values of inst. tractive force being used.
		for(int i=0;i<unitsInCombination.size();i++) {
			unitsInCombination[i]->instantaneousTractiveForce=0;
		}

		// Clear all machine values 
		for(int i=0;i<unitsInCombination.size();i++) {
			auto currentUnit = unitsInCombination[i];
			if(currentUnit->bufferInUnit!=nullptr) {
				for(int j=0;j<currentUnit->axlesInUnit.size();j++) {
					auto currentAxle = currentUnit->axlesInUnit[j];
					if(currentAxle->machineForAxle!=nullptr) {
						auto machine = currentAxle->machineForAxle;
						machine->ClearMachineValues();
					}
				}
			}
		}

		GetMaximumGripLimitedTraction();
		AssignTractiveForceAmongUnits();

		instantaneousAcceleration = (instantaneousTractiveForce-totalRoadResistanceForce)/effectiveGrossCombinationWeight;// m/s^2
		instantaneousLongitudinalPosition += instantaneousSpeed*timeInterval + (0.5*instantaneousAcceleration*pow(timeInterval,2)); // m

		testVector.push_back(maximumInstantaneousTraction);

		accelerationDemandOverMission.push_back(requestedAcceleration);
		accelerationOverMission.push_back(instantaneousAcceleration);
		tractiveForceDemandOverMission.push_back(totalRequestedTractiveForce);
		achievedTractiveForceOverMission.push_back(instantaneousTractiveForce);
		positionOverMission.push_back(instantaneousLongitudinalPosition);
		
		/*std::cout<<"totalRoadResistanceForce "<<totalRoadResistanceForce<<" N"<<std::endl;
		std::cout<<"Instantaneous Tractive Force "<<instantaneousTractiveForce<<" N"<<std::endl;
		std::cout<<"Achieved acceleration in this step: "<<instantaneousAcceleration<<" m/s^2"<<std::endl;
		std::cout<<"Current Position "<<instantaneousLongitudinalPosition<<" m"<<std::endl;//*/

		instantaneousSpeed += instantaneousAcceleration*timeInterval ;
		if(instantaneousSpeed>instantaneousTargetSpeed){
			instantaneousSpeed=instantaneousTargetSpeed;
		}
		speedOverMission.push_back(instantaneousSpeed);
		//std::cout<<"Current Speed "<<instantaneousSpeed<<" m/s"<<std::endl;

		totalTime++;

		auto fuelTank = unitsInCombination[0]->bufferInUnit;
		double cumulativeFuelConsumption = fuelTank->maximumBufferLevel-fuelTank->instantaneousBufferLevel;
		fuelConsumptionOverMission.push_back(cumulativeFuelConsumption);

		double cumulativeChargeConsumption=0;
		double stepPowerGenerated=0;
		for(int i=1;i<unitsInCombination.size();i++) {
			if(unitsInCombination[i]->bufferInUnit!=nullptr) {
				auto battery = unitsInCombination[i]->bufferInUnit;
				double unitChargeConsumed = battery->maximumBufferLevel-battery->instantaneousBufferLevel; // Coulombs
				cumulativeChargeConsumption+=unitChargeConsumed;
				stepPowerGenerated+=battery->endOfStepPowerDemand;
			}
		}
		stepPowerGenerated+=unitsInCombination[0]->bufferInUnit->endOfStepPowerDemand;
		chargeConsumptionOverMission.push_back(cumulativeChargeConsumption);
		achievedPowerOverMission.push_back(stepPowerGenerated);

		for(int i=0;i<unitsInCombination.size();i++) {
			auto unit = unitsInCombination[i];
			auto bufferInUnit = unit->bufferInUnit;
			if(unit->bufferInUnit!=nullptr) {
			  bufferInUnit->bufferLevelOverMission.push_back(bufferInUnit->instantaneousBufferLevel);
			  bufferInUnit->bufferPowerDemandOverMission.push_back(bufferInUnit->endOfStepPowerDemand);
			  bufferInUnit->bufferAvailabilityRatioOverMission.push_back(bufferInUnit->bufferAvailabilityRatio);
			  bufferInUnit->stateOfBufferOverMission.push_back(bufferInUnit->stateOfBuffer);
			  bufferInUnit->referenceSoCOverMission.push_back(bufferInUnit->referenceStateOfBuffer);
			}
		}
	}

	/*std::cout<<"tractionLimitCount "<<tractionLimitCount<<std::endl;
	std::cout<<"powerLimitCount "<<powerLimitCount<<std::endl;
	std::cout<<"testCount "<<testCount<<std::endl;//*/

	auto fuelTank = unitsInCombination[0]->bufferInUnit;
	totalFuelConsumption = fuelTank->maximumBufferLevel-fuelTank->instantaneousBufferLevel;
	//std::cout<<"totalFuelConsumption "<<totalFuelConsumption<<'\n';


	totalElectricEnergyToBeRecharged=0;
	for(int i=1;i<unitsInCombination.size();i++) {
		if(unitsInCombination[i]->bufferInUnit!=nullptr) {
			auto battery = unitsInCombination[i]->bufferInUnit;
			double unitChargeConsumed = 
				(battery->maximumStateOfBuffer-battery->stateOfBuffer)*battery->maximumBufferLevel; // Coulombs
			double unitEnergyConsumed = unitChargeConsumed*battery->openCircuitVoltage;	// J
			if(unitEnergyConsumed>0) {
				totalElectricEnergyToBeRecharged+=unitEnergyConsumed;	// J
			}
		}
	}

	GetMissionProductivity();
	//WriteOutputFiles();
}

void Combination::GetEquivalentMassDueToInertia() {
	equivalentTireMassRatio = 1/tireRadiusEachUnit[0];
	equivalentTireMass = tireInertia*pow(equivalentTireMassRatio,2);
	//std::cout<<"equivalentTireMass "<<equivalentTireMass<<" kg"<<std::endl;

	equivalentAxleMassRatio = equivalentTireMassRatio;
	equivalentAxleMass = axleInertia*pow(equivalentAxleMassRatio,2);
	//std::cout<<"equivalentAxleMass "<<equivalentAxleMass<<" kg"<<std::endl;

	equivalentPropShaftMassRatio = equivalentAxleMassRatio*unitsInCombination[0]->axlesInUnit[1]->axleFinalDriveRatio;
	equivalentPropShaftMass = axleInertia*pow(equivalentPropShaftMassRatio,2);
	//std::cout<<"equivalentPropShaftMass "<<equivalentPropShaftMass<<" kg"<<std::endl;

	int gearIndex;
	double gearRatio;
	if(instantaneousSpeed!=0) { 
		gearIndex = unitsInCombination[0]->axlesInUnit[1]->machineForAxle->transmissionForMachine->operatingGearIndex;
		gearRatio = unitsInCombination[0]->axlesInUnit[1]->machineForAxle->transmissionForMachine->operatingGearRatio;
	} else {
		gearIndex=0;
		gearRatio = unitsInCombination[0]->axlesInUnit[1]->machineForAxle->transmissionForMachine->gearRatios[0];
	}
	double gearInertia = unitsInCombination[0]->axlesInUnit[1]->machineForAxle->transmissionForMachine->gearInertias[gearIndex];
	equivalentTransmissionMassRatio = equivalentPropShaftMassRatio*gearRatio;
	equivalentTransmissionMass = gearInertia*pow(equivalentTransmissionMassRatio,2);
	//std::cout<<"equivalentTransmissionMass "<<equivalentTransmissionMass<<" kg"<<std::endl;

	//equivalentClutchMassRatio = equivalentTransmissionMass;
	//equivalentClutchMass = axleInertia*pow(equivalentClutchMassRatio,2);
	//std::cout<<"equivalentClutchMass "<<equivalentClutchMass<<" kg"<<std::endl;

	equivalentMassDueToInertia = equivalentTireMass+equivalentAxleMass+equivalentPropShaftMass+equivalentTransmissionMass;//+equivalentClutchMass;
	//std::cout<<"equivalentMassDueToInertia "<<equivalentMassDueToInertia<<" kg"<<std::endl;
}

void Combination::GetMaximumGripLimitedTraction() {
	maximumGripLimitedTraction = 0;
	for(int i=0;i<numberOfUnits;i++) {
		std::shared_ptr<Unit> currentUnit = unitsInCombination[i];
		for(int j=0;j<currentUnit->GetNumberOfAxles();j++) {
			//std::cout<<"Max. traction of Axle "<<j+1<<" in Unit "<<i+1<<std::endl;
			std::shared_ptr<Axle> currentAxle = currentUnit->axlesInUnit[j];
			if(currentAxle->GetAxlePropulsionFlag()==1) { // Only propelled axles provide traction
				/*std::cout<<"Axle "<<j+1<<" is propelled"<<std::endl;
				std::cout<<"Coefficient of friction is "<<currentAxle->GetAxleCoefficientOfFriction()<<std::endl;
				std::cout<<"Axle load is "<<currentAxle->GetAxleLoad()<<std::endl;
				std::cout<<"Gravity is "<<gravitationalAcceleration<<std::endl;*/
				currentAxle->gripLimitedTraction = 
					(currentAxle->GetAxleCoefficientOfFriction()) * (currentAxle->GetAxleLoad()) * gravitationalAcceleration;
				//std::cout<<currentAxle->gripLimitedTraction<<std::endl;
			} else {
				//std::cout<<"Axle "<<j+1<<" is not propelled"<<std::endl;
				currentAxle->gripLimitedTraction = 0;
				//std::cout<<currentAxle->gripLimitedTraction<<std::endl;
			}
			maximumGripLimitedTraction += currentAxle->gripLimitedTraction;
		}
	}
	//std::cout<<"Max. grip Traction: "<<maximumGripLimitedTraction<<std::endl;
}

void Combination::AssignTractiveForceAmongUnits() {
	if(totalRequestedTractiveForce<maximumGripLimitedTraction) {
			//std::cout<<"Requested traction less than max. grip limited traction"<<std::endl;
			tractiveForceToBeDivided = totalRequestedTractiveForce;
		} else {
			//std::cout<<"Requested traction more than max. grip limited traction "<<maximumGripLimitedTraction<<"!! WARNING!!"<<std::endl;
			tractiveForceToBeDivided = maximumGripLimitedTraction;
			tractionLimitCount++;
			//std::cout<<"tractionLimitCount "<<tractionLimitCount<<std::endl;
	}

	//std::cout<<"tractiveForceToBeDivided after grip limit check: "<<tractiveForceToBeDivided<<std::endl;
	instantaneousPowerDemand = tractiveForceToBeDivided*(instantaneousSpeed+instantaneousTargetSpeed)/2;
	//std::cout<<"Instantaneous power demand after grip limit check: "<<instantaneousPowerDemand/1000<<" kW"<<std::endl;
	// NOTE: power demand is zero only if BOTH current speed and target speed is zero!! This automatically
	// implies that the tractive force demand is zero!!

	if(instantaneousPowerDemand>=0) { // Propulsion		
		//////std::cout<<std::endl<<"Propulsion mode engaged"<<std::endl<<'\n';
		double maximumTractionAtGivenSpeed = GetMaximumTraction(instantaneousSpeed);
		if(tractiveForceToBeDivided<=maximumTractionAtGivenSpeed) {
			//std::cout<<"Tractive Force To Be Divided "<<tractiveForceToBeDivided<<" less than max. traction "<<maximumTractionAtGivenSpeed<<" N"<<std::endl;

			// What is the traction produced by the tractor unit if the engine runs at its best efficiency point?
			auto engine = unitsInCombination[0]->axlesInUnit[1]->machineForAxle; // Rear axle
			double tractorTractionAtEngineOptimumSpeed = GetTractorTraction(engine->bestOperatingRPM, engine->bestOperatingTorque);

			// What is the remaining traction that the trailing units will have to deliver?
			double tractionRequestedFromTrailingUnits, tractionRequestedFromTractor;
			if(tractorTractionAtEngineOptimumSpeed<unitsInCombination[0]->maximumInstantaneousTractiveForce) {
				tractionRequestedFromTractor=tractorTractionAtEngineOptimumSpeed;
			} else {
				tractionRequestedFromTractor=unitsInCombination[0]->maximumInstantaneousTractiveForce;
			}			
			tractionRequestedFromTrailingUnits = tractiveForceToBeDivided-tractionRequestedFromTractor;
			//std::cout<<"tractionRequestedFromTrailingUnits "<<tractionRequestedFromTrailingUnits<<std::endl;

			// Can motors provide this?
			maximumTractionFromTrailingUnits=0;
			for(int i=1;i<unitsInCombination.size();i++){
					//std::cout<<"Unit "<<i<<"'s max inst. traction"<<unitsInCombination[i]->maximumInstantaneousTractiveForce<<std::endl;
					maximumTractionFromTrailingUnits+=unitsInCombination[i]->maximumInstantaneousTractiveForce;
					//std::cout<<"Max traction from trailing units (intermediate)" <<maximumTractionFromTrailingUnits<<std::endl; 
			}
			//std::cout<<"maximumTractionFromTrailingUnits "<<maximumTractionFromTrailingUnits<<std::endl<<'\n';

			if(tractionRequestedFromTrailingUnits>=0) { //tractiveForceToBeDivided > tractionRequestedFromTractor
				//std::cout<<"TRACTION IS DEMANDED FROM TRAILING UNITS!!!"<<std::endl;
				
				if(tractionRequestedFromTrailingUnits <= maximumTractionFromTrailingUnits) {
					//std::cout<<"Demanded traction from trailing units can be provided by motors!"<<std::endl<<'\n';
					// Run engine at optimum speed & divide the required remaining power among the trailing Units
					//std::cout<<"Run Engine at optimum operating point"<<std::endl;
					unitsInCombination[0]->RunUnit(tractionRequestedFromTractor); 
					//std::cout<<'\n'<<"Find distribution ratios for splitting remaining traction among trailing units"<<std::endl;
					SetTractionDistributionRatios();
					for(int i=1;i<unitsInCombination.size();i++) {
						if(unitsInCombination[i]->bufferInUnit!=nullptr) {
							double assignedUnitTraction = 
										unitsInCombination[i]->tractionDistributionRatio*tractionRequestedFromTrailingUnits;
							unitsInCombination[i]->RunUnit(assignedUnitTraction);
						}
					}
					operatingModeOverMission.push_back(1);
					//std::cout<<"Operating Mode: 1"<<'\n';
				} else {
					// Else, assign max. possible torque to all trailing units and assign remaining torque to tractor
					/*std::cout<<"Trailing units cannot provide requested traction."<<std::endl;
					std::cout<<"Trailing units will provide max. possible traction. Engine provides remaining."<<std::endl;//*/
					for(int i=1;i<unitsInCombination.size();i++) {
						if(unitsInCombination[i]->bufferInUnit!=nullptr) {
							//std::cout<<"Max inst. tractive force for unit "<<i<<" "<<unitsInCombination[i]->maximumInstantaneousTractiveForce<<std::endl;
							double assignedUnitTraction = unitsInCombination[i]->maximumInstantaneousTractiveForce;
							//std::cout<<"assignedUnitTraction for unit "<<i<<" "<<assignedUnitTraction<<std::endl;
							unitsInCombination[i]->RunUnit(assignedUnitTraction);
						}
					}
					double tractionToBeDeliveredByTractor = tractiveForceToBeDivided-maximumTractionFromTrailingUnits;
					//std::cout<<"Traction to be delivered by tractor "<<tractionToBeDeliveredByTractor<<" N"<<std::endl;
					unitsInCombination[0]->RunUnit(tractionToBeDeliveredByTractor);
					//std::cout<<"Checking inst. tractive force of unit 1 "<<unitsInCombination[1]->instantaneousTractiveForce<<std::endl;
					operatingModeOverMission.push_back(2);
					//std::cout<<"Operating Mode: 2"<<'\n';
				}
			} else { //tractiveForceToBeDivided < tractorTractionAtEngineOptimumSpeed 
				//std::cout<<"POWER REQUIRED LESS THAN OPTIMUM ENGINE OPERATING POWER"<<std::endl;
				/*unitsInCombination[0]->RunUnit(tractiveForceToBeDivided);
				for(int i=1;i<unitsInCombination.size();i++) {
					if(unitsInCombination[i]->bufferInUnit!=nullptr) {
						unitsInCombination[i]->RunUnit(0);
					}
				}
				operatingModeOverMission.push_back(3);//*/
				//--------------------------------------------------------------------------------------	
				// ALTERNATE WAY OF HANDLING POWER REQUIREMENTS LOWER THAN OPTIMUM ENGINE POWER	
				//--------------------------------------------------------------------------------------	
				auto engine = unitsInCombination[0]->axlesInUnit[1]->machineForAxle; // Rear axle
				double tractorTractionAtEngineIdlingSpeed = GetTractorTraction(engine->idlingRPM, engine->idlingTorque);
				double tractionFromTrailingUnits;
				tractionFromTrailingUnits=tractiveForceToBeDivided-tractorTractionAtEngineIdlingSpeed;
				if(tractionFromTrailingUnits>0) {
					if(tractionFromTrailingUnits <= maximumTractionFromTrailingUnits) {
						unitsInCombination[0]->RunUnit(tractorTractionAtEngineIdlingSpeed); 
						SetTractionDistributionRatios();
						for(int i=1;i<unitsInCombination.size();i++) {
							if(unitsInCombination[i]->bufferInUnit!=nullptr) {
								double assignedUnitTraction = 
										unitsInCombination[i]->tractionDistributionRatio*tractionFromTrailingUnits;
								unitsInCombination[i]->RunUnit(assignedUnitTraction);
							}
						}
						operatingModeOverMission.push_back(3);
						//std::cout<<"Operating Mode: 3"<<'\n';
					} else {
						for(int i=1;i<unitsInCombination.size();i++) {
							if(unitsInCombination[i]->bufferInUnit!=nullptr) {
								double assignedUnitTraction = unitsInCombination[i]->maximumInstantaneousTractiveForce;
								unitsInCombination[i]->RunUnit(assignedUnitTraction);
							}
						}
						double tractionToBeDeliveredByTractor = tractiveForceToBeDivided-maximumTractionFromTrailingUnits;
						unitsInCombination[0]->RunUnit(tractionToBeDeliveredByTractor);
						operatingModeOverMission.push_back(4);
						//std::cout<<"Operating Mode: 4"<<'\n';
					}
				} else {
					unitsInCombination[0]->RunUnit(tractiveForceToBeDivided);
					for(int i=1;i<unitsInCombination.size();i++) {
						if(unitsInCombination[i]->bufferInUnit!=nullptr) {
						unitsInCombination[i]->RunUnit(0);
						}
					}
					operatingModeOverMission.push_back(5);
					//std::cout<<"Operating Mode: 5"<<'\n';
				}//*/
			}

		} else {
			//std::cout<<"Tractive Force To Be Divided MORE than max. traction!! All units will provide max. traction!!"<<std::endl;
			powerLimitCount++;
			for(int i=0;i<numberOfUnits;i++) {
				if(unitsInCombination[i]->bufferInUnit!=nullptr) {
					std::shared_ptr<Unit> currentUnit = unitsInCombination[i];
					//std::cout<<"Max inst. tractive force for unit "<<i<<" "<<currentUnit->maximumInstantaneousTractiveForce<<std::endl;
					currentUnit->RunUnit(currentUnit->maximumInstantaneousTractiveForce);
				}
			}
			//operatingModeOverMission.push_back(4);
			// USE THE FOLLOWING ONE IF YOU USE THE ALTERNATIVE ENERGY MANAGEMENT STRATEGY
			operatingModeOverMission.push_back(6);
			//std::cout<<"Operating Mode: 6"<<'\n';
		}

		// Calculate actual acceleration achieved
		//std::cout<<"All units have been run. Will now calculate inst. tractive force for whole combination"<<std::endl;
		GetInstantaneousTractiveForce();

	} else { // Regeneration
		//std::cout<<'\n'<<"Regeneration mode engaged"<<std::endl<<'\n';
		double maximumRegenerativeTraction = GetMaximumRegenerativeTraction(instantaneousSpeed);

		// RUN UNITS IFF max. regen traction != 0 . IMPORTANT.
		if(maximumRegenerativeTraction<0) { // max. regen traction will be zero if buffer availability = 1. Then units will not be run. 
			if(tractiveForceToBeDivided>=maximumRegenerativeTraction) { //Powers are now negative. Hence >=
				//std::cout<<"Regen. tractive force less than max. regen. traction. Can be met. Will now be distributed among available units."<<std::endl;
				SetRegenerationDistributionRatios();
				for(int i=1;i<unitsInCombination.size();i++) {
					if(unitsInCombination[i]->bufferInUnit!=nullptr) {
						double assignedUnitRegenerativeTraction = 
							unitsInCombination[i]->regenerationDistributionRatio*tractiveForceToBeDivided;
						unitsInCombination[i]->RunUnit(assignedUnitRegenerativeTraction);
					}
				}
				operatingModeOverMission.push_back(-1);
				//std::cout<<"Operating Mode: -1"<<'\n';
			} else {
				//std::cout<<"Regenerative Force To Be Divided MORE than max. traction!! All units will provide max. regen. traction!!"<<std::endl;
				for(int i=1;i<unitsInCombination.size();i++) {
					if(unitsInCombination[i]->bufferInUnit!=nullptr) {
						//std::cout<<"Running unit "<<i<< " with its max. inst. regen. tractive force"<<std::endl;
						std::shared_ptr<Unit> currentUnit = unitsInCombination[i];
						currentUnit->RunUnit(currentUnit->maximumInstantaneousRegenerativeTractiveForce);
					}
				}
				operatingModeOverMission.push_back(-2);
				//std::cout<<"Operating Mode: -2"<<'\n';
			}
		} else {
			//std::cout<<"Regeneration not possible. Service braking employed."<<std::endl;
			operatingModeOverMission.push_back(0);
			//std::cout<<"Operating Mode: 0"<<'\n';
		} 

		// Manually set deceleration to required value
		instantaneousTractiveForce=totalRequestedTractiveForce;
	}
}

double Combination::GetMaximumTraction(double instantaneousSpeed) {
	//std::cout<<"Inside GetMaximumTraction() "<<std::endl;
	maximumInstantaneousTraction=0;
	for(int i=0;i<numberOfUnits;i++) {
		if(unitsInCombination[i]->bufferInUnit!=nullptr) { // Only if there are any powered axles in the unit will there be a buffer! 
			unitsInCombination[i]->GetMaximumTractivePower(instantaneousSpeed);
			/*//To check if the max tractive force actually calculated in Unit is preserved. 
			double unitMaxInstTraction = unitsInCombination[i]->maximumInstantaneousTractiveForce;
			std::cout<<"unitMaxInstTraction checked again "<<unitMaxInstTraction<<std::endl;//*/
		} else {
			unitsInCombination[i]->maximumInstantaneousPower=0;
			unitsInCombination[i]->maximumInstantaneousTractiveForce=0;
		}
		maximumInstantaneousTraction+=unitsInCombination[i]->maximumInstantaneousTractiveForce;
		maximumInstantaneousPower+=unitsInCombination[i]->maximumInstantaneousPower;
		//std::cout<<"Maximum instantaneous traction (intermediate): "<<maximumInstantaneousTraction<<" N"<<std::endl;
	}
	//std::cout<<'\n'<<"Maximum instantaneous traction (final): "<<maximumInstantaneousTraction<<" N"<<std::endl;
	////std::cout<<std::endl;
	return maximumInstantaneousTraction;
}

double Combination::GetMaximumRegenerativeTraction(double instantaneousSpeed) {
	double maximumRegenerativePower = 0;
	double maximumRegenerativeTraction = 0;
	for(int i=1;i<numberOfUnits;i++) {  // From unit 1 to last unit
		if(unitsInCombination[i]->bufferInUnit!=nullptr) { // Only if there are any powered axles in the unit will there be a buffer! 
			//std::cout<<"Unit "<<i<<" has a buffer. Will find max. regen. traction"<<std::endl;
			unitsInCombination[i]->GetMaximumRegenerativePower(instantaneousSpeed);	// Negative
		} else {
			unitsInCombination[i]->maximumRegenerativePower=0;
			unitsInCombination[i]->maximumInstantaneousRegenerativeTractiveForce=0;
		}
		maximumRegenerativePower+=unitsInCombination[i]->maximumRegenerativePower;
		maximumRegenerativeTraction+=unitsInCombination[i]->maximumInstantaneousRegenerativeTractiveForce;
		//std::cout<<"Maximum regenerative traction (intermediate): "<<maximumRegenerativeTraction<<" N"<<std::endl;
	}
	//std::cout<<"Maximum regenerative traction (final): "<<maximumRegenerativeTraction<<" N"<<std::endl;
	return maximumRegenerativeTraction; // Negative
}

void Combination::SetTractionDistributionRatios() {
	double instantaneousTotalPowerAvailability=0;
	for(int i=1;i<unitsInCombination.size();i++) {
		// Find power availability only if the unit has a buffer!! 
		if(unitsInCombination[i]->bufferInUnit!=nullptr){
			instantaneousTotalPowerAvailability += unitsInCombination[i]->GetUnitPowerAvailability();
		}
	}
	//std::cout<<"instantaneousTotalPowerAvailability "<<instantaneousTotalPowerAvailability<<std::endl;
	for(int i=1;i<unitsInCombination.size();i++) {
		if(unitsInCombination[i]->bufferInUnit!=nullptr){		
			unitsInCombination[i]->tractionDistributionRatio = 
					unitsInCombination[i]->instantaneousPowerAvailability/instantaneousTotalPowerAvailability;
		} else {
			unitsInCombination[i]->tractionDistributionRatio = 0;
		}
		//std::cout<<"Unit "<<i<<"'s tractionDistributionRatio "<<unitsInCombination[i]->tractionDistributionRatio<<std::endl;
	}
}

void Combination::SetRegenerationDistributionRatios() {
	double instantaneousTotalRegenerativeCapacity;
	for(int i=1;i<unitsInCombination.size();i++) {
		if(unitsInCombination[i]->bufferInUnit!=nullptr) {
			instantaneousTotalRegenerativeCapacity += unitsInCombination[i]->GetUnitRegenerativeCapacity();
		}
	}
	for(int i=1;i<unitsInCombination.size();i++) {
		if(unitsInCombination[i]->bufferInUnit!=nullptr){		
			unitsInCombination[i]->regenerationDistributionRatio = 
					unitsInCombination[i]->instantaneousRegenerativeCapacity/instantaneousTotalRegenerativeCapacity;
		} else {
			unitsInCombination[i]->regenerationDistributionRatio = 0;
		}
	}
}

double Combination::GetTractorTraction(double engineRPM, double engineTorque) {
	//////std::cout<<std::endl<<"Calculating tractor traction if engine is run at its optimum operating point"<<std::endl;
	std::shared_ptr<Axle> currentAxle = unitsInCombination[0]->axlesInUnit[1]; // Rear axle
	auto engine = currentAxle->machineForAxle;
	auto transmission = engine->transmissionForMachine;

	double requiredTransmissionRatio;
	//std::cout<<"RPM at Differential is "<<currentAxle->RPMToAxleDifferential<<" RPM"<<std::endl;
	// IMPORTANT CHECK BELOW!!!c
	if(currentAxle->RPMToAxleDifferential!=0) { 
	// This makes sense if the combination already has a speed. Then compared to the engine optimum speed,
	// we can calculate the transmission ratio that will yield the right rpm at the axle differential.
	// But, if the combination speed is zero, no specific gear ratio is required. We can thus say that, at standstill,
	// while the engine may operate at its optimum point, the transmission ratio is set to the highest possible 
	// to get the highest traction.		
		requiredTransmissionRatio = engineRPM/currentAxle->RPMToAxleDifferential;
		// What if the required transmission ratio is too high or too low? Beyond the range of the gearbox?

	} else{ // If at standstill, find the traction got at lowest gear and engine optimum operating point
		//std::cout<<"Vehicle at standstill. Using lowest gear ratio to find unit traction at engine optimum speed"<<std::endl;
		requiredTransmissionRatio =transmission->gearRatios[0];
	}
	//std::cout<<"Transmission ratio required "<<requiredTransmissionRatio<<std::endl;
	engine->gearIndexAtGivenSpeed=0;
	while(engine->gearIndexAtGivenSpeed<transmission->gearRatios.size() && 
				requiredTransmissionRatio<=transmission->gearRatios[engine->gearIndexAtGivenSpeed]){
		// If required ratio is permanently lower than the lowest, it will enter an infinite loop - OVERDRIVE!!!
		engine->gearIndexAtGivenSpeed++;
	}
	// If the required trans. ratio is too high, gearIndexForOptBSFC will remain at 0 since the while loop won't be entered.
	if(engine->gearIndexAtGivenSpeed!=0) { 
		engine->gearIndexAtGivenSpeed--;
	}
	/*std::cout<<"Gear index at optimum BSFC "<<engine->gearIndexAtGivenSpeed<<std::endl;
	std::cout<<"Best engine operating torque "<<engine->bestOperatingTorque<<" Nm"<<std::endl;//*/
	double torqueAtEachAxleDifferential = transmission->GetOutputTorque(engineTorque, engine->gearIndexAtGivenSpeed)/2;
	//std::cout<<"Optimum torque at each axle differential "<<torqueAtEachAxleDifferential<<std::endl;
	// Note: The actual engine rpm is NOT best operating rpm since gear ratio is not exactly what is required.
	// Hence, the output torque at this altered rpm is NOT the engine's optimum operating torque.
	// In the above calculation, it is assumed that the engine's optimum operating torque is available even at this rpm.
	double tractionAtEachAxle = currentAxle->GetTractiveForceAtWheels(torqueAtEachAxleDifferential);
	double tractionAtWheelsDueToEngine = tractionAtEachAxle*2;
	//std::cout<<"Tractor traction at engine optimum operating point "<<tractionAtWheelsDueToEngine<<std::endl;
	////std::cout<<std::endl;
	return tractionAtWheelsDueToEngine;
}

void Combination::GetInstantaneousTractiveForce() {
	instantaneousTractiveForce=0;
	for(int i=0;i<numberOfUnits;i++) {
		if(unitsInCombination[i]->bufferInUnit!=nullptr) {
			//std::cout<<"Tractive force produced by unit "<<i<<" is "<<unitsInCombination[i]->instantaneousTractiveForce<<" N"<<std::endl;
			instantaneousTractiveForce += unitsInCombination[i]->instantaneousTractiveForce;
		}
	}
	//std::cout<<"Achieved tractive force "<<instantaneousTractiveForce<<std::endl;
}

void Combination::GetCombinationStartability() {
	double startingSpeed = 0; // m/s
	double startabilityTraction = GetMaximumTraction(startingSpeed);
	//std::cout<<"Max. combination traction "<<startabilityTraction<<" N"<<std::endl;
	if(startabilityTraction>maximumGripLimitedTraction) {
		//std::cout<<"Max. combination traction "<<startabilityTraction
									//<<" N > maximumGripLimitedTraction"<<maximumGripLimitedTraction<<" N"<<std::endl;
		startabilityTraction=maximumGripLimitedTraction;
	}
	GetEquivalentMassDueToInertia();
	effectiveGrossCombinationWeight = grossCombinationWeight+equivalentMassDueToInertia;
	double combinationStartabilityInRadians = asin(startabilityTraction/(effectiveGrossCombinationWeight*gravitationalAcceleration));
	combinationStartability = combinationStartabilityInRadians*180/M_PI;
	//std::cout<<"Combination startability "<<combinationStartability<<" degrees"<<std::endl;

	for(int i=0;i<unitsInCombination.size();i++) {
		if(unitsInCombination[i]->bufferInUnit!=nullptr) {
			auto currentUnit = unitsInCombination[i];
			for(int j=0;j<currentUnit->axlesInUnit.size();j++) {
				auto currentAxle = currentUnit->axlesInUnit[j];
				if(currentAxle->machineForAxle!=nullptr) {
					currentAxle->axleWheelRPMOverMission.push_back(currentAxle->axleTireRPM);
					currentAxle->rpmAtAxleDifferentialOverMission.push_back(currentAxle->RPMToAxleDifferential);
				}
			}
		}
	}

	// Clear machine values
	for(int i=0;i<unitsInCombination.size();i++) {
		if(unitsInCombination[i]->bufferInUnit!=nullptr) {
			// Here, assign the minimumAllowedStateOfBuffer value to each battery
			if(i!=0) {
				unitsInCombination[i]->bufferInUnit->UpdateReferenceSoC(referenceSoC[0]);
			}
			auto currentUnit = unitsInCombination[i];
			for(int j=0;j<currentUnit->axlesInUnit.size();j++) {
				if(currentUnit->axlesInUnit[j]->machineForAxle!=nullptr) {
					currentUnit->axlesInUnit[j]->machineForAxle->ClearMachineValues();
				}
			}
		}
	}
}

void Combination::GetMissionProductivity() {

	// Calculate GVW and gross payload
	double GVW = 0;
	for(int i=0;i<unitsInCombination.size();i++) {
		GVW+=kerbUnitWeight[i];
	}
	double grossPayload = grossCombinationWeight-GVW;
	
	// Find additional masses due to electrification
	batteryPrice = 0;
	batteryMass = 0;

	for(int i=1;i<unitsInCombination.size();i++) {
		if(unitsInCombination[i]->bufferInUnit!=nullptr) {
			auto battery = unitsInCombination[i]->bufferInUnit;
			double depthOfDischarge = 1-battery->minimumStateOfBuffer;
			double batteryLife = 100000*pow(battery->referenceDepthOfDischarge/depthOfDischarge, 0.376)*
														exp(1-(depthOfDischarge/battery->referenceDepthOfDischarge));
			battery->numberOfBatteryReplacements = numberOfAnnualMissions*numberOfFirstOwnerYears/batteryLife;
		}
	}

	for(int i=1;i<unitsInCombination.size();i++) {
		if(unitsInCombination[i]->bufferInUnit!=nullptr) {
			int bufferSizeIndex = unitsInCombination[i]->bufferInUnit->bufferIndex;
			batteryPrice+=bufferCost[bufferSizeIndex]*unitsInCombination[i]->bufferInUnit->numberOfBatteryReplacements;
			batteryMass+=batteryMasses[bufferSizeIndex];
		}
	}

	electricPowertrainPrice = 0;
	electricPowertrainMass = 0;
	for(int i=1;i<unitsInCombination.size();i++){
		if(unitsInCombination[i]->bufferInUnit!=nullptr) {
			for(int j=0;j<unitsInCombination[i]->axlesInUnit.size();j++) {
				auto machineForAxle = unitsInCombination[i]->axlesInUnit[j]->machineForAxle;
				if(machineForAxle!=nullptr){
					int machineSizeIndex = machineForAxle->machineIndex;
					electricPowertrainPrice+=motorCost[machineSizeIndex];
					electricPowertrainMass+=motorMasses[machineSizeIndex];
				}
			}
		}
	}
	double massElectrification = batteryMass + electricPowertrainMass;
	
	// Calculate net payload
	double netPayload = grossPayload - massElectrification;
	
	// Mission revenues
	double missionRevenueSingle=revenuePerTonPerKM*(netPayload/1000)*(instantaneousLongitudinalPosition/1000);
	double missionRevenueAnnual=missionRevenueSingle*numberOfAnnualMissions;
	double missionRevenueN = missionRevenueAnnual*numberOfFirstOwnerYears;

	/*std::cout<<"grossCombinationWeight "<<grossCombinationWeight<<'\n';
	std::cout<<"GVW "<<GVW<<'\n';
	std::cout<<"grossPayload "<<grossPayload<<'\n';
	std::cout<<"batteryMass "<<batteryMass<<'\n';
	std::cout<<"electricPowertrainMass "<<electricPowertrainMass<<'\n';
	std::cout<<"massElectrification "<<massElectrification<<'\n';
	std::cout<<"netPayload "<<netPayload<<'\n';

	std::cout<<"revenuePerTonPerKM "<<revenuePerTonPerKM<<'\n';
	std::cout<<"instantaneousLongitudinalPosition "<<instantaneousLongitudinalPosition<<'\n';

	std::cout<<"missionRevenueSingle "<<missionRevenueSingle<<'\n';
	std::cout<<"numberOfAnnualMissions "<<numberOfAnnualMissions<<'\n';
	std::cout<<"missionRevenueAnnual "<<missionRevenueAnnual<<'\n';
	std::cout<<"missionRevenues "<<missionRevenueN<<'\n';//*/

	//-----------------------------------------------------------------------------------------------------------

	baseCombinationCost=0;
	for(int i=0;i<unitsInCombination.size();i++) {
		//std::cout<<"Cost Unit "<<i<<" "<<unitCosts[i]<<'\n';
		baseCombinationCost+= unitCosts[i];

	}
	//std::cout<<"baseCombinationCost (without powertrain premium) "<<baseCombinationCost<<'\n';
	int engineIndex = unitsInCombination[0]->axlesInUnit[1]->machineForAxle->machineIndex;
	if(engineIndex==0) {
		baseCombinationCost+=0;
	} else if(engineIndex==1) {
		baseCombinationCost+=powertrainPremium[0];
	} else {
		baseCombinationCost+=powertrainPremium[1];
	}
	double costFixedElectrification = batteryPrice + electricPowertrainPrice;
	double costFixed = baseCombinationCost + costFixedElectrification;

	/*std::cout<<"baseCombinationCost (Conventional) "<<baseCombinationCost<<'\n';
	std::cout<<"batteryPrice "<<batteryPrice<<'\n';
	std::cout<<"electricPowertrainPrice "<<electricPowertrainPrice<<'\n';
	std::cout<<"costFixedElectrification "<<costFixedElectrification<<'\n';
	std::cout<<"Total Fixed Cost (With electrification) "<<costFixed<<'\n';//*/
	
	//-----------------------------------------------------------------------------------------------------------

	fuelPrice = totalFuelConsumption*fuelPricePerLitre;

	electricityPrice = (totalElectricEnergyToBeRecharged/(3.6*1000000))*electricityPricePerkWh;	

	driverCosts = (totalTime/3600)*driverHourlyRate;

	double maintenanceCosts = otherCostRatios[0]*driverCosts;
	double tyreCosts = otherCostRatios[1]*driverCosts;
	double tollCosts = otherCostRatios[2]*driverCosts;
	
	double variableCostsMission=fuelPrice+electricityPrice+driverCosts+maintenanceCosts+tyreCosts+tollCosts;
	double variableCostsAnnual= variableCostsMission*numberOfAnnualMissions;
	double variableCostsN = variableCostsAnnual*numberOfFirstOwnerYears;

	/*std::cout<<"fuelPrice "<<fuelPrice<<'\n';
	std::cout<<"totalElectricEnergyToBeRecharged "<<totalElectricEnergyToBeRecharged<<'\n';
	std::cout<<"electricityPrice "<<electricityPrice<<'\n';
	std::cout<<"totalTime "<<totalTime<<'\n';
	std::cout<<"driverHourlyRate "<<driverHourlyRate<<'\n';
	std::cout<<"driverCosts "<<driverCosts<<'\n';
	std::cout<<"variableCostsMission "<<variableCostsMission<<'\n';
	std::cout<<"variableCostsAnnual "<<variableCostsAnnual<<'\n';
	std::cout<<"variableCostsN "<<variableCostsN<<'\n';//*/
	//-----------------------------------------------------------------------------------------------------------

	missionProductivity=missionRevenueN/(costFixed+variableCostsN);
	std::cout<<"Mission productivity "<<missionProductivity<<std::endl;
}

void Combination::WriteOutputFiles() {
	WriteCombinationOutput();
	WriteUnitOutput();
	WriteAxleLevelOutputs();
}

void Combination::WriteCombinationOutput() {
	MATFile* pmat;

	std::string fileLocation = "output/C";
	std::string fileTypeString = ".mat";
	std::string combinationFileNameString = fileLocation+fileTypeString;
	char* fileName = new char[combinationFileNameString.size()+1];
	std::copy(combinationFileNameString.begin(), combinationFileNameString.end(), fileName);
	fileName[combinationFileNameString.size()] = '\0';

	pmat = matOpen(fileName, "w");

	mxArray* dataSet;
	dataSet = mxCreateDoubleMatrix(1,positionOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), positionOverMission.data(), positionOverMission.size()*sizeof(double));
	matPutVariable(pmat, "positionOverMission", dataSet);
	mxDestroyArray(dataSet);

	dataSet = mxCreateDoubleMatrix(1,speedOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), speedOverMission.data(), speedOverMission.size()*sizeof(double));
	matPutVariable(pmat, "speedOverMission", dataSet);
	mxDestroyArray(dataSet);
	
	dataSet = mxCreateDoubleMatrix(1,accelerationDemandOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), accelerationDemandOverMission.data(), accelerationDemandOverMission.size()*sizeof(double));
	matPutVariable(pmat, "accelerationDemandOverMission", dataSet);
	mxDestroyArray(dataSet);
	
	dataSet = mxCreateDoubleMatrix(1,accelerationOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), accelerationOverMission.data(), accelerationOverMission.size()*sizeof(double));
	matPutVariable(pmat, "accelerationOverMission", dataSet);
	mxDestroyArray(dataSet);

	dataSet = mxCreateDoubleMatrix(1,aerodynamicDragForceOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), aerodynamicDragForceOverMission.data(), aerodynamicDragForceOverMission.size()*sizeof(double));
	matPutVariable(pmat, "aerodynamicDragForceOverMission", dataSet);
	mxDestroyArray(dataSet);
	
	dataSet = mxCreateDoubleMatrix(1,rollingResistanceForceOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), rollingResistanceForceOverMission.data(), rollingResistanceForceOverMission.size()*sizeof(double));
	matPutVariable(pmat, "rollingResistanceForceOverMission", dataSet);
	mxDestroyArray(dataSet);

	dataSet = mxCreateDoubleMatrix(1,gradientResistanceForceOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), gradientResistanceForceOverMission.data(), gradientResistanceForceOverMission.size()*sizeof(double));
	matPutVariable(pmat, "gradientResistanceForceOverMission", dataSet);
	mxDestroyArray(dataSet);

	dataSet = mxCreateDoubleMatrix(1,totalRoadResistanceForceOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), totalRoadResistanceForceOverMission.data(), totalRoadResistanceForceOverMission.size()*sizeof(double));
	matPutVariable(pmat, "totalRoadResistanceForceOverMission", dataSet);
	mxDestroyArray(dataSet);

	dataSet = mxCreateDoubleMatrix(1,requestedAccelerationForceOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), requestedAccelerationForceOverMission.data(), requestedAccelerationForceOverMission.size()*sizeof(double));
	matPutVariable(pmat, "requestedAccelerationForceOverMission", dataSet);
	mxDestroyArray(dataSet);
	
	dataSet = mxCreateDoubleMatrix(1,tractiveForceDemandOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), tractiveForceDemandOverMission.data(), tractiveForceDemandOverMission.size()*sizeof(double));
	matPutVariable(pmat, "tractiveForceDemandOverMission", dataSet);
	mxDestroyArray(dataSet);
	
	dataSet = mxCreateDoubleMatrix(1,achievedTractiveForceOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), achievedTractiveForceOverMission.data(), achievedTractiveForceOverMission.size()*sizeof(double));
	matPutVariable(pmat, "achievedTractiveForceOverMission", dataSet);
	mxDestroyArray(dataSet);

	/*dataSet = mxCreateDoubleMatrix(1,maximumTractionOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), maximumTractionOverMission.data(), maximumTractionOverMission.size()*sizeof(double));
	matPutVariable(pmat, "maximumTractionOverMission", dataSet);
	mxDestroyArray(dataSet);//*/

	dataSet = mxCreateDoubleMatrix(1,1,mxREAL);
	memcpy(mxGetPr(dataSet), &maximumGripLimitedTraction, sizeof(double));
	matPutVariable(pmat, "maximumGripLimitedTraction", dataSet);
	mxDestroyArray(dataSet);

	dataSet = mxCreateDoubleMatrix(1,1,mxREAL);
	memcpy(mxGetPr(dataSet), &combinationStartability, sizeof(double));
	matPutVariable(pmat, "combinationStartability", dataSet);
	mxDestroyArray(dataSet);

	dataSet = mxCreateDoubleMatrix(1,1,mxREAL);
	memcpy(mxGetPr(dataSet), &missionProductivity, sizeof(double));
	matPutVariable(pmat, "missionProductivity", dataSet);
	mxDestroyArray(dataSet);

	dataSet = mxCreateDoubleMatrix(1,1,mxREAL);
	memcpy(mxGetPr(dataSet), &tractionLimitCount, sizeof(double));
	matPutVariable(pmat, "tractionLimitCount", dataSet);
	mxDestroyArray(dataSet);

	dataSet = mxCreateDoubleMatrix(1,1,mxREAL);
	memcpy(mxGetPr(dataSet), &powerLimitCount, sizeof(double));
	matPutVariable(pmat, "powerLimitCount", dataSet);
	mxDestroyArray(dataSet);
	
	dataSet = mxCreateDoubleMatrix(1,powerDemandOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), powerDemandOverMission.data(), powerDemandOverMission.size()*sizeof(double));
	matPutVariable(pmat, "powerDemandOverMission", dataSet);
	mxDestroyArray(dataSet);
	
	dataSet = mxCreateDoubleMatrix(1,testVector.size(),mxREAL);
	memcpy(mxGetPr(dataSet), testVector.data(), testVector.size()*sizeof(double));
	matPutVariable(pmat, "testVector", dataSet);
	mxDestroyArray(dataSet);
	
	dataSet = mxCreateDoubleMatrix(1,achievedPowerOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), achievedPowerOverMission.data(), achievedPowerOverMission.size()*sizeof(double));
	matPutVariable(pmat, "achievedPowerOverMission", dataSet);
	mxDestroyArray(dataSet);
	
	dataSet = mxCreateDoubleMatrix(1,operatingModeOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), operatingModeOverMission.data(), operatingModeOverMission.size()*sizeof(double));
	matPutVariable(pmat, "operatingModeOverMission", dataSet);
	mxDestroyArray(dataSet);
	
	dataSet = mxCreateDoubleMatrix(1,fuelConsumptionOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), fuelConsumptionOverMission.data(), fuelConsumptionOverMission.size()*sizeof(double));
	matPutVariable(pmat, "fuelConsumptionOverMission", dataSet);
	mxDestroyArray(dataSet);
	
	dataSet = mxCreateDoubleMatrix(1,chargeConsumptionOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), chargeConsumptionOverMission.data(), chargeConsumptionOverMission.size()*sizeof(double));
	matPutVariable(pmat, "chargeConsumptionOverMission", dataSet);
	mxDestroyArray(dataSet);

	matClose(pmat);
}

void Combination::WriteUnitOutput() {
	for(int i=0;i<unitsInCombination.size();i++) { 
		MATFile* pmat;
		char* fileName = GetUnitOutputFileName(i);
		pmat = matOpen(fileName, "w");

		mxArray* dataSet;
		dataSet = mxCreateDoubleMatrix(1,unitsInCombination[i]->unitTractiveForceOverMission.size(),mxREAL);
		memcpy(mxGetPr(dataSet), unitsInCombination[i]->unitTractiveForceOverMission.data(), unitsInCombination[i]->unitTractiveForceOverMission.size()*sizeof(double));
		matPutVariable(pmat, "unitTractiveForceOverMission", dataSet);
		mxDestroyArray(dataSet);

		dataSet = mxCreateDoubleMatrix(1,unitsInCombination[i]->unitTractiveForceDemandOverMission.size(),mxREAL);
		memcpy(mxGetPr(dataSet), unitsInCombination[i]->unitTractiveForceDemandOverMission.data(), unitsInCombination[i]->unitTractiveForceDemandOverMission.size()*sizeof(double));
		matPutVariable(pmat, "unitTractiveForceDemandOverMission", dataSet);
		mxDestroyArray(dataSet);
		
		matClose(pmat);
	}
}

char* Combination::GetUnitOutputFileName(int unitIndex) {
	std::string fileLocation = "output/U";
	std::stringstream unitIndexStringStream;
	unitIndexStringStream << unitIndex;
	std::string unitIndexString = unitIndexStringStream.str();
	std::string fileTypeString = ".mat";
	std::string unitFileNameString = fileLocation+unitIndexString+fileTypeString;
	char* unitFileName = new char[unitFileNameString.size()+1];
	std::copy(unitFileNameString.begin(), unitFileNameString.end(), unitFileName);
	unitFileName[unitFileNameString.size()] = '\0';
	std::cout<<unitFileName<<std::endl;
	return unitFileName;
}

void Combination::WriteAxleLevelOutputs() {
	for(int i=0;i<unitsInCombination.size();i++) {
		auto currentUnit = unitsInCombination[i];
		if(currentUnit->bufferInUnit!=nullptr) {
			WriteBufferOutput(i, currentUnit->bufferInUnit);
			for(int j=0;j<currentUnit->axlesInUnit.size();j++) {
				auto currentAxle = currentUnit->axlesInUnit[j];
				if(currentAxle->machineForAxle!=nullptr) {
					WriteAxleOutput(i,j,currentAxle);
					WriteMachineOutput(i,j,currentAxle->machineForAxle);
					//WriteTransmissionOutput(i,j,currentAxle->machineForAxle);
					WriteTransmissionOutput(i,j,currentAxle->machineForAxle->transmissionForMachine);
				}
			}
		}
	}
}

void Combination::WriteBufferOutput(int unitIndex, std::shared_ptr<Buffer> buffer) {
	MATFile* pmat;
	char* fileName = GetBufferOutputFileName(unitIndex);
	pmat = matOpen(fileName, "w");

	mxArray* dataSet;
	dataSet = mxCreateDoubleMatrix(1,buffer->bufferLevelOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), buffer->bufferLevelOverMission.data(), buffer->bufferLevelOverMission.size()*sizeof(double));
	matPutVariable(pmat, "bufferLevelOverMission", dataSet);
	mxDestroyArray(dataSet);

	dataSet = mxCreateDoubleMatrix(1,buffer->bufferPowerDemandOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), buffer->bufferPowerDemandOverMission.data(), buffer->bufferPowerDemandOverMission.size()*sizeof(double));
	matPutVariable(pmat, "bufferPowerDemandOverMission", dataSet);
	mxDestroyArray(dataSet);
	
	dataSet = mxCreateDoubleMatrix(1,buffer->bufferAvailabilityRatioOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), buffer->bufferAvailabilityRatioOverMission.data(), buffer->bufferAvailabilityRatioOverMission.size()*sizeof(double));
	matPutVariable(pmat, "bufferAvailabilityRatioOverMission", dataSet);
	mxDestroyArray(dataSet);

	dataSet = mxCreateDoubleMatrix(1,buffer->stateOfBufferOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), buffer->stateOfBufferOverMission.data(), buffer->stateOfBufferOverMission.size()*sizeof(double));
	matPutVariable(pmat, "stateOfBufferOverMission", dataSet);
	mxDestroyArray(dataSet);

	dataSet = mxCreateDoubleMatrix(1,buffer->referenceSoCOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), buffer->referenceSoCOverMission.data(), buffer->referenceSoCOverMission.size()*sizeof(double));
	matPutVariable(pmat, "referenceSoCOverMission", dataSet);
	mxDestroyArray(dataSet);

	matClose(pmat);
}

char* Combination::GetBufferOutputFileName(int unitIndex) {
	std::string fileLocation = "output/U";
	std::stringstream unitIndexStringStream;
	unitIndexStringStream << unitIndex;
	std::string unitIndexString = unitIndexStringStream.str();
	std::string fileTypeString = "B.mat";
	std::string bufferFileNameString = fileLocation+unitIndexString+fileTypeString;
	char* bufferFileName = new char[bufferFileNameString.size()+1];
	std::copy(bufferFileNameString.begin(), bufferFileNameString.end(), bufferFileName);
	bufferFileName[bufferFileNameString.size()] = '\0';
	std::cout<<bufferFileName<<std::endl;
	return bufferFileName;	
}

void Combination::WriteAxleOutput(int unitIndex, int axleIndex, std::shared_ptr<Axle> axle) {
	MATFile* pmat;
	char* fileName = GetAxleOutputFileName(unitIndex, axleIndex);
	pmat = matOpen(fileName, "w");

	mxArray* dataSet;
	dataSet = mxCreateDoubleMatrix(1,axle->axleTractiveForceOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), axle->axleTractiveForceOverMission.data(), axle->axleTractiveForceOverMission.size()*sizeof(double));
	matPutVariable(pmat, "axleTractiveForceOverMission", dataSet);
	mxDestroyArray(dataSet);

	dataSet = mxCreateDoubleMatrix(1,axle->torqueAtAxleDifferentialOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), axle->torqueAtAxleDifferentialOverMission.data(), axle->torqueAtAxleDifferentialOverMission.size()*sizeof(double));
	matPutVariable(pmat, "torqueAtAxleDifferentialOverMission", dataSet);
	mxDestroyArray(dataSet);
	
	dataSet = mxCreateDoubleMatrix(1,axle->rpmAtAxleDifferentialOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), axle->rpmAtAxleDifferentialOverMission.data(), axle->rpmAtAxleDifferentialOverMission.size()*sizeof(double));
	matPutVariable(pmat, "rpmAtAxleDifferentialOverMission", dataSet);
	mxDestroyArray(dataSet);

	dataSet = mxCreateDoubleMatrix(1,axle->axleWheelRPMOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), axle->axleWheelRPMOverMission.data(), axle->axleWheelRPMOverMission.size()*sizeof(double));
	matPutVariable(pmat, "axleWheelRPMOverMission", dataSet);
	mxDestroyArray(dataSet);

	matClose(pmat);
}

char* Combination::GetAxleOutputFileName(int unitIndex, int axleIndex) {
	std::string fileLocation = "output/U";
	std::stringstream unitIndexStringStream;
	unitIndexStringStream << unitIndex;
	std::string unitIndexString = unitIndexStringStream.str();
	std::string axleIndicator = "A";
	std::stringstream axleIndexStringStream;
	axleIndexStringStream << axleIndex;
	std::string axleIndexString = axleIndexStringStream.str();
	std::string fileTypeString = ".mat";
	std::string axleFileNameString = fileLocation+unitIndexString+axleIndicator+axleIndexString+fileTypeString;
	char* axleFileName = new char[axleFileNameString.size()+1];
	std::copy(axleFileNameString.begin(), axleFileNameString.end(), axleFileName);
	axleFileName[axleFileNameString.size()] = '\0';
	std::cout<<axleFileName<<std::endl;
	return axleFileName;	
}

void Combination::WriteMachineOutput(int unitIndex, int axleIndex, std::shared_ptr<Machine> machine) {
	MATFile* pmat;
	char* fileName = GetMachineOutputFileName(unitIndex, axleIndex);
	pmat = matOpen(fileName, "w");

	mxArray* dataSet;
	dataSet = mxCreateDoubleMatrix(1,machine->machineRPMOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), machine->machineRPMOverMission.data(), machine->machineRPMOverMission.size()*sizeof(double));
	matPutVariable(pmat, "machineRPMOverMission", dataSet);
	mxDestroyArray(dataSet);

	
	dataSet = mxCreateDoubleMatrix(1,machine->machineTorqueOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), machine->machineTorqueOverMission.data(), machine->machineTorqueOverMission.size()*sizeof(double));
	matPutVariable(pmat, "machineTorqueOverMission", dataSet);
	mxDestroyArray(dataSet);

	dataSet = mxCreateDoubleMatrix(1,machine->machineEfficiencyOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), machine->machineEfficiencyOverMission.data(), machine->machineEfficiencyOverMission.size()*sizeof(double));
	matPutVariable(pmat, "machineEfficiencyOverMission", dataSet);
	mxDestroyArray(dataSet);

	matClose(pmat);
}

char* Combination::GetMachineOutputFileName(int unitIndex, int axleIndex) {
	std::string fileLocation = "output/U";
	std::stringstream unitIndexStringStream;
	unitIndexStringStream << unitIndex;
	std::string unitIndexString = unitIndexStringStream.str();
	std::string machineIndicator = "M";
	std::stringstream axleIndexStringStream;
	axleIndexStringStream << axleIndex;
	std::string axleIndexString = axleIndexStringStream.str();
	std::string fileTypeString = ".mat";
	std::string machineFileNameString = fileLocation+unitIndexString+machineIndicator+axleIndexString+fileTypeString;
	char* machineFileName = new char[machineFileNameString.size()+1];
	std::copy(machineFileNameString.begin(), machineFileNameString.end(), machineFileName);
	machineFileName[machineFileNameString.size()] = '\0';
	std::cout<<machineFileName<<std::endl;
	return machineFileName;	
}

void Combination::WriteTransmissionOutput(int unitIndex, int axleIndex, std::shared_ptr<Transmission> transmission) {
	MATFile* pmat;
	char* fileName = GetTransmissionOutputFileName(unitIndex, axleIndex);
	pmat = matOpen(fileName, "w");

	mxArray* dataSet;
	dataSet = mxCreateDoubleMatrix(1,transmission->gearIndexOverMission.size(),mxREAL);
	memcpy(mxGetPr(dataSet), transmission->gearIndexOverMission.data(), transmission->gearIndexOverMission.size()*sizeof(double));
	matPutVariable(pmat, "gearIndexOverMission", dataSet);

	matClose(pmat);
}

char* Combination::GetTransmissionOutputFileName(int unitIndex, int axleIndex) {
	std::string fileLocation = "output/U";
	std::stringstream unitIndexStringStream;
	unitIndexStringStream << unitIndex;
	std::string unitIndexString = unitIndexStringStream.str();
	std::string transmissionIndicator = "T";
	std::stringstream axleIndexStringStream;
	axleIndexStringStream << axleIndex;
	std::string axleIndexString = axleIndexStringStream.str();
	std::string fileTypeString = ".mat";
	std::string transmissionFileNameString = fileLocation+unitIndexString+transmissionIndicator+axleIndexString+fileTypeString;
	char* transmissionFileName = new char[transmissionFileNameString.size()+1];
	std::copy(transmissionFileNameString.begin(), transmissionFileNameString.end(), transmissionFileName);
	transmissionFileName[transmissionFileNameString.size()] = '\0';
	std::cout<<transmissionFileName<<std::endl;
	return transmissionFileName;	
}

Combination::~Combination(void) {
	//std::cout<<"End of generation. Vehicle destroyed."<<std::endl;
}