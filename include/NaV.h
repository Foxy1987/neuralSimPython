// Sodium CONDUCTANCE
#ifndef NAV
#define NAV
#include "conductance.h"
#include <iostream>
#include <cmath>
using namespace std;

//inherit conductance class spec
class NaV: public conductance {
protected:
    double m;
    double h;
    double vshiftm;
    double vscalem;
    double tauscalem;
    double vshifth;
    double vscaleh;
    double tauscaleh;
    
public:
    //constructors - must set gbar!    
    // gbar only
    NaV(double g_in)//: conductance(g_in)
    {
        gbar = g_in;
        state_dim = 2;
        // default if unspecified
        e_rev = 50.0;
    }
    
    //specify both gbar and erev
    NaV(double g_in, double e_in) : conductance(g_in)
    {
        vshiftm=-1.476393;
        vscalem=9.1553499999999999e-01;
        tauscalem=5.0015500000000002e-01;
        vshifth=-9.5136900000000004e+00;
        vscaleh=5.0000000000000000e-01;
        tauscaleh=2.0000000000000000e+00;
        state_dim = 2;
        e_rev = e_in;
   
    }
    void set_atol(double *tol) {
        tol[0] = 1e-10;
        tol[1] = 1e-10;
    }
    //return current
    double i(void) {
        return (1e-4)*g*(v_m()-e_rev);
    }

    void integrate(double t, double *y, double *yd) {

        m = y[0]; h = y[1]; 
        double m_inf; double tau_m;
        double h_inf; double tau_h;
	double ms = -5.3; double hs = -12;
	double Q= 1.501533;
        double vscaledm=v_m()/vscalem-vshiftm;
        
        double alpham=Q*1.0*linoid((vscaledm+35.0-5.3)/10.0)/tauscalem;
        
        
        double betam=Q*4.0*exp(-(vscaledm+60.0-5.3)/18.0)/tauscalem;
        
        m_inf=alpham/(alpham+betam);
        
        
        tau_m=1/(alpham+betam);
        
        
            
        double vscaledh=v_m()/vscaleh-vshifth;
        
        double alphah=Q*0.07*exp(-(vscaledh+48)/20)/tauscaleh/tauscaleh;
        
        
        double betah=Q*1.0*lgc((vscaledh-(-30.0+12.0))/10.0)/tauscaleh;
        
                  
        h_inf=alphah/(alphah+betah);
        tau_h=1.0/(alphah+betah);

        yd[0] = ((m_inf - m)/tau_m);
        yd[1] = ((h_inf - h)/tau_h);
        g = gbar*m*m*m*h;
    }
    //let conductance access cell/compartment state
    int get_state_dim(void) {
        return 2;
    }
    void connect(compstate *s_in) {
        compartment_state = s_in;
        double m_inf; double tau_m;
        double h_inf; double tau_h;
	
	double ms = -5.3; double hs = -12;
        double Q= 1.501533;
        double vscaledm=v_m()/vscalem-vshiftm;
        double alpham=Q*1.0*linoid((vscaledm-(-35.0+5.3))/10.0)/tauscalem;
        
        
        double betam=Q*4.0*exp(-(vscaledm-(-60.0+5.3))/18.0)/tauscalem;
        
        m_inf=alpham/(alpham+betam);
            
        double vscaledh=v_m()/vscaleh-vshifth;
         double alphah=Q*0.07*exp(-(vscaledh+48)/20)/tauscaleh;
        
        
        double betah=Q*1.0*lgc((vscaledh-(-30.0+12.0))/10.0)/tauscaleh;
        
                  
        h_inf=alphah/(alphah+betah);
        
        m = m_inf;
        h = h_inf;
     }

    void state2double(double* st){
        st[0] = m;
        st[1] = h;
    }
    void double2state(double *st){
        m = st[0];
        h = st[1];
    }
};
#endif
