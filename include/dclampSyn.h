// LEAK CONDUCTANCE
#ifndef DCLAMPSYN 
#define DCLAMPSYN
#include <iostream>
#include "conductance.h"

using namespace std;

//inherit conductance class spec
class dclampSyn: public conductance {

    private:
	vector<double> g_Syn;
	int count;
	double tOld;
    public:
    //constructors - must set gbar!    
    // gbar only
    dclampSyn(double g_in, double e_in) : conductance(g_in)
    {
	tOld = 0;
	count = 0;
	std::ifstream is("gSyn_10Cycles_P1000_dt0Pt05.dat", ios::in);

	double val;
	while (is >> val) {
	    g_Syn.push_back(val);
	}
	
        gbar = g_in;

        e_rev = e_in;
	flag = 1;
    }
    
    ~dclampSyn(){} 

    void set_atol(double *tol) {
    	tol[0] = 1e-12;
    }
    double i(void) {
	//cout << g << "\t" << g*(v_m()-e_rev) << "\n";
        return g*(v_m()-e_rev);
    }
    int get_state_dim(void){
        return 0;
    }
    int integrate(double t, double *y, double *yd){
	if (t != tOld) {
	    tOld = t;
	    count++;
	}
        g = gbar*g_Syn[count];
	return 0;
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
    static shared_ptr<conductance> create (double g_in, double e_in) { 
	return shared_ptr<dclampSyn>(new dclampSyn(g_in, e_in)); }
};
#endif
