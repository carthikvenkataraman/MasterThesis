#ifndef _BATTERY_H
#define _BATTERY_H

#include "Buffer.h"

typedef std::vector<int> Genes;

class Battery : public Buffer {

public:
	double batteryMassPerkWh=10;		// kg/kWh
	double batteryCostPerkWh=300;		// Euros

    Battery(int , Genes );// : Buffer(int, Genes );
    virtual void RunBuffer(double , double );
    ~Battery();
};

#endif