//class for storing state of a compartment
#define R_F 8.6174e-005 
#ifndef COMPSTATE
#define COMPSTATE
#include <cmath>
#include <cvode/cvode.h>             /* prototypes for CVODE fcts., consts. */
#include <nvector/nvector_serial.h>  /* serial N_Vector types, fcts., macros */
#include <sundials/sundials_types.h> /* definition of type realtype */
#include <iostream>

class compstate
{
protected:
    double v;
    double Ca;     //calcium concentration (uM)
    double e_leak;
    double e_K;
    double e_Na;
    double e_Ca;
    double e_h;
    double i_Ca; //Ca current (for liu sensor)
    double T; //temperature (deg C)
    double area; 
    double cm;
    double ga;
    double i_ion;
    double ica;
    double i_Syn;
public:
    compstate() {}
    ~compstate() {}
    
    compstate(double v_in, double c_in, double cm_in, double ga_in, double area_in)
    {
	i_Syn = 0;
        ica = 0;
        i_ion=0;
        ga = ga_in;
        cm = cm_in;
        area = area_in; //(mm^2) to scale conductance gbar
        v = v_in;
        Ca = c_in;
        //default reversal potentials
        e_leak = -60.0;
        e_K = -80.0;
        e_Na = 50.0;
        e_Ca = 120.0;
        T = 10.0;
    }
    
    void set_v(double);
    void set_Ca(double);
    void set_e_leak(double);
    void set_e_K(double);
    void set_e_Na(double);
    void set_e_Ca(double);
    void set_e_h(double);
    void set_i_Ca(double);
    void set_i_ion(double);
    void set_i_Syn(double);

    double get_i_Syn(void);
    double get_i_ion(void);
    double get_v(void);
    double get_Ca(void);
    double get_e_leak(void);
    double get_e_Na(void);
    double get_e_K(void);
    double get_e_Ca(void);    
    double get_e_h(void);    
    double get_i_Ca(void);
    double getArea(void);
    double get_cm(void);
    double get_ga(void);
    double calc_e_Ca(void);
};
#endif
