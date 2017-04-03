// H CONDUCTANCE Farzan
#ifndef H_F
#define H_F
#include "conductance.h"
#include <iostream>
#include <cmath>
using namespace std;

//inherit conductance class spec
class H_f: public conductance {
protected:
    double m;
    
public:
    //constructors - must set gbar!    
    // gbar only
    H_f(double g_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 1;
        // default if unspecified
        e_rev = -10.0;
    }
    
    //specify both gbar and erev
    H_f(double g_in, double e_in) : conductance(g_in)
    {
        state_dim = 1;
        e_rev = e_in;
   
    }
    void set_atol(double *tol) {
        tol[0] = 1e-14;
    }
    //return current
    double i(void) {
        return g*(v_m()-e_rev);
    }

    int integrate(double t, double *y, double *yd) {

        m = y[0]; 
        double m_inf, tau_m;
        
        m_inf=max(tanh(-0.1*(v_m()+60.0)), 0.0);
        
        tau_m=200.0;

        yd[0] = ((m_inf - m)/tau_m);
        g = gbar*m*m;
	return 0;
    }
    //let conductance access cell/compartment state
    int get_state_dim(void) {
        return 1;
    }
    int connect(compstate *s_in) {
        compartment_state = s_in;

        double m_inf;
        m_inf=max(tanh(-0.1*(v_m()+60.0)), 0.0);
        m = m_inf;
	return 0;
     }

    int state2double(double* st){
        st[0] = m;
	return 0;
    }
    void double2state(double *st){
        m = st[0];
    }
    double i_Syn() {
	if (flag) {
	    return g*(v_m()-e_rev);
	} else {
	    return 0;
	}
    }
    static shared_ptr<conductance> create (double g_in, double e_in) { 
	return shared_ptr<H_f>(new H_f(g_in, e_in)); }
};
#endif
