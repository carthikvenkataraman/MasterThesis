// Takes individual chromosome, creates a vehicle, runs it on the given mission, 
// calculates productivity for the mission and returns it to the GA.

#include <iostream>
#include <vector>
#include "Combination.h"
#include "Unit.h"

double VehicleMissionProductivity (Individual individualChromosome) {
   Combination* combination = Combination Vehicle(individualChromosome);  // Contains data such as speed, acceleration, GCW,
		// fuel consumed, charge consumed etc. (global vehicle parameters) along with unit object data
   char* drivingCycleFileName;
   std::vector<std::vector<double>> missionData = LoadMissionData(drivingCycleFileName);
   std::vector<double> vehicleMissionOutputs = combination->RunMission(missionData);
   vehicleMissionProductivity = CalculateVehicleProductivity(vehicleMissionOutputs);
   return vehicleMissionProductivity;
}
