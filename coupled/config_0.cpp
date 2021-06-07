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
#include "../atomics/config_0/hit_or_miss.hpp"
#include "../coupled/config_0/Hit.cpp"
#include "../coupled/config_0/Miss.cpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

// Coupled model port definitions --> can have multible of each
struct Config_0_defs{
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
shared_ptr<dynamic::modeling::model> HIT_MISS_c0 = dynamic::translate::make_dynamic_atomic_model<hit_or_miss,TIME>("HIT_MISS_c0");

// atomic model instantiation --> can have multiple 
shared_ptr<dynamic::modeling::coupled<TIME>> HIT_c0 = make_shared<dynamic::modeling::coupled<TIME>>("HIT_c0", submodels_Hit_c0, iports_hit_c0_in, oports_hit_c0_out, eics_c0_Hit, eocs_c0_Hit, ics_c0_Hit);

// atomic model instantiation --> can have multiple 
shared_ptr<dynamic::modeling::coupled<TIME>> MISS_c0 = make_shared<dynamic::modeling::coupled<TIME>>("MISS_c0", submodels_Miss_c0, iports_miss_c0_in, oports_miss_c0_out, eics_c0_Miss, eocs_c0_Miss, ics_c0_Miss);

//Model wireing

// Port instantiation 
dynamic::modeling::Ports iports_config_0_in = {typeid(Config_0_defs::Parsed_request_in), typeid(Config_0_defs::hit_miss_in), typeid(Config_0_defs::mem_read_in), typeid(Config_0_defs::cache_read_in)};

dynamic::modeling::Ports oports_config_0_out = {typeid(Config_0_defs::Cache_write_out), typeid(Config_0_defs::Mem_write_out), typeid(Config_0_defs::Cache_read_request_out), typeid(Config_0_defs::Mem_read_request_out), typeid(Config_0_defs::tag_check_out), typeid(Config_0_defs::Serviced_request_out)};

// Define Models within 
dynamic::modeling::Models submodels_config_0 = {HIT_MISS_c0, HIT_c0, MISS_c0};

// EIC links --> External Input Connections --> input to input 
dynamic::modeling::EICs eics_config_0 = {
	// Each individual connections 
	dynamic::translate::make_EIC<Config_0_defs::Parsed_request_in,hit_or_miss_defs::Request_in>("HIT_MISS_c0"),

dynamic::translate::make_EIC<Config_0_defs::hit_miss_in,hit_or_miss_defs::hit_miss_in>("HIT_MISS_c0"),

dynamic::translate::make_EIC<Config_0_defs::mem_read_in,Miss_c0_defs::mem_read_in>("MISS_c0"),

dynamic::translate::make_EIC<Config_0_defs::cache_read_in,Hit_c0_defs::cache_read_in>("HIT_c0")
};

// EOC links --> External Output Connections --> output to output 
dynamic::modeling::EOCs eocs_config_0 = { 

dynamic::translate::make_EOC<hit_or_miss_defs::tag_check_out,Config_0_defs::tag_check_out>("HIT_MISS_c0"),

dynamic::translate::make_EOC<Hit_c0_defs::Cache_write_out,Config_0_defs::Cache_write_out>("HIT_c0"),

dynamic::translate::make_EOC<Hit_c0_defs::Read_request_out,Config_0_defs::Cache_read_request_out>("HIT_c0"),

dynamic::translate::make_EOC<Hit_c0_defs::Serviced_request_out,Config_0_defs::Serviced_request_out>("HIT_c0"),

dynamic::translate::make_EOC<Miss_c0_defs::Mem_write_out,Config_0_defs::Mem_write_out>("MISS_c0"),

dynamic::translate::make_EOC<Miss_c0_defs::Cache_write_out,Config_0_defs::Cache_write_out>("MISS_c0"),

dynamic::translate::make_EOC<Miss_c0_defs::Mem_read_request_out,Config_0_defs::Mem_read_request_out>("MISS_c0"),

dynamic::translate::make_EOC<Miss_c0_defs::Serviced_request_out,Config_0_defs::Serviced_request_out>("MISS_c0")

};

// IC links --> Internal Connections --> output to input 
dynamic::modeling::ICs ics_config_0 = {

dynamic::translate::make_IC<hit_or_miss_defs::hit_read_out,Hit_c0_defs::read_request_in>("HIT_MISS_c0","HIT_c0"),

dynamic::translate::make_IC<hit_or_miss_defs::hit_write_out,Hit_c0_defs::write_request_in>("HIT_MISS_c0","HIT_c0"),

dynamic::translate::make_IC<hit_or_miss_defs::miss_read_out,Miss_c0_defs::read_request_in>("HIT_MISS_c0","MISS_c0"),

dynamic::translate::make_IC<hit_or_miss_defs::miss_write_out,Miss_c0_defs::write_request_in>("HIT_MISS_c0","MISS_c0")
};



























