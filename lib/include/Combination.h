#ifndef _COMBINATION_H
#define _COMBINATION_H

#include <string.h>
#include <numeric>
#include <cmath>
#include "Unit.h"
#include "Buffer.h"
#include "Axle.h"
#include "Machine.h"
#include "Transmission.h"
#include "/home/karthik/MATLAB/R2014a/extern/include/mat.h"

typedef std::vector<int> Genes;
typedef std::vector<Genes> UnitGene;
typedef std::vector<UnitGene> Individual;

class Combination { // Access control pending
	int numberOfUnits;
	char* inputFileName;

	// Mission parameters
	std::vector<std::vector<double>> missionData;
	std::vector<double> kerbUnitWeight, payloadEachUnit;
	std::vector<std::vector<double>> unitAxleLoads;
	std::vector<double> targetSpeed, longitudinalPosition, roadGradientInRadians,
		unitCosts, motorCost, powerElectronicsCost, bufferCost, mechanicalDriveCost, powertrainPremium, 
	    annuitPercentage, coefficientOfFriction, tireRadiusEachUnit, referenceSoC, motorMasses, batteryMasses, otherCostRatios;

	double gravitationalAcceleration, frontalArea, 
				aerodynamicDragCoefficient, densityOfAir, grossCombinationWeight, rollingResistanceCoefficient;
	double semiTrailerCost,dollyCost;
	double driverHourlyRate, fuelPricePerLitre;
	double totalPayload, electricityPricePerkWh, revenuePerTonPerKM;
	double numberOfAnnualMissions;
	int numberOfFirstOwnerYears;

	// Iteration time step
	double timeInterval;

	// Instantaneous vehicle parameters - drag force, rolling resistance, force due to gradient, traction
	double instantaneousPowerDemand, maximumInstantaneousPower; 
	double maximumPowerFromTrailingUnits, maximumTractionFromTrailingUnits;

	// Rotational inertia parameters
	double tireInertia, axleInertia, propShaftInertia, transmissionInertia, clutchInertia;
	double equivalentTireMassRatio, equivalentTireMass, equivalentAxleMassRatio, equivalentAxleMass, 
			equivalentPropShaftMassRatio, equivalentPropShaftMass, equivalentTransmissionMassRatio, equivalentTransmissionMass,
			equivalentClutchMassRatio, equivalentClutchMass;
	double equivalentMassDueToInertia, effectiveGrossCombinationWeight;

	double aerodynamicDragForce, rollingResistanceForce, gradientResistanceForce, 
											totalRoadResistanceForce, requestedAccelerationForce;
	double totalRequestedTractiveForce, maximumGripLimitedTraction, maximumInstantaneousTraction;
	double tractiveForceToBeDivided, instantaneousTractiveForce;
	double requestedAcceleration, instantaneousAcceleration;
	double instantaneousTargetSpeed, instantaneousSpeed;
	double instantaneousLongitudinalPosition;
	double instantaneousAltitude;
	double instantaneousGradient;
	double instantaneousReferenceSoC;
	int currentPositionIndex;
	double tractionLimitCount=0, powerLimitCount=0;
	std::vector<double> operatingModeOverMission;

	// Vehicle parameters logged over the whole mission
	std::vector<double> positionOverMission, altitudeOverMission, speedOverMission, accelerationOverMission, accelerationDemandOverMission,
						tractiveForceDemandOverMission, achievedTractiveForceOverMission, powerDemandOverMission,
						achievedPowerOverMission, fuelConsumptionOverMission, chargeConsumptionOverMission;
	std::vector<double> aerodynamicDragForceOverMission, rollingResistanceForceOverMission, gradientResistanceForceOverMission, 
											totalRoadResistanceForceOverMission, requestedAccelerationForceOverMission;
	// Mission outputs - fuel and charge consumption, time taken, 
	double cumulativeFuelConsumption, instantaneousFuelConsumption;
	double cumulativeChargeConsumption, instantaneousChargeConsumption;
	double totalTime=0;
	
	// Vehicle productivity
	double baseTractorCost;
	double baseCombinationCost;

	double batteryPrice;
	double batteryMass;

	double electricPowertrainPrice;
	double electricPowertrainMass;

	double totalFuelConsumption, totalElectricEnergyToBeRecharged;
	double fuelPrice, electricityPrice;

	double reducedPayload, revenuePerUnitPayload;
	double driverCosts;
	double missionCosts, missionRevenues;

	int testCount=0;

	std::vector<double> testVector;

	// Combination methods	
	void AssignMissionData();
	void AssignUnitParameters();
	void CheckUnitParameters();
	void PrintMissionDataEntry(std::vector<double>& );

	void UpdateAltitude();
	void UpdateVehicleEnergy();

	void GetEquivalentMassDueToInertia();
	void GetMaximumGripLimitedTraction();
	double GetTractorTraction(double , double );
	void SetTractionDistributionRatios();
	void SetRegenerationDistributionRatios();
	void AssignAxleTractionDistributionRatios();
	void AssignTractiveForceAmongUnits();
	double AssignTractionAmongElectricMachines(double );

	void WriteOutputFiles();	
	void WriteCombinationOutput();
	void WriteUnitOutput();
	char* GetUnitOutputFileName(int );
	void WriteAxleLevelOutputs();
	void WriteBufferOutput(int , std::shared_ptr<Buffer> );
	char* GetBufferOutputFileName(int );
	void WriteAxleOutput(int , int , std::shared_ptr<Axle> );
	char* GetAxleOutputFileName(int , int );
	void WriteMachineOutput(int , int , std::shared_ptr<Machine> );
	char* GetMachineOutputFileName(int , int );
	void WriteTransmissionOutput(int , int , std::shared_ptr<Transmission> );
	char* GetTransmissionOutputFileName(int , int );

public:
	double combinationStartability;
	double missionProductivity;
	// Units in current combination
	std::vector<std::shared_ptr<Unit>> unitsInCombination;
	
	Combination(Individual individual, std::vector<std::vector<double>> missionData);
	void RunMission();
	double GetMaximumTraction(double );
	double GetMaximumRegenerativeTraction(double );
	void GetInstantaneousTractiveForce();
	void CalculateProductivity();
	void GetInstantaneousLongitudinalPosition();
	void GetInstantaneousSpeed();
	void GetCumulativeFuelConsumption();
	void GetCumulativeChargeConsumption();
	void GetCombinationStartability();
	void GetMissionProductivity();
	~Combination();
};

#endif