//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>

//Data sturctures needed --> can have many
#include "../../data_structures/Parsed_Request.hpp"
#include "../../data_structures/Cache_write.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs

//Atomic models in coupled model --> can have many
#include "../../atomics/config_0/Hit_write.hpp"
#include "../../atomics/config_0/Hit_read.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

// Coupled model port definitions --> can have multible of each
struct Hit_c0_defs{
	struct write_request_in : public in_port<Parsed_Request_t>{};
	struct read_request_in : public in_port<Parsed_Request_t>{};
	struct cache_read_in : public in_port<string> {};

	struct Mem_write_out : public out_port<Cache_write_t>{};
	struct Cache_write_out : public out_port<Cache_write_t>{};
	struct Read_request_out: public out_port<int> {};
	struct Serviced_request_out : public out_port<string>{};
};

// atomic model instantiation --> can have multiple 
shared_ptr<dynamic::modeling::model> READ_H_c0 = dynamic::translate::make_dynamic_atomic_model<Hit_read,TIME>("READ_H_c0");

// atomic model instantiation --> can have multiple 
shared_ptr<dynamic::modeling::model> WRITE_H_c0 = dynamic::translate::make_dynamic_atomic_model<Hit_write,TIME>("WRITE_H_c0");

//Model wireing

// Port instantiation 
dynamic::modeling::Ports iports_hit_c0_in = {typeid(Hit_c0_defs::write_request_in), typeid(Hit_c0_defs::read_request_in), typeid(Hit_c0_defs::cache_read_in)};
dynamic::modeling::Ports oports_hit_c0_out = {typeid(Hit_c0_defs::Cache_write_out), typeid(Hit_c0_defs::Read_request_out), typeid(Hit_c0_defs::Serviced_request_out)};

// Define Models within 
dynamic::modeling::Models submodels_Hit_c0 = {READ_H_c0, WRITE_H_c0};

// EIC links --> External Input Connections --> input to input 
dynamic::modeling::EICs eics_c0_Hit = {
	// Each individual connections 
	dynamic::translate::make_EIC<Hit_c0_defs::write_request_in,Hit_write_defs::Request_in>("WRITE_H_c0"),
// Can connect many to a single input port or if many input ports
dynamic::translate::make_EIC<Hit_c0_defs::read_request_in,Hit_read_defs::Request_in>("READ_H_c0"),

dynamic::translate::make_EIC<Hit_c0_defs::cache_read_in,Hit_read_defs::Cache_read_in>("READ_H_c0")
};

// EOC links --> External Output Connections --> output to output 
dynamic::modeling::EOCs eocs_c0_Hit = { 

dynamic::translate::make_EOC<Hit_write_defs::Cache_write_out,Hit_c0_defs::Cache_write_out>("WRITE_H_c0"),

//dynamic::translate::make_EOC<Hit_write_defs::Mem_write_out,Hit_c0_defs::Mem_write_out>("WRITE_H_c0"),

dynamic::translate::make_EOC<Hit_read_defs::Read_request_out,Hit_c0_defs::Read_request_out>("READ_H_c0"),

dynamic::translate::make_EOC<Hit_read_defs::Serviced_request_out,Hit_c0_defs::Serviced_request_out>("READ_H_c0"),

dynamic::translate::make_EOC<Hit_write_defs::Serviced_request_out,Hit_c0_defs::Serviced_request_out>("WRITE_H_c0")
};

// IC links --> Internal Connections --> output to input 
dynamic::modeling::ICs ics_c0_Hit = {
};



























