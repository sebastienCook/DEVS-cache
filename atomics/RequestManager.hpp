/*
* Author: Sebastien Cook
*	
* RequestManager:
* Cadmium implementation of a request receiver atomic model
* This model receives a CPU request and parses the address into tag,index,offset
*/

#ifndef __RequestManager_HPP__
#define __RequestManager_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include "../data_structures/CPU_Request.hpp"
#include "../data_structures/Parsed_Request.hpp"

using namespace cadmium;
using namespace std; 

struct RequestManager_defs{
	// can have multiple out_port and in_port 
	// port type can be data_structures or type ex: int, String...
	struct Parsed_Request_out: public out_port<Parsed_Request_t> {};
	struct CPU_Request_in : public in_port<CPU_Request_t> {};
	struct Serviced_request_in : public in_port<string> {};
};

template<typename TIME>

class RequestManager{
	public:
	// port definitions
	// do as many port as initialized above
	using input_ports = tuple<typename RequestManager_defs::CPU_Request_in, RequestManager_defs::Serviced_request_in>;
	using output_ports = tuple<typename RequestManager_defs::Parsed_Request_out>;


	// state definition 
	struct state_type{
		//define all state variables here 

		//temp values
		int offset;
		int index;
		int R_W;
		char *tag;
		string mem;
		TIME next_internal;
		int send;
		int num_requests;
		typedef list<Parsed_Request_t> parsed_requests;
		parsed_requests request;
	};
	state_type state; 

	// default constructor 
	//RequestManager() = default; 
	// or default without input 
	RequestManager(){

		state.send = 1;
		state.num_requests = 0;
		state.next_internal = std::numeric_limits<TIME>::infinity();

		state.request.erase(state.request.begin(), state.request.end());
	}
	


	// external transition
	void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
	// external transition implementation
		/*if((get_messages<typename RequestManager_defs::CPU_Request_in>(mbs).size() + get_messages<typename RequestManager_defs::Serviced_request_in>(mbs).size())>1) 
             		assert(false && "one input per time unit");*/

		for(const auto &x : get_messages<typename RequestManager_defs::CPU_Request_in>(mbs)){
		
			//////////////////////////////////////////////
			printf("Input Address: %s\n",x.address.c_str());
			
			state.num_requests++;

			unsigned int dec = htoi(x.address.c_str());
			char *bstring, *Tindex, *Toffset, *Ttag;

			bstring = getBinary(dec);
			bstring = formatBinary(bstring);
		
			Ttag = (char *) malloc( sizeof(char) * (TAG + 1) );
	    		assert(Ttag!= NULL);
	    		Ttag[TAG] = '\0';

			Tindex = (char *) malloc( sizeof(char) * (INDEX + 1) );
	    		assert(Tindex != NULL);
	    		Tindex[INDEX] = '\0';
		
			Toffset = (char *) malloc( sizeof(char) * (OFFSET + 1) );
	    		assert(Toffset != NULL);
	    		Toffset[OFFSET] = '\0';

			int i = 0;
			for(i = 0; i < TAG; i++)
	    		{
				Ttag[i] = bstring[i];
	    		}
			for(i = TAG + 1; i < INDEX + TAG + 1; i++)
	   	 	{
				Tindex[i - TAG - 1] = bstring[i];
	    		}	
			for(i = INDEX + TAG + 2; i < OFFSET + INDEX + TAG + 2; i++)
	    		{
				Toffset[i - INDEX - TAG - 2] = bstring[i];
	    		}

		
			Parsed_Request_t new_request;
			new_request.R_W = x.R_W;
			new_request.tag = Ttag;
			new_request.index = btoi(Tindex);
			new_request.offset = btoi(Toffset);
			new_request.mem = x.mem;
			state.request.push_back(new_request);
				
			//state.next_internal = TIME("00:00:00:000");
			printf("TAG: %s\n Index: %d\n Offset: %d\n",new_request.tag.c_str(), new_request.index,new_request.offset);
		}

		for(const auto &x : get_messages<typename RequestManager_defs::Serviced_request_in>(mbs)){\
			state.send = 1;
			//printf("READY TO SEND");
		}

		state.next_internal = TIME("00:00:00:000");
	}

	//confluence transiton --> don't touch 
    //confluence transiton --> don't touch 
    	void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
		internal_transition();
        external_transition(TIME(), move(mbs));
}
	// internal transition 
	void internal_transition(){
	//internal transition implementation
		if(state.send == 1  && state.request.size() > 0){
			state.send++;
			state.next_internal = TIME("00:00:00:000");
		}else if(state.send == 2){
			state.request.pop_front();
			state.send = 0;
			state.next_internal = std::numeric_limits<TIME>::infinity();
		}else{
			state.next_internal = std::numeric_limits<TIME>::infinity();
		}
}

	// output function 
	typename make_message_bags<output_ports>::type output() const {
		// output function implementation 
		typename make_message_bags<output_ports>::type bags;
		if(state.send == 2){
			Parsed_Request_t temp = state.request.front();
			get_messages<typename RequestManager_defs::Parsed_Request_out>(bags).push_back(temp);
			//printf("SENDING TO MUX\n");
			return bags; 
		}
}

	// time advance function 
	TIME time_advance() const {
		return state.next_internal;		
}

	// output stream --> put infor you want to analyze here 
	friend ostringstream& operator<<(ostringstream& os, const typename RequestManager<TIME>::state_type& i) {
	//change model name above
	os << "Number of requests received: " << i.num_requests << "\n";
	//os << "\nTAG: " << i.tag << "\n" << "INDEX: " << i.index << "\n" << "OFFSET: " << i.offset << "\n";
	return os; 
}

};
#endif // _RequestManager_HPP_ 













