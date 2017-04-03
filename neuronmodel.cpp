#include "neuronmodel.h"
#include "rk65n.h"
#include "rk4.h"
#include "neuron.h"
#define VARIABLE 0
#define FIXED 1

NeuronModel::NeuronModel(vector<shared_ptr<Neuron>> neurs, int in_N)
{
    dim = in_N;
    // if we have multiple neurons, call each of their get_state_dim() methods
    
    this->neurs = neurs;
}

void NeuronModel::printNeuronNames() {
    for (auto n : neurs) {
	cout << n->getName() << "\n";
    }
}

void NeuronModel::derivative(realtype t, N_Vector &y, N_Vector &ydot, void *user_data) {  
    //this->printNeuronNames();
    for (auto n : neurs) {
	n->derivative(t, y, ydot, user_data);
    }
}

// see if we can put y back in the NeuronModel constructor
void NeuronModel::run(int method, double T, vector<double> y, vector<double> &t, vector<double> &v, vector<double> &iSyn) {

    // put a conditional on solver so if the user decides variable step, then  RK65n otherwise RK4 with specified dt
    if (method == VARIABLE) {
	dt = 0.005;
        
    } else {
      
	dt = 0.05;
    }
    double rk65_MINDT= 0.05;
    double rk65_eps= 1e-3;
    double rk65_relEps= 1e-4;
    double rk65_absEps= 1e-5;
    RK65n machine(dim, rk65_MINDT, rk65_eps, rk65_absEps, rk65_relEps);
    RK4 machine2(dim);
  
    yn = N_VNew_Serial(dim);
    yi = N_VMake_Serial(dim, &y[0]);
    tout = 0.0; 

    while(1) {
	if (method == VARIABLE) {
           
	    dtx = machine.integrate(tout, yi, yn, *this, dt);
	    dtx = min(dtx, 2.0*dt);
	    dt=dtx;
	} else {
	    machine2.integrate(tout, yi, yn, *this, dt);
	}
	machine.swap(yi, yn, dim);
	tout +=dt;
        
     // cout << "tout is " << tout << "\t" << neurs[0]->get_vm(1)  << "\n" ;
	t.push_back(tout);
	v.push_back(neurs[0]->get_vm(1));
	iSyn.push_back(neurs[0]->get_iSyn(1));
    	if (tout >= T) {
            break;
    	} 
    }
    
}






