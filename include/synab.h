// LEAK CONDUCTANCE
#ifndef SYNAB 
#define SYNAB
#include <iostream>
#include "conductance.h"

using namespace std;

//inherit conductance class spec
class SynAB: public conductance {
    protected:
        double T;
        double thresh;
        double scale;
	double tauf;
    public:
    //constructors - must set gbar!    
    // gbar only
    SynAB(double g_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 1;
        // default if unspecified
        e_rev = -80.0;
    }
    
    //specify both gbar and erev
    SynAB(double T_in, double g_in, double e_in) : conductance(g_in)
    {
	flag = 1;
        T = T_in;
        thresh = -58;
        scale = 15;
        state_dim = 0;
        e_rev = e_in;
	tauf = 1.0;
    }
    ~SynAB(){} 

    void set_atol(double *tol) {
    	tol[0] = 1e-12;
    }
    double i(void) {
        return g*(v_m()-e_rev);
    }
    int get_state_dim(void){
        return 0;
    }
    int integrate(double t, double *y, double *yd){
        double phi, f_inf, f;
	//f = y[0];
	//double vpre;
        phi=2*M_PI*(t/T);

        vpre = (-62.6) + (11.039)*cos(1*phi+(6.054)) + (5.199)*cos(2*phi+(5.184)) + (1.437)*cos(3*phi+(10.824)) + (0.323)*cos(4*phi+(14.913)) + (0.256)*cos(5*phi+(19.364)) + (0.325)*cos(6*phi+(18.787)) + (0.110)*cos(7*phi+(23.995));

        f_inf = naka_rushton((vpre-thresh)/scale);
	//f_inf = 1/(1+exp(-(vpre+58.0)/15.0));
	//yd[0] = (f_inf-f)/tauf;
        g = gbar*f_inf;
	return 0;
    }
    
    double naka_rushton(double x) {
        double y = 0;
        if (x > 0) {
            y = pow(x, 2);
            return y/(1+y);
        } else {
            return 0;
        }
    }
    double get_vpre() {
	return vpre;
    }
    int state2double(double*){ return 0;}
    void double2state(double*){}  

    int connect(compstate *s_in){
	//cout << "Ih has been connected to this compartment\n";
        compartment_state = s_in;
        return 0;
    }  

    double i_Syn() {
	if (flag) {
	    return g;
	} else {
	    return 0;
	}
    }
    static shared_ptr<conductance> create (double T_in, double g_in, double e_in) { 
	return shared_ptr<SynAB>(new SynAB(T_in, g_in, e_in)); }
};
#endif
