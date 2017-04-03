// LEAK CONDUCTANCE
#ifndef MI 
#define MI
#include "conductance.h"

//inherit conductance class spec
class Mi: public conductance {
protected:
    double m;
public:
    //constructors - must set gbar!    
    // gbar only
    
    Mi(double g_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 1;
        // default if unspecified
        e_rev = -10.0;
    }
    
    //specify both gbar and erev
    Mi(double g_in, double e_in) : conductance(g_in)
    {
       
        state_dim = 1;
        e_rev = e_in;
    }
    
    void set_atol(double *tol) {
        tol[0] = 1e-12;
    }
    //required by abstract class
    int get_state_dim(void){
        return 1;
    }
    double i(void) {
        return (gbar*m*(v_m()-e_rev));
    }
    int integrate(double t, double *y, double *yd){
        m = y[0];
        yd[0] = ((m_inf() - m)/6.0);
        g = gbar*m;
	return 0;
    }
    int connect(compstate *s_in) {
        compartment_state = s_in;
        m=m_inf();
	return 0;
    }

    int state2double(double *st){
        st[0] = m;  
	return 0; 
    }
    void double2state(double *st){
        m = st[0];
    }
    
    //specific to this class
    double m_inf(void) {
        return 1.0/(1.0+exp(-(v_m()+45.0)/5.0));
    }
    double i_Syn() {
	if (flag) {
	    return g*(v_m()-e_rev);
	} else {
	    return 0;
	}
    }
    static shared_ptr<conductance> create (double g_in, double e_in) { 
	return shared_ptr<Mi>(new Mi(g_in, e_in)); }
};
#endif
