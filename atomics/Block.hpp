/*
* Author: Sebastien Cook
*	
* Template:
* Cadmium implementation of a template atomic model
*/

#ifndef __BLOCK_HPP__
#define __BLOCK_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <iostream>
#include <string>

#include "../data_structures/Cache_write.hpp"
#include "../data_structures/Tag_change.hpp"

using namespace cadmium;
using namespace std; 


struct Block_defs{
	// can have multiple out_port and in_port 
	// port type can be data_structures or type ex: int, String...
	struct tag_change_out: public out_port<Tag_change_t> {};
	struct block_out: public out_port<string> {};  

	struct read_request_in : public in_port<int> {};
	struct write_request_in : public in_port<Cache_write_t> {};
};

template<typename TIME>

class Block{
	public:
	// port definitions
	// do as many port as initialized above
	using input_ports = tuple<typename Block_defs::read_request_in, Block_defs::write_request_in>;
	using output_ports = tuple<typename Block_defs::tag_change_out,Block_defs::block_out>;


	// state definition 
	struct state_type{
		//define all state variables here 
		string memory[CACHE_SIZE/BLOCK_SIZE]; 
		int read;
		int index;

		string tag;
		TIME next_internal;
	};
	state_type state; 

	Block(){
		for(int i = 0; i< CACHE_SIZE/BLOCK_SIZE; i++){
			state.memory[i] = "";
		}
		state.read = 0;
		state.tag = "";
		state.index = 0;

		state.next_internal = std::numeric_limits<TIME>::infinity();
	}


	// external transition
	void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
	// external transition implementation
		if((get_messages<typename Block_defs::read_request_in>(mbs).size()+get_messages<typename Block_defs::write_request_in>(mbs).size())>1) 
             assert(false && "one input per time unit");

		for(const auto &x : get_messages<typename Block_defs::read_request_in>(mbs)){	
			state.index = x;
			state.read = 1;

		}

		for(const auto &x : get_messages<typename Block_defs::write_request_in>(mbs)){

			state.memory[x.index] = x.mem;

			state.tag = x.tag;
			state.index = x.index;
			state.read = 0;
		
			
		}
		state.next_internal = TIME(CACHE_RESPONCE_DELAY);
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
		if(state.read == 1){ //send block
			get_messages<typename Block_defs::block_out>(bags).push_back(state.memory[state.index]);
		}else{ //update tag because it was a write
			Tag_change_t update_tag = {state.tag, state.index};
			get_messages<typename Block_defs::tag_change_out>(bags).push_back(update_tag);
		}
		return bags;
}

	// time advance function 
	TIME time_advance() const {
		return state.next_internal;
}

	// output stream --> put infor you want to analyze here 
	friend ostringstream& operator<<(ostringstream& os, const typename Block<TIME>::state_type& i) {
	//change model name above
	os << "\nMem: " << i.memory << "\n";
	//os << "read or write (1,0): " << i.read << "\ntag: " << i.tag << "\nindex" << i.index;
	return os; 
}

};
#endif // _BLOCK_HPP_ 

