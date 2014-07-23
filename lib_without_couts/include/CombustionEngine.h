#ifndef _COMBUSTION_ENGINE_H
#define _COMBUSTION_ENGINE_H

#include "Machine.h"
#include "Unit.h"
#include "Axle.h"
#include "MechanicalGearbox.h"

class CombustionEngine : public Machine {
public:
    //std::shared_ptr<MechanicalGearbox> transmissionForMachine;

    CombustionEngine(int , Genes , std::shared_ptr<Buffer> , int );
    void SetEngineOptimumOperatingPoint();
    ~CombustionEngine();
};

#endif