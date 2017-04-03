// Sodium CONDUCTANCE Farzan
#ifndef NAV_F
#define NAV_F
#include "conductance.h"
#include <iostream>
#include <cmath>
using namespace std;

//inherit conductance class spec
class NaV_f: public conductance {
protected:
    double h;
    
public:
    //constructors - must set gbar!    
    // gbar only
    NaV_f(double g_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 1;
        // default if unspecified
        e_rev = 50.0;
    }
    
    //specify both gbar and erev
    NaV_f(double g_in, double e_in) : conductance(g_in)
    {
        state_dim = 1;
        e_rev = e_in;
   
    }
    void set_atol(double *tol) {
        tol[0] = 1e-12;
    }
    //return current
    double i(void) {
        return g*(v_m()-e_rev);
    }

    int integrate(double t, double *y, double *yd) {

        h = y[0]; 
        double m_inf, h_inf, tau_h;
        
        m_inf=1/(1+exp(-0.08*(v_m()+11)));
        
        h_inf=1/(1+exp(0.13*(v_m()+25)));;
        tau_h=2.0;

        yd[0] = ((h_inf - h)/tau_h);
        g = gbar*pow(m_inf, 3)*h;
	return 0;
    }
    //let conductance access cell/compartment state
    int get_state_dim(void) {
        return 1;
    }
    int connect(compstate *s_in) {
        compartment_state = s_in;

        double h_inf;
        
        h_inf=1/(1+exp(0.13*(v_m()+28)));

        h = h_inf;
	return 0;
     }

    int state2double(double* st){
        st[0] = h;
	return 0;
    }
    void double2state(double *st){
        h = st[0];
    }
    double i_Syn() {
	if (flag) {
	    return g*(v_m()-e_rev);
	} else {
	    return 0;
	}
    }
    static shared_ptr<conductance> create (double g_in, double e_in) { 
	return shared_ptr<NaV_f>(new NaV_f(g_in, e_in)); }
};
#endif
