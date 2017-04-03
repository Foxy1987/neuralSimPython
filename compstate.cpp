#include "compstate.h"

// setters
void compstate::set_v(double v_in)      {v = v_in;}
void compstate::set_Ca(double c_in)     {Ca = c_in;}
void compstate::set_e_leak(double e_in) {e_leak = e_in;}
void compstate::set_e_K(double e_in)    {e_K = e_in;}
void compstate::set_e_Na(double e_in)   {e_Na = e_in;}
void compstate::set_e_Ca(double e_in)   {e_Ca = e_in;}
void compstate::set_e_h(double e_in)    {e_h = e_in;}
void compstate::set_i_Ca(double iCa_in) {ica = iCa_in;}
void compstate::set_i_ion(double i_ion_in) {i_ion = i_ion_in;}
void compstate::set_i_Syn(double i_Syn_in) {i_Syn = i_Syn_in;}
// getters

double compstate::get_i_ion(void)   {return i_ion;}
double compstate::get_i_Syn(void)   {return i_Syn;}
double compstate::get_v(void)       {return v;}
double compstate::get_Ca(void)      {return Ca;}
double compstate::get_e_leak(void)  {return e_leak;}
double compstate::get_e_K(void)     {return e_K;}
double compstate::get_e_Na(void)    {return e_Na;}
double compstate::get_e_Ca(void)    {return e_Ca;}
double compstate::get_e_h(void)     {return e_h;}
double compstate::get_i_Ca(void)    {return ica;}
double compstate::getArea(void)     {return area;}
double compstate::get_cm(void)      {return cm;}
double compstate::get_ga(void)      {return ga;}

//set reversal according to Nernst equation
double compstate::calc_e_Ca(void)
{
    double e_Ca = 500.0*R_F*(T+273.15)*log(3000.0/Ca);
    return e_Ca;
}

