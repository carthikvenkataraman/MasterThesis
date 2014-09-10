#ifndef _BATTERY_H
#define _BATTERY_H

#include "Buffer.h"

typedef std::vector<int> Genes;

class Battery : public Buffer {

public:
    Battery(int , Genes );// : Buffer(int, Genes );
    virtual void RunBuffer(double , double );
    ~Battery();
};

#endif