/*
* Author: Sebastien Cook
*	
* Template:
* Cadmium implementation of a template atomic model
*/

#ifndef __MEMORYREQUESTMANAGER_HPP__
#define __MEMORYREQUESTMANAGER_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <iostream>
#include <string>

#include "../data_structures/Cache_write.hpp"

using namespace cadmium;
using namespace std; 

struct Memory_request_manager_defs{
	struct Read_out: public out_port<string> {};

	struct Read_request_in : public in_port<Cache_write_t> {};
	struct Write_request_in : public in_port<Cache_write_t> {};
};

template<typename TIME>

class Memory_request_manager{
	public:
	// port definitions
	// do as many port as initialized above
	using input_ports = tuple<typename Memory_request_manager_defs::Read_request_in, Memory_request_manager_defs::Write_request_in>;
	using output_ports = tuple<typename Memory_request_manager_defs::Read_out>;


	// state definition 
	struct state_type{
		int R_or_W;
		string mem_to_send;
		string mem[MEM_SIZE/MEM_BLOCK_SIZE];
		TIME next_internal;
	};
	state_type state; 

	// or default without input 
	Memory_request_manager(){
		state.R_or_W = 0;
		state.mem_to_send = "";
		for(int i = 0; i < MEM_SIZE/MEM_BLOCK_SIZE; i++){
			state.mem[i] = "";
		}
		state.next_internal = std::numeric_limits<TIME>::infinity();
	}


	// external transition
	void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
	// external transition implementation
		if((get_messages<typename Memory_request_manager_defs::Read_request_in>(mbs).size()+get_messages<typename Memory_request_manager_defs::Write_request_in>(mbs).size())>1) 
             assert(false && "one input per time unit");

		for(const auto &x : get_messages<typename Memory_request_manager_defs::Read_request_in>(mbs)){
			state.R_or_W = 0; //read
			
			
			//calc address
			int address = (64 * htoi(x.tag.c_str())) + x.index;
			if(address >= MEM_SIZE/MEM_BLOCK_SIZE){
				printf("ADDRESS OUT OF BOUNDS!! addr:%d increase mem size\n", address);
			}else{
				state.mem_to_send = state.mem[address];
			}
			
			
		}

		for(const auto &x : get_messages<typename Memory_request_manager_defs::Write_request_in>(mbs)){
			state.R_or_W = 1; //write
			int address = (64 * htoi(x.tag.c_str())) + x.index;
			if(address >= MEM_SIZE/MEM_BLOCK_SIZE){
				printf("ADDRESS OUT OF BOUNDS!! addr:%d increase mem size\n", address);
			}else{
				state.mem[address] = x.mem;
			}
		}
		state.next_internal = TIME(MEM_RESPONCE_DELAY); //MEM DELAY
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
		if(state.R_or_W == 0){
			get_messages<typename Memory_request_manager_defs::Read_out>(bags).push_back(state.mem_to_send);
			return bags;
		}
}

	// time advance function 
	TIME time_advance() const {
		return state.next_internal;
}

	// output stream --> put infor you want to analyze here 
	friend ostringstream& operator<<(ostringstream& os, const typename Memory_request_manager<TIME>::state_type& i) {
	//change model name above

	os << "state mem_to_send: " << i.mem_to_send << "\n";
	return os; 
}

};
#endif // _MEMORYREQUESTMANAGER_HPP_ 













