#ifndef _ELECTRIC_MOTOR_H
#define _ELECTRIC_MOTOR_H

#include "Machine.h"
#include "MotorTransmission.h"

class ElectricMotor : public Machine {
public:
	//std::shared_ptr<MotorTransmission> transmissionForMachine;

    ElectricMotor(int , Genes , std::shared_ptr<Buffer> , int );
    ~ElectricMotor();
};

#endif