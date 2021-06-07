/*
* Author: Sebastien Cook
*	
* Template:
* Cadmium implementation of a template atomic model
*/

#ifndef __HIT_WRITE_HPP__
#define __HIT_WRITE_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <iostream>
#include <string>

#include "../../data_structures/Parsed_Request.hpp"
#include "../../data_structures/Cache_write.hpp"

using namespace cadmium;
using namespace std; 

struct Hit_write_defs{
	// can have multiple out_port and in_port 
	// port type can be data_structures or type ex: int, String...
	struct Cache_write_out: public out_port<Cache_write_t> {};
	struct Serviced_request_out: public out_port<string> {};
	struct Request_in : public in_port<Parsed_Request_t> {};
};

template<typename TIME>

class Hit_write{
	public:
	// port definitions
	// do as many port as initialized above
	using input_ports = tuple<typename Hit_write_defs::Request_in>;
	using output_ports = tuple<typename Hit_write_defs::Cache_write_out, Hit_write_defs::Serviced_request_out>;


	// state definition 
	struct state_type{
		//define all state variables here 
		Cache_write_t temp;
		int send;
		TIME next_internal;
	};
	state_type state; 

	// or default without input 
	Hit_write(){
		//state.variable = ...;
		state.temp = {};
		state.send = 0;
		state.next_internal = std::numeric_limits<TIME>::infinity();
	}


	// external transition
	void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
	// external transition implementation

		if((get_messages<typename Hit_write_defs::Request_in>(mbs).size())>1) 
             assert(false && "one input per time unit");

		for(const auto &x : get_messages<typename Hit_write_defs::Request_in>(mbs)){
			state.temp.tag = x.tag;
			state.temp.index = x.index;
			state.temp.offset = x.offset;
			state.temp.mem = x.mem;
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
		
		if(state.send == 0){
			state.next_internal = TIME(CACHE_RESPONCE_DELAY); 
			state.send = 1;
		}else{
			state.next_internal = std::numeric_limits<TIME>::infinity();
			state.send = 0;
		}
}

	// output function 
	typename make_message_bags<output_ports>::type output() const {
	// output function implementation 
		typename make_message_bags<output_ports>::type bags;
		if(state.send == 0){
			get_messages<typename Hit_write_defs::Cache_write_out>(bags).push_back(state.temp);
		}else{
			get_messages<typename Hit_write_defs::Serviced_request_out>(bags).push_back("done");
		}
		return bags;
}

	// time advance function 
	TIME time_advance() const {
		return state.next_internal;
}

	// output stream --> put infor you want to analyze here 
	friend ostringstream& operator<<(ostringstream& os, const typename Hit_write<TIME>::state_type& i) {
	//change model name above
	os << "Model: Hit_write\n";
	//os << "state writing: " << i.temp.mem << " To index: " << i.temp.index;
	return os; 
}

};
#endif // _HIT_WRITE_HPP_ 













