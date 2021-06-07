//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>

//data structures
#include "../data_structures/Tag_change.hpp"
#include "../data_structures/Cache_write.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs

//Atomic models in coupled model --> can have many
#include "../atomics/Tag.hpp"
#include "../atomics/Block.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

// Coupled model port definitions --> can have multible of each
struct Cache_defs{

	struct Tag_check_in : public in_port<Tag_change_t> {};
	struct Read_request_in : public in_port<int> {};
	struct Write_request_in : public in_port<Cache_write_t> {};

	struct Block_out: public out_port<string> {}; 
	struct Hit_or_miss_out: public out_port<int> {};
};

// atomic model instantiation --> can have multiple 
shared_ptr<dynamic::modeling::model> TAG_C = dynamic::translate::make_dynamic_atomic_model<Tag,TIME>("TAG_C");

shared_ptr<dynamic::modeling::model> BLOCK = dynamic::translate::make_dynamic_atomic_model<Block,TIME>("BLOCK");

//Model wireing

// Port instantiation 
dynamic::modeling::Ports iports_cache_in = {typeid(Cache_defs::Tag_check_in),typeid(Cache_defs::Read_request_in),typeid(Cache_defs::Write_request_in)};
dynamic::modeling::Ports oports_cache_out = {typeid(Cache_defs::Block_out), typeid(Cache_defs::Hit_or_miss_out)};

// Define Models within 
dynamic::modeling::Models submodels_cache = {TAG_C, BLOCK};

// EIC links --> External Input Connections --> input to input 
dynamic::modeling::EICs eics_cache = {
	// Each individual connections 
	dynamic::translate::make_EIC<Cache_defs::Tag_check_in,Tag_defs::tag_check_in>("TAG_C"),

dynamic::translate::make_EIC<Cache_defs::Write_request_in,Block_defs::write_request_in>("BLOCK"),

dynamic::translate::make_EIC<Cache_defs::Read_request_in,Block_defs::read_request_in>("BLOCK")
};

// EOC links --> External Output Connections --> output to output 
dynamic::modeling::EOCs eocs_cache = { 

dynamic::translate::make_EOC<Block_defs::block_out,Cache_defs::Block_out>("BLOCK"),
// Can connect many to a single input port or if many input ports
dynamic::translate::make_EOC<Tag_defs::hit_or_miss_out,Cache_defs::Hit_or_miss_out>("TAG_C")
};

// IC links --> Internal Connections --> output to input 
dynamic::modeling::ICs ics_cache = {

dynamic::translate::make_IC<Block_defs::tag_change_out,Tag_defs::tag_change_in>("BLOCK","TAG_C")
};



























