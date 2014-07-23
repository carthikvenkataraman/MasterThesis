#include "Battery.h"

Battery::Battery(int currentUnitIndex, Genes bufferGenes) : Buffer(currentUnitIndex, bufferGenes) {
	double unitBufferEnergy = maximumBufferLevel*openCircuitVoltage*0.001/3600;	//kWh
	unitBatteryCost = unitBufferEnergy*batteryCostPerkWh;
	unitBatteryMass = unitBufferEnergy*batteryMassPerkWh;
}

void Battery::RunBuffer(double powerDemand, double operatingEfficiency) {
  double timeInterval = 1.0;
  instantaneousBufferDemand = powerDemand*timeInterval/(openCircuitVoltage*operatingEfficiency);
  //std::cout<<"instantaneousBufferDemand in Coulombs "<<instantaneousBufferDemand<<" Coulombs"<<std::endl; 
  instantaneousBufferLevel -= instantaneousBufferDemand;
  if(instantaneousBufferLevel<minimumAllowedStateOfBuffer*maximumBufferLevel) { // While using energy, avoid going below min. charge
  	instantaneousBufferLevel=minimumAllowedStateOfBuffer*maximumBufferLevel;
  }
  if(instantaneousBufferLevel>maximumBufferLevel) { // Upon recharging, avoid exceeding max. charge
  	instantaneousBufferLevel=maximumBufferLevel;
  }
  //std::cout<<"instantaneousBufferLevel in Coulombs "<<instantaneousBufferLevel<<" Coulombs"<<std::endl;
  endOfStepPowerDemand+=powerDemand;
}

Battery::~Battery() {
}