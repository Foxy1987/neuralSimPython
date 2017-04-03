#include "neuron.h"


void Neuron::init(vector<double> &y) {
    double *yi = y.data();
    //int n_comp = (int) comp.size();
    int ind = 0;

    for (auto& c : comp) {
	c->get_state(&yi[ind]);
	ind += c->get_state_dim();
    }
}


int Neuron::get_state_dim(void) {
    //int n_comp = (int) comp.size();
    int st_dim = 0;
   
    for (auto& c : comp)
	st_dim += c->get_state_dim();

    return st_dim;

}
/*
void Neuron::get_state(double *out) {
    int n_comp = (int) comp.size();
    int ind = 0;

    for (int i = 0; i < n_comp; i++) {
        comp[i]->get_state(&out[ind]);
        ind += comp[i]->get_state_dim();
    }
}
*/
void Neuron::addCompartment(shared_ptr<Compartment> comp_in) {
   comp.push_back(comp_in);
   //cout << comp[0]->getName() << " was added to " << n_name << "\n";
}

void Neuron::connect_compartments(shared_ptr<Compartment> c1, shared_ptr<Compartment> c2) {

    c1->connect(c2);
    c2->connect(c1);

}
/*
void Neuron::setTol(double *tol) {
    int n_comp = (int) comp.size();
    int ind = 0;
    for (int i = 0; i < n_comp; i++) {
         comp[i]->setTol(&tol[ind]);
         ind+=comp[i]->get_state_dim();
    }
}
*/
int Neuron::derivative(realtype t, N_Vector &_y, N_Vector &_ydot, void *user_data) { 
    realtype *yi, *yd;
    yi = NV_DATA_S(_y);
    yd = NV_DATA_S(_ydot);
    
    

    int n_comp = (int) comp.size();
    int ind = 0;
    for(int i = 0; i < n_comp; i++) {
        switch(i) {
            case 1:
                comp[i]->integrate(t, &yi[ind], &yd[ind]);
                break;
            default:
               comp[i]->integrate(t, &yi[ind], &yd[ind]);
        }

        ind += comp[i]->get_state_dim();
    }   
    return 0; 
}



