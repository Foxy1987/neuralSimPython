// CA_INT 
#ifndef CA_INT
#define CA_INT
#include <iostream>
#include "sensor.h"

using namespace std;

class CaInt: public sensor {
protected:
    //fixed constants
    static const int z_ca=2;
    static const double q_e; //const double variables have to be initialized outside of the class
  //  static const double N_a;
   // static const double FARADAY;
    double cai;
    double vol;
    double ica;
    double cai_ss;
    double tau_ca;
    
public:
    //constructor
    CaInt(double vol_in)
    {
        atol = 1e-18;
        tau_ca = 70.0;      //(ms)
        cai_ss = 0.02;      //(mM)
        vol = vol_in;        //(um^3)
    }
    
    int get_state_dim(void){
        return 1;
    }
    int connect(compstate *s_in){
        pcompartment_state = s_in;
        ica = pcompartment_state->get_i_Ca();
        // calculate initial calcium based on current v_m() and Ica
        pcompartment_state->set_Ca(0.57);
        cai = 0.57;
	return 0;
    }
    int integrate(double *y, double *yd){
        double ica = pcompartment_state->get_i_Ca();
        cai = y[0];
        double F = 9.4685e4; double z_ca = 2.0;
         
        yd[0] = -(1e2)*(ica/(z_ca*F*vol))-(cai-cai_ss)/tau_ca;
        pcompartment_state->set_Ca(cai);
	return 0;
    }

    int state2double(double *st){
        st[0] = cai;
	return 0;
    }
    /*void double2state(double *st){
        cai = st[0];
    }*/
    static shared_ptr<sensor> create (double vol_in) { 
	return shared_ptr<CaInt>(new CaInt(vol_in)); }
    
};

//const double CaInt::q_e = 1.602e-19;
//const double CaInt::N_a = 6.022e23;
//const double CaInt::FARADAY = 9.46855e4;
#endif

