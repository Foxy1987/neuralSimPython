// calcium ohmic conductance

#ifndef CAOHMIC
#define CAOHMIC
#include "conductance.h"
#include <iostream>
#include <cmath>

using namespace std;

class CaOhmic: public conductance {
    protected:
        double m;
        double h;
        double m_inf, h_inf;
        double i_Ca;
    public:
        CaOhmic(double g_in, double e_in) : conductance(g_in) {
            state_dim = 2;
            e_rev = e_in;
        }

    void set_atol(double *tol) {
        tol[0] = 1e-14;
        tol[1] = 1e-14;
    }
    
    double i(void) {
	//double RT_zF = 1000*(8.314*10.0+273.15)/(2*96485);
	//e_rev = RT_zF*log(13e-3/cai);

	i_Ca = compartment_state->get_i_Ca();
	compartment_state->set_i_Ca(i_Ca+g*(v_m()-e_rev));
        return g*(v_m()-e_rev);
    }
    
    int integrate(double t, double *y, double *yd) {
        m = y[0]; h = y[1];
	double tau_m;
        m_inf = 1/(1+exp(-(v_m()+55.0)/1.0));
        h_inf = 1/(1+exp((v_m()+54.0)/3.0));
        
	tau_m = 60.0+30.0*(1-m_inf);
        yd[0] = (m_inf - m)/40.0;
        yd[1] = (h_inf - h)/400.0;
	g = gbar*m*m*m*h;
	return 0;
    }
    
    int get_state_dim(void) {
        return 2;
    }

    int connect(compstate *s_in) {
        
        compartment_state = s_in;
        m_inf = 1/(1+exp(-(v_m()+50.0)/8.0));
        h_inf = 1/(1+exp((v_m()-67.0)/6.0));
        
        m = m_inf;
        h = h_inf;
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
    double i_Syn() {
	if (flag) {
	    return g*(v_m()-e_rev);
	} else {
	    return 0;
	}
    }

    static shared_ptr<conductance> create (double g_in, double e_in) { 
	return shared_ptr<CaOhmic>(new CaOhmic(g_in, e_in)); }
};
#endif
