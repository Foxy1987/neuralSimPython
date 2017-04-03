#ifndef _NEURONMODEL_H
#define _NEURONMODEL_H
#include <memory>
#include <iostream>
#include <vector>
#include <nvector/nvector_serial.h>  /* serial N_Vector types, fcts., macros */
#include <sundials/sundials_types.h> /* definition of type realtype */


using namespace std;

class Neuron;

class NeuronModel : public enable_shared_from_this<NeuronModel>
{
  private:
    N_Vector yi;
    N_Vector yn;
    double tout, dt, dtx;
    vector<shared_ptr<Neuron>> neurs;
    int dim;
  public:
    NeuronModel(vector<shared_ptr<Neuron>>, int); 
    ~NeuronModel(){ 
	

	// clean up N_Vectors
	N_VDestroy_Serial(yi);
	N_VDestroy_Serial(yn);
};
    void printNeuronNames();
        /* loops over neuron and synapse objects in the list so that
         * solver can call integrate function at time t */
    // this function is used to rescale the ODEs by 1/f
    int getDim() { return dim; }
    virtual void derivative(realtype t, N_Vector &, N_Vector &, void *);

    void run(int, double, vector<double>, vector<double> &, vector<double> &, vector<double> &);
    static shared_ptr<NeuronModel> create (vector<shared_ptr<Neuron>> in_vec, int in_dim) { return 				shared_ptr<NeuronModel>(new NeuronModel(in_vec, in_dim)); }
};

#endif
