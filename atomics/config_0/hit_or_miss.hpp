/*
* Author: Sebastien Cook
*	
* Template:
* Cadmium implementation of a template atomic model
*/

#ifndef __HIT_OR_MISS_HPP__
#define __HIT_OR_MISS_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <iostream>
#include <string>

#include "../../data_structures/Parsed_Request.hpp"
#include "../../data_structures/Tag_change.hpp"

using namespace cadmium;
using namespace std; 

struct hit_or_miss_defs{
	struct hit_read_out: public out_port<Parsed_Request_t> {};  //if hit, send to hit block 
	struct hit_write_out: public out_port<Parsed_Request_t> {};  //if hit, send to hit block 
	struct miss_read_out: public out_port<Parsed_Request_t> {}; //if miss send to miss block 
	struct miss_write_out: public out_port<Parsed_Request_t> {}; //if miss send to miss block 
	struct tag_check_out: public out_port<Tag_change_t> {}; //ask cash if its a hit

	struct hit_miss_in: public in_port<int> {}; //connected to cash for hit miss answer 
	struct Request_in : public in_port<Parsed_Request_t> {}; //rquest to check 
};

template<typename TIME>

class hit_or_miss{
	public:
	// port definitions
	// do as many port as initialized above
	using input_ports = tuple<typename hit_or_miss_defs::Request_in, hit_or_miss_defs::hit_miss_in>;
	
	using output_ports = tuple<typename hit_or_miss_defs::hit_read_out, hit_or_miss_defs::hit_write_out, hit_or_miss_defs::miss_read_out, hit_or_miss_defs::miss_write_out, hit_or_miss_defs::tag_check_out>;


	// state definition 
	struct state_type{
		//bool to determine if its the active config
		bool active;

		int hit_miss;
		int num_hit;
		int num_miss;
		Parsed_Request_t temp; 
		int check_if_in_cash;
		TIME next_internal;
	};
	state_type state; 
 
	hit_or_miss(){
		state.active = false;
		
		state.hit_miss = 0;
		state.temp = {};
		state.check_if_in_cash = 0;

		state.num_hit = 0;
		state.num_miss = 0;	
	
		state.next_internal = std::numeric_limits<TIME>::infinity();
	}


	// external transition
	void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
	// external transition implementation
	if((get_messages<typename hit_or_miss_defs::Request_in>(mbs).size()+get_messages<typename hit_or_miss_defs::hit_miss_in>(mbs).size())>1) 
             assert(false && "one input per time unit");
	
		for(const auto &x : get_messages<typename hit_or_miss_defs::hit_miss_in>(mbs)){
			if(state.active){
				state.hit_miss = x;
				state.check_if_in_cash = 0;
				if(x == 0)//miss
					state.num_miss++;
				else
					state.num_hit++;
				state.active = false;
				state.next_internal = TIME("00:00:00:000");
			}else{
				state.next_internal = std::numeric_limits<TIME>::infinity();
			}
		}
		
		for(const auto &x : get_messages<typename hit_or_miss_defs::Request_in>(mbs)){
			state.temp = x;
			state.check_if_in_cash = 1;	
			state.active = true;
			state.next_internal = TIME("00:00:00:000");
		}

		
		
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
		if(state.check_if_in_cash == 1){
			Tag_change_t check;
			check.tag = state.temp.tag;
			check.index = state.temp.index;
			get_messages<typename hit_or_miss_defs::tag_check_out>(bags).push_back(check);
		}else{
			if(state.hit_miss == 1){
				if(state.temp.R_W == 0){
					get_messages<typename hit_or_miss_defs::hit_read_out>(bags).push_back(state.temp);
				}else{
					get_messages<typename hit_or_miss_defs::hit_write_out>(bags).push_back(state.temp);
				}
			}else{
				if(state.temp.R_W == 0){
					get_messages<typename hit_or_miss_defs::miss_read_out>(bags).push_back(state.temp);
				}else{
					get_messages<typename hit_or_miss_defs::miss_write_out>(bags).push_back(state.temp);
				}
			}
			
		}
		return bags;
}

	// time advance function 
	TIME time_advance() const {
		return state.next_internal;
}

	// output stream --> put infor you want to analyze here 
	friend ostringstream& operator<<(ostringstream& os, const typename hit_or_miss<TIME>::state_type& i) {
	//change model name above

	os << "\nHITS: " << i.num_hit << "\nMISSES: " << i.num_miss << "\n";
	return os; 
}

};
#endif // _HIT_OR_MISS_HPP_ 













