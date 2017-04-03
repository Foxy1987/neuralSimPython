#ifndef _NEURON_H
#define _NEURON_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <list>
#include <vector>
#include <queue>

#include "compartment.h"

#define efunc(A, B, V) (1.0/(1.0+exp(A*(V+B))))
#define forall(l, it) for (it= (l).begin(); it != (l).end(); it++)
#define Ith(v,i)    NV_Ith_S(v,i-1)       /* Ith numbers components 1..NEQ */
#define INF 1.0e14

class Compartment;

using namespace std;

/* let this be the base class from which all other neuron subclasses will derive methods
 * define parameters: conductances, equilibrium potentials
 * let neuron class define own function derivative y' = f(t,y)
 */
class Neuron {
  protected:
    int enabled, idx;
  public:

    Neuron(string const & in_name): n_name(in_name) {}   
    virtual ~Neuron(){ 
	cout << "Neuron destroyed\n";
    }

    virtual int derivative(realtype t, N_Vector &, N_Vector &, void *user_data);
    virtual void init(vector<double> &y);
    //virtual void setTol(double *);
    string getName() { return n_name; }
    //void writeStateVector(N_Vector y);*/
    void addCompartment(shared_ptr<Compartment> comp_in);// {comp.push_back(comp_in);}
  
    void connect_compartments(shared_ptr<Compartment>, shared_ptr<Compartment> );
    double get_vm(int i) {
	return comp[i]->get_v();
    }
    double get_iSyn(int i) {
	return comp[i]->get_iSyn();
    }
    int get_state_dim(void);
    //void get_state(double *);
    bool operator==(const Neuron& other) {return false;}
    bool operator!=(const Neuron& other) {return true;}
    static shared_ptr<Neuron> create (string const &in_name) { return 				  		shared_ptr<Neuron>(new Neuron(in_name)); }
  private:
    string n_name;
    //int mode;
    vector<shared_ptr<Compartment>> comp;
};
#endif
