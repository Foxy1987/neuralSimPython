// LEAK CONDUCTANCE
#ifndef KCA
#define KCA
#include "conductance.h"

//inherit conductance class spec
class KCa: public conductance {
protected:
    double m;
    double h;
public:
    //constructors - must set gbar!    
    // gbar only
    KCa(double g_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 1;
        // default if unspecified
        e_rev = -80.0;
    }
    
    //specify both gbar and erev
    KCa(double g_in, double e_in) : conductance(g_in)
    {
        state_dim = 1;
        e_rev = e_in;
    }
    
    void set_atol(double *tol) {
        tol[0] = 1e-14;
        //tol[1] = 1e-18;
    }

    //return current
    double i(void) {
	//cout << (gbar*m*m*m*m*(v_m()-e_rev)) << "\n";
        return (gbar*m*m*m*m*(v_m()-e_rev));
    }
    int get_state_dim(void) {
        return 1;
    }
    int integrate(double t, double *y, double *yd){
        m = y[0]; //h = y[1];
        //cout << "m and h are " << m << "\t" << h << "\n";
        yd[0] = ((m_inf() - m)/tau_m());
        //yd[1] = ((h_inf() - h)/15.0);
	return 0;
    }
    //let conductance access cell/compartment state
    int connect(compstate *s_in){
        compartment_state = s_in;
        m = m_inf();
        h = h_inf();
	return 0;
    }
    double m_inf(void) {
        double c = compartment_state->get_Ca();
	//cout << "intracellular calcium is " << c << "\n";
        //return 1.0/((1.0+pow(c/1.43, -5.0)))*(1.0/(1.0+exp(-(v_m()+35.5)/8.0)));
	return (c/(c+3.0e-3))*(1/(1+exp(-(v_m()+51.0)/4)));
    }
    double tau_m(void) {
        double c = compartment_state->get_Ca();
        //return 299.0+(5.0-299.0)/(1.0+exp(-(v_m()-(-52.0-(2.7)*log(c/1)))/10.0));
	return 90.0-(75.0/(1+exp(-(v_m()+46.0)/22.0)));
    }
    double h_inf(void) {
        double c = compartment_state->get_Ca();
        return 1.0/(1.0+pow(c/7.0, 1.25));
    }
    int state2double(double *st) {
        st[0] = m;
        //st[1] = h;
	return 0;
    }
    /*void double2state(double *st){
        m = st[0];
        //h = st[1];
    }*/
    double i_Syn() {
	if (flag) {
	    return g*(v_m()-e_rev);
	} else {
	    return 0;
	}
    }
     static shared_ptr<conductance> create (double g_in, double e_in) { 
	return shared_ptr<KCa>(new KCa(g_in, e_in)); }
};
#endif
