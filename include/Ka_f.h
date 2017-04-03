// LEAK CONDUCTANCE
#ifndef KA_F 
#define KA_F

#include "conductance.h"
#include <cmath>


//inherit conductance class spec
class Ka_f: public conductance {
protected:
    double m;
    double h;
public:
    //constructors - must set gbar!    
    // gbar only
    Ka_f(double g_in) : conductance(g_in)
    {
        state_dim = 2;
        // default if unspecified
        e_rev = -80.0;
    }
    
    //specify both gbar and erev
    Ka_f(double g_in, double e_in) : conductance(g_in)
    {
        state_dim = 2;
        e_rev = e_in;
    }

    void set_atol(double *tol) {
        tol[0] = 1e-10;
        tol[1] = 1e-10;
    }
    double i(void) {
         return (gbar*m*m*m*h*(v_m()-e_rev));
    }

    int integrate(double t, double *y, double *yd) {
        m = y[0]; h = y[1]; 

	double m_inf, tau_m, h_inf, tau_h;
	m_inf = 1.0/(1.0+exp(-0.5*(v_m()+54.0)));
	tau_m = 5.0+20.0*m_inf;

	h_inf = 1.0/(1.0+exp((v_m()+60.0)));
	tau_h = 50.0+100.0*h_inf;

        yd[0] = ((m_inf - m)/tau_m);
        yd[1] = ((h_inf - h)/tau_h);
        g = gbar*m*m*h;
	return 0;
    }
    //let conductance access cell/compartment state
    int get_state_dim(void) {
        return 2;
    }
    int connect(compstate *s_in) {
        compartment_state = s_in;
	double m_inf, h_inf;

        m_inf = 1.0/(1.0+exp(-0.5*(v_m()+54.0)));

        h_inf = 1.0/(1.0+exp((v_m()+60.0)));

	m=m_inf;
	h=h_inf;
	return 0;
     }
    
    int state2double(double* st){
        st[0] = m;
        st[1] = h;
	return 0;
    }
    void double2state(double *st){
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
	return shared_ptr<Ka_f>(new Ka_f(g_in, e_in)); }
};
#endif
