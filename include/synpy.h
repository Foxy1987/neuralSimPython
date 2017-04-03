// LEAK CONDUCTANCE
#ifndef SYNPY 
#define SYNPY
#include <iostream>
#include "conductance.h"

using namespace std;

//inherit conductance class spec
class SynPY: public conductance {
    protected:
        double T;
        double thresh;
        double scale;
	double tauf;
public:
    //constructors - must set gbar!    
    // gbar only
    SynPY(double g_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 1;
        // default if unspecified
        e_rev = -70.0;
    }
    
    //specify both gbar and erev
    SynPY(double T_in, double g_in, double e_in) : conductance(g_in)
    {
        flag = 1;
        T = T_in;
        thresh = -53;
        scale = 3;
        state_dim = 1;
        e_rev = e_in;
	tauf = 1;
    }
    ~SynPY(){} 

    void set_atol(double *tol) {
    	tol[0] = 1e-12;
    }
    double i(void) {
        return g*(v_m()-e_rev);
    }
    int get_state_dim(void){
        return state_dim;
    }
    int integrate(double t, double *y, double *yd){
	
        double phi, vpre, f_inf, f;
	f = y[0];
        phi=2*M_PI*(t/T);
      
        vpre = (-57.6) + (11.425)*cos(1*phi+(1.293)) + (4.407)*cos(2*phi+(0.503)) + (1.581)*cos(3*phi+(0.019)) + (1.177)*cos(4*phi+(0.095)) + (0.786)*cos(5*phi+(-0.460)) + (0.598)*cos(6*phi+(-7.240)) + (0.305)*cos(7*phi+(-7.474));
        
       // f_inf = naka_rushton((vpre-thresh)/scale);
	f_inf = 1/(1+exp(-(vpre+53.0)/3.0));
	yd[0] = (f_inf - f)/tauf;
        g = gbar*f;
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
    int state2double(double*){return 0;}
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
	return shared_ptr<SynPY>(new SynPY(T_in, g_in, e_in)); }
};
#endif
