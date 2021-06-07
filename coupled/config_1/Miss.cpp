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
#include "../../atomics/config_1/Miss_write.hpp"
#include "../../atomics/config_1/Miss_read.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

// Coupled model port definitions --> can have multible of each
struct Miss_c1_defs{

	struct write_request_in : public in_port<Parsed_Request_t>{};
	struct read_request_in : public in_port<Parsed_Request_t>{};
	struct mem_read_in : public in_port<string> {};

	struct Mem_write_out : public out_port<Cache_write_t>{};
	struct Cache_write_out : public out_port<Cache_write_t>{};
	struct Mem_read_request_out: public out_port<Cache_write_t> {};
	struct Serviced_request_out : public out_port<string>{};
};

// atomic model instantiation --> can have multiple 
shared_ptr<dynamic::modeling::model> READ_M_c1 = dynamic::translate::make_dynamic_atomic_model<Miss_read,TIME>("READ_M_c1");

// atomic model instantiation --> can have multiple 
shared_ptr<dynamic::modeling::model> WRITE_M_c1 = dynamic::translate::make_dynamic_atomic_model<Miss_write,TIME>("WRITE_M_c1");

//Model wireing

// Port instantiation 
dynamic::modeling::Ports iports_miss_c1_in = {typeid(Miss_c1_defs::write_request_in), typeid(Miss_c1_defs::read_request_in), typeid(Miss_c1_defs::mem_read_in)};
dynamic::modeling::Ports oports_miss_c1_out = {typeid(Miss_c1_defs::Mem_write_out), typeid(Miss_c1_defs::Cache_write_out), typeid(Miss_c1_defs::Mem_read_request_out), typeid(Miss_c1_defs::Serviced_request_out)};

// Define Models within 
dynamic::modeling::Models submodels_Miss_c1 = {READ_M_c1, WRITE_M_c1};

// EIC links --> External Input Connections --> input to input 
dynamic::modeling::EICs eics_c1_Miss = {
	// Each individual connections 
	dynamic::translate::make_EIC<Miss_c1_defs::write_request_in,Miss_write_defs::Request_in>("WRITE_M_c1"),
	dynamic::translate::make_EIC<Miss_c1_defs::read_request_in,Miss_read_defs::Request_in>("READ_M_c1"),
	dynamic::translate::make_EIC<Miss_c1_defs::mem_read_in,Miss_read_defs::Memory_read_in>("READ_M_c1")
};

// EOC links --> External Output Connections --> output to output 
dynamic::modeling::EOCs eocs_c1_Miss = { 

dynamic::translate::make_EOC<Miss_write_defs::Write_mem_out,Miss_c1_defs::Mem_write_out>("WRITE_M_c1"),

dynamic::translate::make_EOC<Miss_write_defs::Write_cache_out,Miss_c1_defs::Cache_write_out>("WRITE_M_c1"),

dynamic::translate::make_EOC<Miss_read_defs::Cache_write_out,Miss_c1_defs::Cache_write_out>("READ_M_c1"),

dynamic::translate::make_EOC<Miss_read_defs::Mem_read_request_out,Miss_c1_defs::Mem_read_request_out>("READ_M_c1"),

dynamic::translate::make_EOC<Miss_read_defs::Serviced_request_out,Miss_c1_defs::Serviced_request_out>("READ_M_c1"),

dynamic::translate::make_EOC<Miss_write_defs::Serviced_request_out,Miss_c1_defs::Serviced_request_out>("WRITE_M_c1")

};

// IC links --> Internal Connections --> output to input 
dynamic::modeling::ICs ics_c1_Miss = {
};



























