//Abstract class for defining conductances
#ifndef CONDUCTANCE
#define CONDUCTANCE
#include "compstate.h"
#include <cmath>
#include <iostream>

using namespace std;

class conductance {
    protected:
	double vpre;
        double gbar;
        double g;
        double e_rev;
        int state_dim;
        compstate *compartment_state;
	int flag;
    public:
        conductance()
        {
	    flag = 0;
            //null pointer for safety
            compartment_state = 0;
            state_dim = 0;
        }
        conductance(double g_in)
        {
            flag = 0;
            gbar = g_in; 
        }
        
        virtual ~conductance(){ 
	    //cout << "conductance was destroyed\n";
        }
        
        double v_m(void)
        {    
            return compartment_state->get_v();
        }
	void set_vpre(double v_in) {
		vpre = v_in;
	}

        double get_gbar(void) {
            return gbar;
        }

        double get_g(void) {
            return g;
        }

       // virtual void set_atol(double *) = 0;
        // all of these must be defined and will be unique
        // for each conductance in general
        virtual int get_state_dim(void) =0;
        // pass variables by reference
        virtual int integrate(double, double *, double *) =0;
        virtual double i(void) =0; //consider removing this
        virtual int connect(compstate *s_in)=0;
       
        virtual int state2double(double*) =0;
        //virtual void double2state(double*) =0;
	virtual double i_Syn() =0;

};

#endif
