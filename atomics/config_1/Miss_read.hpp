/*
* Author: Sebastien Cook
*	
* Template:
* Cadmium implementation of a template atomic model
*/

#ifndef __MISS_READ_HPP__
#define __MISS_READ_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <iostream>
#include <string>


#include "../../data_structures/Parsed_Request.hpp"
#include "../../data_structures/Cache_write.hpp"

using namespace cadmium;
using namespace std; 

struct Miss_read_defs{
	// can have multiple out_port and in_port 
	// port type can be data_structures or type ex: int, String...
	struct Cache_write_out: public out_port<Cache_write_t> {};
	struct Serviced_request_out: public out_port<string> {};
	struct Mem_read_request_out: public out_port<Cache_write_t> {};

	struct Request_in : public in_port<Parsed_Request_t> {};
	struct Memory_read_in : public in_port<string> {};
};

template<typename TIME>

class Miss_read{
	public:
	// port definitions
	// do as many port as initialized above
	using input_ports = tuple<typename Miss_read_defs::Request_in, Miss_read_defs::Memory_read_in>;
	using output_ports = tuple<typename Miss_read_defs::Cache_write_out, Miss_read_defs::Serviced_request_out, Miss_read_defs::Mem_read_request_out>;


	// state definition 
	struct state_type{
		//define all state variables here 
		Cache_write_t temp;
		int read;
		TIME next_internal;
	};
	state_type state; 

	Miss_read(){
		//state.variable = ...;
		state.temp = {};
		state.read = 0;
		state.next_internal = std::numeric_limits<TIME>::infinity();
	}


	// external transition
	void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
	// external transition implementation
		if((get_messages<typename Miss_read_defs::Request_in>(mbs).size()+get_messages<typename Miss_read_defs::Memory_read_in>(mbs).size())>1) 
             assert(false && "one input per time unit");

		for(const auto &x : get_messages<typename Miss_read_defs::Request_in>(mbs)){
			state.temp.tag = x.tag;
			state.temp.index = x.index;
			state.temp.offset = x.offset;
			state.read = 1;
		}

		for(const auto &x : get_messages<typename Miss_read_defs::Memory_read_in>(mbs)){
			if(state.read == 1){
				state.temp.mem = x;
				state.read = 2; //ready to send?? 
			}else{
				state.read = 0;
			}
		}
		
		state.next_internal = TIME("00:00:00:000");
	
}

	//confluence transiton --> don't touch 
    	void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
		internal_transition();
        external_transition(TIME(), move(mbs));
}

	// internal transition 
	void internal_transition(){
	//internal transition implementation
		state.next_internal = std::numeric_limits<TIME>::infinity();
}

	// output function 
	typename make_message_bags<output_ports>::type output() const {
	// output function implementation 
		typename make_message_bags<output_ports>::type bags;
		if(state.read == 1){
			get_messages<typename Miss_read_defs::Mem_read_request_out>(bags).push_back(state.temp);
			return bags;
		}else if(state.read == 2){
			get_messages<typename Miss_read_defs::Cache_write_out>(bags).push_back(state.temp);
			get_messages<typename Miss_read_defs::Serviced_request_out>(bags).push_back(state.temp.mem);
			return bags;
		} 
		
}

	// time advance function 
	TIME time_advance() const {
		return state.next_internal;
}

	// output stream --> put infor you want to analyze here 
	friend ostringstream& operator<<(ostringstream& os, const typename Miss_read<TIME>::state_type& i) {
	//change model name above
	os << "Model: Miss_read\n";
	//os << "state info: " << i.temp << "\n" << "...";
	return os; 
}

};
#endif // _MISS_READ_HPP_ 













