#ifndef _TRANSMISSION_H
#define _TRANSMISSION_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <memory>
#include <sstream>
#include <cstring>
#include "/home/karthik/MATLAB/R2013b/extern/include/mat.h"

typedef std::vector<int> Genes;

class Transmission {
    int transmissionTypeIndex;
    std::string transmissionName;
    char* transmissionFileName;
    std::vector<std::vector<double>> transmissionData;
  
    void GetTransmissionIndex();
    void GetTransmissionFileName();
    void LoadTransmission();
    void PrintTransmissionData(std::vector<double>& transmissionDataEntry);

  public:
    std::vector<double> gearRatios, gearInertias, gearEfficiency;
    double transmissionInputTorque, transmissionOutputTorque;
    double transmissionInputRPM, transmissionOutputRPM;
    double operatingGearRatio;
    int operatingGearIndex=0;
    std::vector<double> gearIndexOverMission;


    Transmission(int );
    double GetOutputTorque(double , int );
    std::string ReturnTransmissionName();
    ~Transmission();
};

#endif