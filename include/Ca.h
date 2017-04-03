// LEAK CONDUCTANCE
#ifndef CA 
#define CA
#include "conductance.h"
#include <cmath>

using namespace std;

//inherit conductance class spec
class Ca: public conductance {
protected:
    double m;
    double Pbar;
    double h;
    double frac;
    double ghknow;
    static const int z_ca=2;
    static const double q_e; 
    static const double N_a;
    static const double k;
    static const double FARADAY;
    static const double R;
    double co;
    double celsius;
    double ica;
public:
    Ca(double Pbar_in) {
        Pbar = Pbar_in;
        state_dim = 1;
        co = 13;       // (mM)
        celsius = 10;   // (degC)
        ica = 0.001;
    }
    
    //return current
    double i(void) {
        //cout << frac << "\n";
        frac = m*m*m*h;
        ica = Pbar*frac*ghknow;
        compartment_state->set_i_Ca(ica);
        return ica;
    }
    int get_state_dim(void) {
       return 1;   
    }
    void set_atol(double *tol) {
        tol[0] = 1e-10;
    }
    void integrate(double t, double *y, double *yd){
        m = y[0]; //cout << m << endl;
        yd[0] = ((m_inf() - m)/tau_m());
        double cai = compartment_state->get_Ca();
        fh(cai);
        ghknow=ghk(cai);
    }
    //let conductance access cell/compartment state
    void connect(compstate *s_in){
        compartment_state = s_in;
        m=m_inf();
    }

    double ghk(double ci) {
        double z, eci, eco;
        z = 1e-3*((2*9.6485e4*v_m())/(8.3145*(10+273.15)));
        eco = co*efun(z);
        eci = ci*efun(-z);
        
        return 1e-7*(2*9.6485e4*(eci - eco));
    }
    double m_inf(void) {
        return 1/(1+exp(-(v_m()+45.2)/5.6));
    }
    double tau_m(void){
        return 1.8 + (4.1 - 1.8)/(1+exp(-(v_m()+50.2)/20.7));
    }
    // function to calculate the channel inactivation due to cai
    void fh(double cai_in) {
        h = 1/(1+(cai_in/12.57e-3));
    }
    double efun(double z) {
        if (abs(z) < 1e-4) {
            return 1 - z/2;
        }
        else {
            return z/(exp(z)-1);
        }
    }
    void state2double(double *st){
        st[0] = m;
    }
    void double2state(double *st){
        m = st[0];
    }
};

const double Ca::q_e = 1.602e-19;
const double Ca::N_a = 6.022e23;
const double Ca::k = 1.381e-23;
const double Ca::FARADAY = 9.6485e4;
const double Ca::R = 8.3145;
#endif
