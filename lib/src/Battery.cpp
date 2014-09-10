#include "Battery.h"

Battery::Battery(int currentUnitIndex, Genes bufferGenes) : Buffer(currentUnitIndex, bufferGenes) {
	double unitBufferEnergy = maximumBufferLevel*openCircuitVoltage*0.001/3600;	//kWh
}

void Battery::RunBuffer(double powerDemand, double operatingEfficiency) {
  //std::cout<<"Running battery"<<'\n';
  double timeInterval = 1.0;
  instantaneousBufferDemand = powerDemand*timeInterval/(openCircuitVoltage*operatingEfficiency);
  /*std::cout<<"powerDemand "<<powerDemand<<" W"<<'\n'<<"operatingEfficiency "<<operatingEfficiency<<'\n';
  std::cout<<"instantaneousBufferDemand in Coulombs "<<instantaneousBufferDemand<<" Coulombs"<<std::endl;//
  if(isinf(instantaneousBufferDemand)!=0) {
    std::cout<<"WHAT THE FUCK!"<<std::endl;
  }//*/
  instantaneousBufferLevel -= instantaneousBufferDemand;
  if(instantaneousBufferLevel<referenceStateOfBuffer*maximumBufferLevel) { // While using energy, avoid going below min. charge
  	//std::cout<<"Buffer level reset to refSoc*maxBufferLevel to avoid going below min allowed SoC"<<'\n';
    instantaneousBufferLevel=referenceStateOfBuffer*maximumBufferLevel;
  }
  if(instantaneousBufferLevel>maximumBufferLevel) { // Upon recharging, avoid exceeding max. charge
  	instantaneousBufferLevel=maximumBufferLevel;
  }
  //std::cout<<"instantaneousBufferLevel in Coulombs "<<instantaneousBufferLevel<<" Coulombs"<<std::endl;
  endOfStepPowerDemand+=powerDemand;
}

Battery::~Battery() {
}