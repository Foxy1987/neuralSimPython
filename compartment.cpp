//single compartment class
#include "compartment.h"


double Compartment::get_v(void)
{
    return state.get_v();
}
double Compartment::get_iSyn(void)
{
    return state.get_i_Syn();
}
double Compartment::get_Ca(void)
{
    return state.get_Ca();
}

double Compartment::get_i_ion(void) 
{
    return state.get_i_ion();
}
double Compartment::get_i_Ca(void)
{
    return state.get_i_Ca();
}
void Compartment::set_i_Ca(double c)
{
    state.set_i_Ca(c);
}


double Compartment::integrate(double t, double *yi, double *yd)
{   
    int n_cond = (int) cond.size();
    state.set_i_Ca(0.0); //set ica to zero before summing contributions
    (void) state.calc_e_Ca(); //new reversal potential
    
    
    if (!n_cond)
    {
	double ra_ = 0.0;
	// set ra of node (compartment that has no conductances to have
	// sum of half of each of connected compartment's ra
	for (auto c1 : neighbors) {
	    ra_ += c1->get_ra()/2;
        }
        this->set_ra(ra_);	
        double raCommon;
        double sum1 = 0; double sum2 = 0;
        for (auto c : neighbors) {
            raCommon= (ra_ + c->get_ra());
            sum1+=c->get_v()/raCommon;
            sum2+=(1/raCommon);
        }
        state.set_v(sum1/sum2);

    } else {
 	double iIon = 0; // sum ionic current from all added conductances
        state.set_v(yi[0]);
        state.set_Ca(yi[1]);
      
        
        // integrate ca_int equation
        int ind = 1;
        
        int n_sens = (int)sens.size(); 
        if (n_sens > 0) {
            
            for (auto s0 : sens) {
                s0->integrate(&yi[ind], &yd[ind]);
                ind+=s0->get_state_dim();
            }
        }
        else {
            yd[1] = 0;
        }
         
	// integrate conductances
        int n_cond = (int) cond.size();
        ind = 2; 
	int flag = 0;

	double iSyn = 0.0;
	for (auto ci : cond) {
            flag =  ci->integrate(t, &yi[ind], &yd[ind]);
            ind +=  ci->get_state_dim();
            iIon += ci->i();
	    iSyn+=ci->i_Syn();
        }
	state.set_i_Syn(iSyn);

	double i_axial=0;
        for (auto c: neighbors) {
	    
   	    i_axial = (1.0/ra)*(state.get_v() - c->get_v());
	   
	}

        yd[0] = (-(iIon+i_axial)/cm);
	
    } 
    return state.get_v();
    

}

// add conductance and provide pointer to Compartment state
void Compartment::add_conductance(shared_ptr<conductance> cond_in)
{
    cond.push_back(cond_in);

    int flag = cond_in->connect(&state);
}


int Compartment::get_state_dim(void) {
    //cout << "Compartment::get_state_dim() and size of cond is " << cond.size() << "\n";
    return 2 + get_cond_state_dim();
}

int Compartment::get_cond_state_dim(void)
{
    //cout << "Compartment::cond_state_dim()\n";
    int n_cond = cond.size();
    int st_dim = 0;
    //cout << "Number of conductances = " << n_cond << "\n";
    for (int i=0; i<n_cond; i++)     // count states
    {
        //cout << "HELLLLO!\n";
        st_dim += cond[i]->get_state_dim();   
    }
 
    return st_dim;

}


void Compartment::add_sensor(shared_ptr<sensor> sens_in) {
    sens.push_back(sens_in);
    int flag = sens_in->connect(&state); //so mechanism has access to ica in compstate
}/*
void Compartment::add_electrode(Electrode *e_in) {
    iclamp.push_back(e_in);
}
*/
void Compartment::connect(shared_ptr<Compartment> comp_in){
    //preComp = comp_in; 
    neighbors.push_back(comp_in);
  
    //cout << c_name << " was connected to " << comp_in->getName() << "\n";
    //cout << "The initial voltage of conencted compartment is " << preComp->get_v() << "\n";
}
/*
int Compartment::get_vcgs_dim(void)
{
    int n_cond = (int) cond.size();
    int ind = 0;

    return 0;
}

void Compartment::get_cond_i(double *out) {
    int n_cond = (int) cond.size();
    for (int i = 0; i < n_cond; i++) {
        out[i] = cond[i]->i();
    }
}
// get voltage, [Ca], gbars, sensor vals
void Compartment::get_vcgs(double* out)
{
    int n_cond = (int) cond.size();
    
    int ind = 2;
    
    out[0] = state.get_v();
    out[1] = state.get_Ca();
    
    for (int i=0; i<n_cond; i++)
    {
        out[ind] = cond[i]->get_gbar();
    }
}
*/
// use compstate object to fill initial vector out starting from index idx
void Compartment::get_state(double* out)
{
    int n_cond = (int) cond.size();
    
    int ind = 2;
    
    out[0] = state.get_v();
    out[1] = state.get_Ca();
    
    //cout << out[0] << "\n";
    int flag = 0;
    for (int i=0; i<n_cond; i++)
    {
        flag = cond[i]->state2double(&out[ind]);
        ind += cond[i]->get_state_dim();
    }
}
/*
void Compartment::setTol(double *tol) {
    int n_cond = (int) cond.size();
    //int n_sens = (int)sens.size(); 

    int ind = 2;

    tol[0] = 1e-8;
    tol[1] = 1e-12; 
    
 
    for (int i=0; i<n_cond; i++)
    {
        cond[i]->set_atol(&tol[ind]);
        ind += cond[i]->get_state_dim();
    } 

}
void Compartment::set_state(double* st_in)
{
    int n_cond = (int) cond.size();
    int ind = 2;
    
    state.set_v(st_in[0]);
    state.set_Ca(st_in[1]);
    
    for (int i=0; i<n_cond; i++)
    {
        cond[i]->double2state(&st_in[ind]);
        ind += cond[i]->get_state_dim();
    }
    
}

*/

