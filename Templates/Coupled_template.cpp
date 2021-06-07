//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>

//Data sturctures needed --> can have many
#include "../data_structures/Data_structure_name.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs

//Atomic models in coupled model --> can have many
#include "../atomics/Atomic_model_name.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

// Coupled model port definitions --> can have multible of each
struct Model_name_defs{
	struct Port_name_In : public in_port<DATA_TYPE>{};
	struct Port_name_Out : public out_port<DATA_TYPE>{};
};

// atomic model instantiation --> can have multiple 
shared_ptr<dynamic::modeling::model> Temp_Model_Name = dynamic::translate::make_dynamic_atomic_model<Atomic_Model_Name,TIME>("Temp_Model_Name");

// if the model constructor requires an inpute --> change int to val input type
shared_ptr<dynamic::modeling::model> Temp_Model_Name = dynamic::translate::make_dynamic_atomic_model<InTrainQueue,TIME,int>("Temp_Model_Name",1);

//Model wireing

// Port instantiation 
dynamic::modeling::Ports Temp_Port_name_in = {typeid(Model_name_defs::Port_name_IN)};
dynamic::modeling::Ports Temp_Port_name_out = {typeid(Model_name_defs::Port_name_Out), typeid...};

// Define Models within 
dynamic::modeling::Models submodels_Model_Name = {Temp_Model_Name, ..., Temp_Model_Names};

// EIC links --> External Input Connections --> input to input 
dynamic::modeling::EICs eics_Model_Name = {
	// Each individual connections 
	dynamic::translate::make_EIC<Model_name_defs::Port_name_In,Atomic_Model_nam_defs::Atomic_Port_name_In>("Temp_name_of_atomic_model_used"),
// Can connect many to a single input port or if many input ports
dynamic::...
};

// EOC links --> External Output Connections --> output to output 
dynamic::modeling::EOCs eocs_Model_Name = { 

dynamic::translate::make_EOC<Atomic_Model_nam_defs::Atomic_Port_name_Out,Model_name_defs::Port_name_Out>("Temp_name_of_atomic_model_used"),
// Can connect many to a single input port or if many input ports
dynamic::...
};

// IC links --> Internal Connections --> output to input 
dynamic::modeling::ICs ics_Model_Name = {

dynamic::translate::make_IC<Atomic_Model_nam_defs::Atomic_Port_name_Out,Atomic_Model_nam_defs::Atomic_Port_name_In>("Temp_name_of_atomic_model_used","Temp_name_of_atomic_model_used"),

dynamic::...
};



























