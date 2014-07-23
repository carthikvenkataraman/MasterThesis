#ifndef _MACHINE_H
#define _MACHINE_H
#define _USE_MATH_DEFINES


#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <memory>
#include <sstream>
#include <cstring>
#include "math.h"
#include "Buffer.h"
#include "Transmission.h"
#include "/home/karthik/MATLAB/R2013b/extern/include/mat.h"

typedef std::vector<int> Genes;

class Machine {
    int machineTypeIndex;
    std::string machineName, completeMachineName;
    Genes machineGenes;
    char* machineFileName;
    std::shared_ptr<Buffer> bufferForMachine;
  
    void GetMachineIndex();
    void GetMachineFileName();
    void LoadMachine();
    double GetMachineEfficiency(double , double );

  public:
    int machineIndex, machineAxleIndex;
    double unitPowertrainPrice;
    double unitPowertrainMass;

    std::shared_ptr<Transmission> transmissionForMachine;

    std::vector<double> machineRPMRange, machineTorqueRange;
    std::vector<double> possibleMachineSpeeds, possibleMachineTorques;
    std::vector<double> permittedMachineSpeeds, permittedMaximumTorques, permittedGearRatios;
    std::vector<double> permittedMaximumBrakePower;
    std::vector<double> permittedMaximumRegenerativePower;   
    std::vector<double> machineMaximumBrakeTorque, machineMaximumRegenerationTorque, rpmRangeForMaximumBrakeTorque;
    std::vector<std::vector<double>> machineEfficiencyData;

    double maximumInstantaneousMachineTorque, maximumInstantaneousMachinePower;
    double maximumInstantaneousRegenerativeMachineTorque, maximumInstantaneousRegenerativeMachinePower;
    double maximumInstantaneousPowertrainTorque; // After transmission
    double maximumInstantaneousRegenerationPowertrainTorque; // After transmission
    int gearNumberAtMaximumTorque, gearNumberAtMaximumRegenerativeTorque;

    double bestOperatingTorque, idlingTorque;
    double bestOperatingRPM, idlingRPM;
    double bestOperatingBSFC;
    int gearIndexAtGivenSpeed;

    double machineRPM, machineTorque, machineOperatingEfficiency;
    std::vector<double> machineRPMOverMission, machineTorqueOverMission, machineEfficiencyOverMission, gearIndexOverMission;
    //std::vector<int> gearIndexOverMission;


    Machine(int , Genes , std::shared_ptr<Buffer> , int );
    void PrintMachineData(std::vector<std::vector<double>>& );
    void PrintMachineData(std::vector<double>& );
    std::string ReturnMachineName();
    
    void GetPossibleMachineSpeeds(double );
    void GetPossibleMachineTorques(double );

    void GetMaximumInstantaneousTorque();
    double GetMaximumBrakeTorque(double );

    void GetMaximumInstantaneousRegenerativeTorque();
    double GetMaximumRegenerativeTorque(double );

    void RunMachine(double , double);
    void ClearMachineValues();

    ~Machine(); 
};

#endif