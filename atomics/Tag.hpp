/*
* Author: Sebastien Cook
*	
* Template:
* Cadmium implementation of a template atomic model
*/

#ifndef __TAG_HPP__
#define __TAG_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <iostream>
#include <string>


#include "../data_structures/Tag_change.hpp"

using namespace cadmium;
using namespace std; 

struct Tag_defs{
	
	struct hit_or_miss_out: public out_port<int> {};
	struct tag_check_in : public in_port<Tag_change_t> {};
	struct tag_change_in : public in_port<Tag_change_t> {};
};

template<typename TIME>

class Tag{
	public:
	// port definitions
	// do as many port as initialized above
	using input_ports = tuple<typename Tag_defs::tag_check_in, Tag_defs::tag_change_in>;
	using output_ports = tuple<typename Tag_defs::hit_or_miss_out>;


	// state definition 
	struct state_type{
		//define all state variables here
		string tag[CACHE_SIZE/BLOCK_SIZE]; 
		int is_hit;
		
		int index;
		string input_tag;
		int respond;
		TIME next_internal;
	};
	state_type state; 

	// or default without input 
	Tag(){
		for(int i = 0; i< CACHE_SIZE/BLOCK_SIZE; i++){
			state.tag[i] = "";
		}
		state.is_hit = 0;
		state.respond = 0;
		state.index = 0;
		state.input_tag = "";
		state.next_internal = std::numeric_limits<TIME>::infinity();
	}


	// external transition
	void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
	// external transition implementation
		if((get_messages<typename Tag_defs::tag_check_in>(mbs).size()+get_messages<typename Tag_defs::tag_change_in>(mbs).size())>1) 
             assert(false && "one input per time unit");
		
		for(const auto &x : get_messages<typename Tag_defs::tag_check_in>(mbs)){
			state.is_hit = 0;
			state.respond = 1;
			
			if(x.tag.compare(state.tag[x.index])==0){
				state.is_hit = 1;
			}
			state.index = x.index;
			state.input_tag = x.tag;	
		}
	
		for(const auto &x : get_messages<typename Tag_defs::tag_change_in>(mbs)){
			state.respond = 0;
			state.tag[x.index] = x.tag;
		}

		state.next_internal = TIME(CACHE_RESPONCE_DELAY); //CACHE DELAY
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
		if(state.respond == 1){
			get_messages<typename Tag_defs::hit_or_miss_out>(bags).push_back(state.is_hit);
			return bags;
		}
}

	// time advance function 
	TIME time_advance() const {
		return state.next_internal;
}

	// output stream --> put infor you want to analyze here 
	friend ostringstream& operator<<(ostringstream& os, const typename Tag<TIME>::state_type& i) {
	//change model name above
	os << "Model: Tag\n" << "Given tag: "<< i.input_tag << " Tag in cache: " << i.tag[i.index] << "\n";
	//os << "sending: " << i.respond << "\n" << "is hit: " << i.is_hit << "\n";
	return os; 
}

};
#endif // _TAG_HPP_ 













