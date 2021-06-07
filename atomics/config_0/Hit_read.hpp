/*
* Author: Sebastien Cook
*	
* Template:
* Cadmium implementation of a template atomic model
*/

#ifndef __HIT_READ_HPP__
#define __HIT_READ_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <iostream>
#include <string>


#include "../../data_structures/Parsed_Request.hpp"

using namespace cadmium;
using namespace std; 

struct Hit_read_defs{
	// can have multiple out_port and in_port 
	// port type can be data_structures or type ex: int, String...
	struct Read_request_out: public out_port<int> {};
	struct Serviced_request_out: public out_port<string> {};
	struct Request_in : public in_port<Parsed_Request_t> {};
	struct Cache_read_in : public in_port<string> {};
};

template<typename TIME>

class Hit_read{
	public:
	// port definitions
	// do as many port as initialized above
	using input_ports = tuple<typename Hit_read_defs::Request_in, Hit_read_defs::Cache_read_in>;
	using output_ports = tuple<typename Hit_read_defs::Read_request_out, Hit_read_defs::Serviced_request_out>;


	// state definition 
	struct state_type{

		//define all state variables here 
		int index;
		int read;
		string memTemp;
 		TIME next_internal;
	};
	state_type state; 

	// or default without input 
	Hit_read(){

		state.index = 0;
		state.read = 0;
		state.memTemp = "";
		state.next_internal = std::numeric_limits<TIME>::infinity();
	}


	// external transition
	void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
	// external transition implementation
		if((get_messages<typename Hit_read_defs::Request_in>(mbs).size()+get_messages<typename Hit_read_defs::Cache_read_in>(mbs).size())>1) 
             assert(false && "one input per time unit");

		for(const auto &x : get_messages<typename Hit_read_defs::Request_in>(mbs)){
			state.index = x.index;
			state.read = 1;

		}

		for(const auto &x : get_messages<typename Hit_read_defs::Cache_read_in>(mbs)){
			if(state.read == 1){
				state.memTemp = x;
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
			get_messages<typename Hit_read_defs::Read_request_out>(bags).push_back(state.index);
			return bags;
		}else if(state.read == 2){
			get_messages<typename Hit_read_defs::Serviced_request_out>(bags).push_back(state.memTemp);
			return bags;
		} 
}

	// time advance function 
	TIME time_advance() const {
		return state.next_internal;
}

	// output stream --> put infor you want to analyze here 
	friend ostringstream& operator<<(ostringstream& os, const typename Hit_read<TIME>::state_type& i) {
	//change model name above
	os << "Model: Hit_read\n";
	//os << "state read: " << i.read << "\nindex: " << i.index << "\nread word: " << i.memTemp;
	return os; 
}

};
#endif // _HIT_READ_HPP_ 













