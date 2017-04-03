#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python.hpp>
#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/wrapper.hpp>
#include <boost/python/call.hpp>
#include "neuron.h"
#include "compartment.h"
#include "Ih_f.h"
#include "Ka_f.h"
#include "MI.h"
#include "CaS.h"
#include "caOhmic.h"
#include "ca_int.h"
#include "KCa.h"
#include "leak.h"
#include "neuronmodel.h"

#include "NaV_f.h"
#include "Kdr_f.h"

#include "synab.h"
#include "synpd.h"
#include "synpy.h"
#include "dclampSyn.h"


using namespace std;

using namespace boost::python;

class conductanceWrap : public conductance, public wrapper<conductance>  { 
    public: 
        int get_state_dim() {return this->get_override("get_state_dim")();} 
	int integrate() { return this->get_override("integrate")();} 
	double i() {return this->get_override("i")();} 
	int connect() { return this->get_override("connect")();} 
	virtual int state2double() {return this->get_override("state2double")();} 
	double i_Syn() { return this->get_override("i_Syn")();} 
}; 


class sensorWrap : public sensor, public wrapper<sensor>  { 
    public: 
        int get_state_dim() {return this->get_override("get_state_dim")();} 
	int integrate() { return this->get_override("integrate")();} 
	int connect() { return this->get_override("connect")();} 
	virtual int state2double() {return this->get_override("state2double")();} 
	/*int connect()
    	{
            if (override connect = this->get_override("connect")) {
                return connect(); // *note*
	    }
            return conductance::connect();
    }

    int default_connect() { return this->conductance::connect(); }*/
}; 

BOOST_PYTHON_MODULE(mymodules) {
    // An established convention for using boost.python.
    //using namespace boost::python;

    class_<Neuron, shared_ptr<Neuron>>("Neuron", init<string>())
	.def("addCompartment", &Neuron::addCompartment)
 	.def("init", &Neuron::init)
	.def("get_state_dim", &Neuron::get_state_dim)
        .def("connect_compartments", &Neuron::connect_compartments)
	.def("create", &Neuron::create)
	.staticmethod("create");
    ;

    // Expose the class Neuron.
    class_<Compartment, shared_ptr<Compartment>>("Compartment", init<string, double>())
	.def("initialize", &Compartment::initialize)
 	.def("add_conductance", &Compartment::add_conductance)
	.def("add_sensor", &Compartment::add_sensor)
	.def("create", &Compartment::create)
	.staticmethod("create")
    ;

    class_<NeuronModel, shared_ptr<NeuronModel>>("NeuronModel", init<vector<shared_ptr<Neuron>> , int>())
	 .def("printNeuronNames", &NeuronModel::printNeuronNames)
	 .def("run", &NeuronModel::run)
	 .def("create", &NeuronModel::create)
	 .staticmethod("create")
    ;    
    
    //class_<conductance>("conductance");
    class_<conductanceWrap,  shared_ptr<conductance>, boost::noncopyable>("conductance", no_init)
        .def("get_state_dim", pure_virtual(&conductance::get_state_dim))
        .def("i", pure_virtual(&conductance::i))
	.def("integrate", pure_virtual(&conductance::integrate))
	.def("connect", pure_virtual(&conductance::connect))
	.def("state2double", pure_virtual(&conductance::state2double))
	.def("i_Syn", pure_virtual(&conductance::i_Syn))
    ;
    

    class_<leak, bases<conductance>, shared_ptr<leak>>("leak", init<double, double>())
	.def("create", &leak::create)
	.staticmethod("create")
    ;
    class_<H_f, bases<conductance> , shared_ptr<H_f>>("H", init<double, double>())
	.def("create", &H_f::create)
	.staticmethod("create");
    class_<Ka_f, bases<conductance>, shared_ptr<Ka_f> >("Ka", init<double, double>())
	.def("create", &Ka_f::create)
	.staticmethod("create");
    class_<Mi, bases<conductance>, shared_ptr<Mi> >("Mi", init<double, double>())
	.def("create", &Mi::create)
	.staticmethod("create");
    class_<CaS, bases<conductance>, shared_ptr<CaS> >("CaS", init<double, double>())
	.def("create", &CaS::create)
	.staticmethod("create");
    class_<CaOhmic, bases<conductance>, shared_ptr<CaOhmic> >("CaOhmic", init<double, double>())
	.def("create", &CaOhmic::create)
	.staticmethod("create");
    class_<KCa, bases<conductance>, shared_ptr<KCa> >("KCa", init<double, double>())
	.def("create", &KCa::create)
	.staticmethod("create");

    class_<SynAB, bases<conductance>, shared_ptr<SynAB> >("SynAB", init<double, double, double>())
	.def("create", &SynAB::create)
	.staticmethod("create");
    class_<SynPD, bases<conductance>, shared_ptr<SynPD> >("SynPD", init<double, double, double>())
	.def("create", &SynPD::create)
	.staticmethod("create");
    class_<SynPY, bases<conductance>, shared_ptr<SynPY> >("SynPY", init<double, double, double>())
	.def("create", &SynPY::create)
	.staticmethod("create");

    class_<dclampSyn, bases<conductance>, shared_ptr<dclampSyn> >("dClampSyn", init<double, double>())
	.def("create", &dclampSyn::create)
	.staticmethod("create");

    class_<NaV_f, bases<conductance>, shared_ptr<NaV_f> >("NaV", init<double, double>())
	.def("create", &NaV_f::create)
	.staticmethod("create");
    class_<Kdr_f, bases<conductance>, shared_ptr<Kdr_f> >("Kdr", init<double, double>())
	.def("create", &Kdr_f::create)
	.staticmethod("create");

    class_<std::vector<shared_ptr<Neuron> >>("NeuronVec")
        .def(vector_indexing_suite<std::vector<shared_ptr<Neuron> >>())
    ;
    class_<std::vector<double> >("VectorOfDouble")
        .def(vector_indexing_suite<std::vector<double> >());
    
    

    class_<sensorWrap, shared_ptr<sensor>, boost::noncopyable>("sensor", no_init)
        .def("get_state_dim", pure_virtual(&sensor::get_state_dim))
	.def("integrate", pure_virtual(&sensor::integrate))
	.def("connect", pure_virtual(&sensor::connect))
	.def("state2double", pure_virtual(&sensor::state2double))
    ;
    class_<CaInt, bases<sensor>, shared_ptr<CaInt> >("CaInt", init<double>())
	.def("create", &CaInt::create)
	.staticmethod("create");
    
}

