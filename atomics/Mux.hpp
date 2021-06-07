/*
* Author: Sebastien Cook
*	
* Template:
* Cadmium implementation of a template atomic model
*/

#ifndef __MUX_HPP__
#define __MUX_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include "../data_structures/Parsed_Request.hpp"

using namespace cadmium;
using namespace std; 

struct Mux_defs{
	// can have multiple out_port and in_port 
	// port type can be data_structures or type ex: int, String...
	struct Config0_out: public out_port<Parsed_Request_t> {};
	struct Config1_out: public out_port<Parsed_Request_t> {};
	// can keep adding configuration for every config you want connected 
	struct Request_in : public in_port<Parsed_Request_t> {};
	struct Select_in : public in_port<int> {};
};

template<typename TIME>

class Mux{
	public:
	// port definitions
	// do as many port as initialized above
	using output_ports = tuple<typename Mux_defs::Config0_out, Mux_defs::Config1_out>;
	using input_ports = tuple<typename Mux_defs::Request_in, Mux_defs::Select_in>;


	// state definition 
	struct state_type{
		//define all state variables here 
		Parsed_Request_t temp; 
		int selected;
		int num_transfered;
		TIME next_internal;
	};
	state_type state; 

	// or default without input 
	Mux(){
		//default 0
		state.num_transfered = 0;
		state.selected = 1;
		state.next_internal = std::numeric_limits<TIME>::infinity();
	}

	// or default without input 
	Mux(int select){
		//default 0
		state.num_transfered = 0;
		if(select == 0 || select == 1){
			state.selected = select;
		}else{ //default select
			printf("invalid configuration selected. default 0 selected\n");
			state.selected = 0;
		} 
		state.next_internal = std::numeric_limits<TIME>::infinity();
	}

	// external transition
	void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
	// external transition implementation
		if((get_messages<typename Mux_defs::Select_in>(mbs).size()+get_messages<typename Mux_defs::Request_in>(mbs).size())>1) 
             		assert(false && "one input per time unit");
	
		for(const auto &x : get_messages<typename Mux_defs::Select_in>(mbs)){
			state.selected = x;
		}

		for(const auto &x : get_messages<typename Mux_defs::Request_in>(mbs)){
			state.temp = x;
			state.num_transfered++;
		}
		state.next_internal = TIME(MUX_RESPONCE_DELAY); //MUX DELAY
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
		if(state.selected == 0){
			get_messages<typename Mux_defs::Config0_out>(bags).push_back(state.temp);
		}else if(state.selected == 1){
			get_messages<typename Mux_defs::Config1_out>(bags).push_back(state.temp);
		}else{}
		return bags;
}

	// time advance function 
	TIME time_advance() const {
		return state.next_internal;
}

	// output stream --> put infor you want to analyze here 
	friend ostringstream& operator<<(ostringstream& os, const typename Mux<TIME>::state_type& i) {
	//change model name above

	os << "selected configuration: " << i.selected << " Requests transfered: " << i.num_transfered << "\n";
	return os; 
}

};
#endif // _MUX_HPP_ 













