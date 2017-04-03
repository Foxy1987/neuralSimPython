/// calcium ohmic conductance

#ifndef CAS
#define CAS
#include "conductance.h"
#include <iostream>
#include <cmath>

using namespace std;

class CaS: public conductance {
    protected:
        double m;
        double h;
        double i_Ca;
    public:
        CaS(double g_in, double e_in) : conductance(g_in) {
            state_dim = 2;
            e_rev = e_in;
        }

    void set_atol(double *tol) {
        tol[0] = 1e-14;
        tol[1] = 1e-14;
    }
    
    double i(void) {
	double eca = compartment_state->calc_e_Ca();
	//cout << "ICa reversal potential is " << eca << "\n";
	i_Ca = compartment_state->get_i_Ca();
	compartment_state->set_i_Ca(i_Ca+g*(v_m()-120.0));
        return g*(v_m()-eca);
    }
    
    int integrate(double t, double *y, double *yd) {
        m = y[0]; h = y[1];
        yd[0] = (m_inf() - m)/tau_m();
        yd[1] = (h_inf() - h)/tau_h();
	g = gbar*m*m*m*h;
	return 0;
    }
    
    int get_state_dim(void) {
        return 2;
    }

    int connect(compstate *s_in) {
        
        compartment_state = s_in;
        
        m = m_inf();
        h = h_inf();
	return 0;

    }

    int state2double(double* st) {
    
        st[0] = m;
        st[1] = h;
	return 0;

    }

    void double2state(double *st) {
        m = st[0];
        h = st[1];
    }

    double m_inf(void) {return 1.0/(1.0+exp(-(v_m()+45.0)/5.0));}
 //double m_inf(void) {return 1.0/(1.0+exp(-(v_m()+30.0)/5.0));}
 //   double h_inf(void) {return 1.0/(1.0+exp((v_m()+60.0)/1.0));}
    double h_inf(void) {return 1.0/(1.0+exp((v_m()+60.0)/.5));}
    double tau_m(void) {return 1.0;}
   // double tau_h(void) {return 3500.0 +(500.0-3500.0)*h_inf();}
    double tau_h(void) {return 85.0;}
    double i_Syn() {
	if (flag) {
	    return g*(v_m()-e_rev);
	} else {
	    return 0;
	}
    }
    static shared_ptr<conductance> create (double g_in, double e_in) { 
	return shared_ptr<CaS>(new CaS(g_in, e_in)); }


};
#endif
