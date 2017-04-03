//single Compartment class
#ifndef COMPARTMENT
#define COMPARTMENT
#include <boost/foreach.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include "conductance.h"
#include "compstate.h"
#include "sensor.h"
#include "electrode.h"

#include <memory>

using namespace std;

//#define foreach_    BOOST_FOREACH

/* single Compartment class (unit capacitance)
 * contains vector of conductances
 * integrates membrane equation after
 * integrating each conductance state equation */
class conductance;


class Compartment
{
protected:
    double cm; // capacitance (not used)

   
    
    //vector<Electrode*> iclamp;
    // voltage and other state variables (calcium, ..
    compstate state;    //pointer passed to membrane mechanisms (channels, sensors,..)
    //shared_ptr<Compartment> preComp;
    double sum_g;
    
    double Ca;           //calcium
    double Ca_inf; //put in compstate
    double tau_Ca;
    double Ra;
    double ra;
    // specify geometry in um
    double length;
    double radius;
    double area, area2;
    double Qj;
public:
   

    Compartment(string const & in_name, double in_v): c_name(in_name), v(in_v) {}
    // make compartment object and create setter methods to set cm and ra and compstate object
    Compartment(double v_init) {
	v = v_init;
    }
    void initialize(double ra_in, double cm_in) {
        cond.reserve(20);
	ra = ra_in;
	cm = cm_in;
	state = compstate(v, 0, cm_in,ra, 1);
    }
    // constructor
    // should change argument to (const double &v_init) 
    Compartment(double v_init, double l, double diam)
    {
       // cout << "Entered Compartment constructor" << endl;
        Ra=100; // (Ohm-cm)
        ra=0.01*Ra*(l/2)/(M_PI*pow((diam/2), 2)); //// 0.01 is necessary to go from Ohm cm um/um^2 to MOhm
        radius = diam/2;
        length = l; 
        area2 = M_PI*pow((diam/2), 2); //um^2 cross-sectional area
        area = 2*M_PI*(diam/2)*l; // surface-area
        cm = 1;//uF/cm^2 
        state = compstate(v_init, 0.57e-3, cm, ra, area);
        //preComp = NULL;
    } 
    ~Compartment() {
	 //cout << "Compartment was destroyed\n";
    }
    double getRadius(void) {    return radius; }
    double get_length(void) {   return length; }
    double getQj(void) {    return Qj;}
    double get_ra(void) {   return ra; }
    void set_ra(double ra_in) { ra = ra_in; }
    double get_v(void);
    double get_iSyn(void);
    double get_Ca(void);
    double get_cm(void){    return cm;}
    double get_area(void){   return area;}
    double get_area2(void){   return area2;}
    double get_i_ion(void);  
    double get_i_Ca(void);
    void set_i_Ca(double);
    void setTol(double *);
    string getName() { return c_name; };
    double integrate(double,  double *, double *);
    // insert conductance

    void add_conductance(shared_ptr<conductance> cond_in);
    // connect compartment
    void connect(shared_ptr<Compartment> );

    // connect sensor for monitor of calcium concentration
    void add_sensor(shared_ptr<sensor>);
    
    //
    //void get_cond_i(double*);
    int get_state_dim(void);
    int get_cond_state_dim(void);
    
    //int get_vcgs_dim(void);
    void get_state(double*);
/*
    void set_state(double*);
    void get_vcgs(double*);
    void add_electrode(Electrode *e_in);*/
    static shared_ptr<Compartment> create (string const &in_name, double in_v) { return shared_ptr<Compartment>(new Compartment(in_name, in_v)); }
  private:
    string c_name;
    double v;           //voltage
    vector<shared_ptr<conductance>> cond;
    vector<shared_ptr<Compartment>> neighbors;
    vector<shared_ptr<sensor>> sens; //sensor of calcium  
};
#endif

