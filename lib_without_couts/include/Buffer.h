#ifndef _BUFFER_H
#define _BUFFER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <memory>
#include <sstream>
#include <cstring>

class Buffer {
      int unitIndex, unitBufferTypeIndex;
      std::string bufferName, completeBufferName;
      std::vector<int> bufferGenes;
      int bufferIndex;              
      char* bufferFileName;
      std::vector<double> bufferData;

      std::vector<double> bufferEfficiency;  
      double maximumPower;

      void GetBufferIndex();
      void GetBufferFileName();
      void LoadBuffer();
      void UpdateBufferAvailabilityRatio();

   public:
      double maximumBufferLevel, minimumBufferLevel, instantaneousBufferLevel;//, bufferCapacity; 
      double minimumAllowedStateOfBuffer, maximumStateOfBuffer;         
      double stateOfBuffer;
      double openCircuitVoltage;
      double instantaneousBufferDemand;
      double bufferAvailabilityRatio;

      double endOfStepPowerDemand;
      std::vector<double> bufferLevelOverMission, bufferPowerDemandOverMission;
      std::vector<double> bufferAvailabilityRatioOverMission, stateOfBufferOverMission;

      double unitBatteryCost;
      double unitBatteryMass;
      
      Buffer(int , std::vector<int> );
      void GetStateOfBuffer();
      double GetBufferAvailabilityRatio();
      std::string ReturnBufferName();
      virtual void RunBuffer(double , double );
      ~Buffer();
};

#endif