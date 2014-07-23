#ifndef _UNIT_H
#define _UNIT_H

#include <iostream>
#include <vector>
#include <cmath>
#include "Axle.h"
#include "Buffer.h"
#include "Battery.h"
#include "FuelTank.h"
#include "Machine.h"
#include "ElectricMotor.h"
#include "CombustionEngine.h"
#include "Transmission.h"
#include "MechanicalGearbox.h"
#include "MotorTransmission.h"

typedef std::vector<int> Genes;
typedef std::vector<Genes> UnitGene;
typedef std::vector<UnitGene> Individual;

class Unit {
   int unitIndex;
   int numberOfAxles, numberOfDrivenAxles; 

   Genes machineGenes;
   Genes bufferGenes;
   Genes axleGenes;

   double grossUnitWeight;
   double tractionSplitRatio;

   std::vector<std::shared_ptr<Machine>> machinesInUnit;
   std::vector<std::shared_ptr<Transmission>> transmissionsInUnit;

   void GetNumberOfDrivenAxles();
   void BuildUnitBuffer();
   //void BuildTractorPowertrain();
   void BuildTrailerPowertrain();
   void BuildTractorAxles();
   void BuildTrailerAxles();
   
   //void GetPossibleMachineSpeeds(int , std::shared_ptr<Machine>& , double );
   void GetAchievedTractiveForce();

public:
   std::shared_ptr<Buffer> bufferInUnit = nullptr; 

   double unitTireRadius, unitCoefficientOfFriction;
   std::vector<double> loadEachAxle;
   double gravitationalAcceleration;
   
   double instantaneousPower, demandedPower, maximumInstantaneousPower, maximumRegenerativePower, gripLimitedTractivePower;
   double instantaneousTractiveForce=0, demandedTractiveForce, maximumInstantaneousTractiveForce, maximumInstantaneousRegenerativeTractiveForce;
   double unitInstantaneousSpeed;
   double instantaneousPowerAvailability, instantaneousRegenerativeCapacity;
   double tractionDistributionRatio, regenerationDistributionRatio;

   int unitBufferCount;
   int unitEngineCount, unitElectricMachineCount, unitTransmissionCount;
   int unitInstantaneousFuelConsumption, unitInstantaneousChargeConsumption;

   std::vector<std::shared_ptr<Axle>> axlesInUnit;

   std::vector<double> unitTractiveForceDemandOverMission, unitTractiveForceOverMission;

   Unit(int , UnitGene); 
   int GetNumberOfAxles();
   void AssignAxleParameters();
   void GetMaximumTractivePower(double );
   void GetMaximumRegenerativePower(double );
   double GetUnitPowerAvailability(); 
   double GetUnitRegenerativeCapacity();                                                                             
   void RunUnit(double );
   ~Unit();

   double GetUnitPropulsionChargeAvailabilityRatio();
   double GetUnitRegenerationChargeAvailabilityRatio();
   void UpdateUnitChargeAvailabilityRatio();
};

#endif