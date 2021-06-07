//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>

//Data sturctures needed --> can have many
#include "../data_structures/Parsed_Request.hpp"
#include "../data_structures/Cache_write.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs

//Atomic models in coupled model --> can have many
#include "../atomics/config_1/hit_or_miss.hpp"
#include "../coupled/config_1/Hit.cpp"
#include "../coupled/config_1/Miss.cpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

// Coupled model port definitions --> can have multible of each
struct Config_1_defs{
	struct Parsed_request_in : public in_port<Parsed_Request_t>{};
	struct hit_miss_in: public in_port<int> {};
	struct mem_read_in : public in_port<string> {};
	struct cache_read_in : public in_port<string> {};

	struct Cache_write_out : public out_port<Cache_write_t>{};
	struct Mem_write_out : public out_port<Cache_write_t>{};
	struct Cache_read_request_out: public out_port<int> {};
	struct Mem_read_request_out: public out_port<Cache_write_t> {};
	struct tag_check_out: public out_port<Tag_change_t> {};
	struct Serviced_request_out : public out_port<string>{};
};

// atomic model instantiation --> can have multiple 
shared_ptr<dynamic::modeling::model> HIT_MISS_c1 = dynamic::translate::make_dynamic_atomic_model<hit_or_miss,TIME>("HIT_MISS_c1");

// atomic model instantiation --> can have multiple 
shared_ptr<dynamic::modeling::coupled<TIME>> HIT_c1 = make_shared<dynamic::modeling::coupled<TIME>>("HIT_c1", submodels_Hit_c1, iports_hit_c1_in, oports_hit_c1_out, eics_c1_Hit, eocs_c1_Hit, ics_c1_Hit);

// atomic model instantiation --> can have multiple 
shared_ptr<dynamic::modeling::coupled<TIME>> MISS_c1 = make_shared<dynamic::modeling::coupled<TIME>>("MISS_c1", submodels_Miss_c1, iports_miss_c1_in, oports_miss_c1_out, eics_c1_Miss, eocs_c1_Miss, ics_c1_Miss);

//Model wireing

// Port instantiation 
dynamic::modeling::Ports iports_config_1_in = {typeid(Config_1_defs::Parsed_request_in), typeid(Config_1_defs::hit_miss_in), typeid(Config_1_defs::mem_read_in), typeid(Config_1_defs::cache_read_in)};

dynamic::modeling::Ports oports_config_1_out = {typeid(Config_1_defs::Cache_write_out), typeid(Config_1_defs::Mem_write_out), typeid(Config_1_defs::Cache_read_request_out), typeid(Config_1_defs::Mem_read_request_out), typeid(Config_1_defs::tag_check_out), typeid(Config_1_defs::Serviced_request_out)};

// Define Models within 
dynamic::modeling::Models submodels_config_1 = {HIT_MISS_c1, HIT_c1, MISS_c1};

// EIC links --> External Input Connections --> input to input 
dynamic::modeling::EICs eics_config_1 = {
	// Each individual connections 
	dynamic::translate::make_EIC<Config_1_defs::Parsed_request_in,hit_or_miss_defs::Request_in>("HIT_MISS_c1"),

dynamic::translate::make_EIC<Config_1_defs::hit_miss_in,hit_or_miss_defs::hit_miss_in>("HIT_MISS_c1"),

dynamic::translate::make_EIC<Config_1_defs::mem_read_in,Miss_c1_defs::mem_read_in>("MISS_c1"),

dynamic::translate::make_EIC<Config_1_defs::cache_read_in,Hit_c1_defs::cache_read_in>("HIT_c1")
};

// EOC links --> External Output Connections --> output to output 
dynamic::modeling::EOCs eocs_config_1 = { 

dynamic::translate::make_EOC<hit_or_miss_defs::tag_check_out,Config_1_defs::tag_check_out>("HIT_MISS_c1"),

dynamic::translate::make_EOC<Hit_c1_defs::Cache_write_out,Config_1_defs::Cache_write_out>("HIT_c1"),

dynamic::translate::make_EOC<Hit_c1_defs::Read_request_out,Config_1_defs::Cache_read_request_out>("HIT_c1"),

dynamic::translate::make_EOC<Hit_c1_defs::Serviced_request_out,Config_1_defs::Serviced_request_out>("HIT_c1"),

dynamic::translate::make_EOC<Miss_c1_defs::Mem_write_out,Config_1_defs::Mem_write_out>("MISS_c1"),

dynamic::translate::make_EOC<Miss_c1_defs::Cache_write_out,Config_1_defs::Cache_write_out>("MISS_c1"),

dynamic::translate::make_EOC<Miss_c1_defs::Mem_read_request_out,Config_1_defs::Mem_read_request_out>("MISS_c1"),

dynamic::translate::make_EOC<Miss_c1_defs::Serviced_request_out,Config_1_defs::Serviced_request_out>("MISS_c1")

};

// IC links --> Internal Connections --> output to input 
dynamic::modeling::ICs ics_config_1 = {

dynamic::translate::make_IC<hit_or_miss_defs::hit_read_out,Hit_c1_defs::read_request_in>("HIT_MISS_c1","HIT_c1"),

dynamic::translate::make_IC<hit_or_miss_defs::hit_write_out,Hit_c1_defs::write_request_in>("HIT_MISS_c1","HIT_c1"),

dynamic::translate::make_IC<hit_or_miss_defs::miss_read_out,Miss_c1_defs::read_request_in>("HIT_MISS_c1","MISS_c1"),

dynamic::translate::make_IC<hit_or_miss_defs::miss_write_out,Miss_c1_defs::write_request_in>("HIT_MISS_c1","MISS_c1")
};



























