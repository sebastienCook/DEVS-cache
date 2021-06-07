/*
* Author: Sebastien Cook
*	
* Template:
* Cadmium implementation of a template atomic model
*/

#ifndef __MODELNAME_HPP__
#define __MODELNAME_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <Libraries Needed> 

#include "../data_structures/data_strucutre_name.hpp"

using namespace cadmium;
using namespace std; 

struct Model_name_defs{
	// can have multiple out_port and in_port 
	// port type can be data_structures or type ex: int, String...
	struct Port_name_out: public out_port<data_structure_t> {};
	struct Port_name_in : public in_port<data_structure_t> {};
};

template<typename TIME>

class Model_name{
	public:
	// port definitions
	// do as many port as initialized above
	using input_ports = tuple<typename Model_name_defs::Port_name_in, Model_name_defs::if_more_inputs>;
	using output_ports = tuple<typename Model_name_defs::Port_name_out>;


	// state definition 
	struct state_type{
		//define all state variables here 
	};
	state_type state; 

	// default constructor 
	Model_name() = default; 
	// or default without input 
	Model_name(){
		//state.variable = ...;
	}
	// constructor with variables
	Model_name(int example){
	}


	// external transition
	void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
	// external transition implementation
}

	//confluence transiton --> don't touch 
    	void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
		internal_transition();
        external_transition(TIME(), move(mbs));
}

	// internal transition 
	void internal_transition(){
	//internal transition implementation
}

	// output function 
	typename make_message_bags<output_ports>::type output() const {
	// output function implementation 
}

	// time advance function 
	TIME time_advance() const {
		return state.next_internal;
}

	// output stream --> put infor you want to analyze here 
	friend ostringstream& operator<<(ostringstream& os, const typename Model_name<TIME>::state_type& i) {
	//change model name above

	os << "state info: " << i.variable << "\n" << "...";
	return os; 
}

};
#endif // _MODELNAME_HPP_ 













