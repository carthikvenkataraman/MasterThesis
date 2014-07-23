#ifndef _AXLE_H
#define _AXLE_H
#define _USE_MATH_DEFINES

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <math.h>
#include "Machine.h"

typedef std::vector<int> Genes;
typedef std::vector<Genes> UnitGene;
typedef std::vector<UnitGene> Individual;

class Axle {
   int axleIndex;
   double axleLoad;
   int axlePropulsionFlag;
   int axleMachineCount;

   double coefficientOfFriction;   
   
   double instantaneousBufferDemand;
   double tractionDistributionRatio;

public:
   double axleFinalDriveRatio;
   double gripLimitedTraction;
   double axleTireRadius, axleTireRPM;
   double tractiveForceAtWheels;
   std::shared_ptr<Machine> machineForAxle;

   double axleTractiveForceDemand, axleTractiveForce; 
   double torqueToAxleDifferential, RPMToAxleDifferential;

   std::vector<double> torqueAtAxleDifferentialOverMission, rpmAtAxleDifferentialOverMission;
   std::vector<double> axleWheelRPMOverMission, axleTractiveForceOverMission;

   Axle(int , int , std::shared_ptr<Machine> );
   int GetAxleIndex();
   double GetAxleLoad();
   int GetAxlePropulsionFlag();
   double GetAxleCoefficientOfFriction();
   void UpdateAxleWheelSpeed(double );
   void DefineAxleParameters(double , double , double , double);
   std::shared_ptr<Machine> ReturnMachineForAxle();
   int GetAxleMachineCount();

   double GetTractiveForceAtWheels(double );
   void GetTorqueToAxleDifferential(double );
   ~Axle();

   void UpdateAxleTractionDistributionRatio(double );
};

#endif