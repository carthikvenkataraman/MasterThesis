#ifndef _FUELTANK_H
#define _FUELTANK_H

#include "Buffer.h"

typedef std::vector<int> Genes;

class FuelTank : public Buffer {
	void LoadFuelTank();
public:
	double instantaneousBufferDemandInGrams;
	double densityOfFuel = 0.832;	// kg/litre
    FuelTank(int , Genes ); // : Buffer(int, Genes );
    virtual void RunBuffer(double , double );
    ~FuelTank();
};

#endif