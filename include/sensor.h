//Abstract class for defining sensors of intracellular calcium
#ifndef SENSOR
#define SENSOR
#include "compstate.h"
#include <cmath>

class sensor {
protected:
    compstate* pcompartment_state;
    double atol;
public:
    sensor()
    {
        //null pointer for safety
        //pcompartment_state = 0;
        //pvalue = 0;
    }
    
    ~sensor() {}
    
    // all of these must be defined and will be unique
    // for each sensor in general
    virtual int get_state_dim(void) =0;
    virtual int integrate(double*, double*) =0;
    virtual int connect(compstate*) =0;
    virtual int state2double(double*) =0;
    //virtual void double2state(double*) =0;
    //virtual double getATOL(void) {  return atol;} 
};

#endif
