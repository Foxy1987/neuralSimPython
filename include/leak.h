// LEAK CONDUCTANCE
#ifndef LEAK
#define LEAK
#include <iostream>
#include "conductance.h"

using namespace std;

//inherit conductance class spec
class leak: public conductance {
public:
    //constructors - must set gbar!    
    // gbar only
    leak(double g_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 0;
        // default if unspecified
        e_rev = -50.0;
    }
    
    //specify both gbar and erev
    leak(double g_in, double e_in) : conductance(g_in)
    {
        state_dim = 0;
        e_rev = e_in;
    }
    ~leak(){ 
	//cout << "leak was destroyed\n";
    } 

    void set_atol(double *tol) {}
    double i(void) {
	//cout << "gbar is " << gbar << " and v is " << compartment_state->get_v() << " and e_rev is " << e_rev << "\n";
        //cout << "The leak current is " << gbar*(compartment_state->get_v()-e_rev) << "\n";
        //cout << " the compartment voltage is " << e_rev << "\n";
        return gbar*(compartment_state->get_v()-e_rev);
    }
    int get_state_dim(void){
        return 0;
    }
    int integrate(double t, double *y, double *yd){
	//cout << "I'M IN THE LEAK INTEGRATE FUNCTION\n";
        g = gbar;
	return 0;
    }
    int connect(compstate *s_in){
        compartment_state = s_in;
	
	return 0;
    }
    int state2double(double*){return 0;}
    void double2state(double*){}   
    double i_Syn() {
	if (flag) {
	    return g*(v_m()-e_rev);
	} else {
	    return 0;
	}
    } 

    static shared_ptr<conductance> create (double g_in, double e_in) { 
	return shared_ptr<leak>(new leak(g_in, e_in)); }
};
#endif
