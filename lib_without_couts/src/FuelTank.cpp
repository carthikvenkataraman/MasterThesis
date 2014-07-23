#include "FuelTank.h"

FuelTank::FuelTank(int currentUnitIndex, Genes bufferGenes) : Buffer(currentUnitIndex, bufferGenes) {
}

void FuelTank::RunBuffer(double powerDemand, double operatingEfficiency) {
	//std::cout<<"Fuel requested from fuel tank to provide "<<powerDemand/1000<<" kW. Machine efficiency is "<<operatingEfficiency<<" g/J"<<std::endl;
	double bsfcInGramsPerJoule = operatingEfficiency;	// grams per Joule
	double timeInterval = 1.0;
	double instantaneousEnergyDemand = powerDemand*timeInterval;	// Joules
	instantaneousBufferDemandInGrams = instantaneousEnergyDemand*bsfcInGramsPerJoule;	// grams
	//std::cout<<"instantaneousBufferDemand in grams "<<instantaneousBufferDemandInGrams<<" grams"<<std::endl;
	instantaneousBufferDemand = instantaneousBufferDemandInGrams*0.001/densityOfFuel*1.05;	// litres
	//std::cout<<"instantaneousBufferDemand in litres "<<instantaneousBufferDemand<<" litres"<<std::endl;
	instantaneousBufferLevel -= instantaneousBufferDemand; 
	//std::cout<<"instantaneousBufferLevel in litres "<<instantaneousBufferLevel<<" litres"<<std::endl;
   endOfStepPowerDemand+=powerDemand;      
}

FuelTank::~FuelTank() {
}