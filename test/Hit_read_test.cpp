//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>

//Atomic model headers 
#include <cadmium/basic_model/pdevs/iestream.hpp>


#include "../SimParameters/Parameters.h"
#include "../SimParameters/helperFunc.c"

//Atomic and Coupled models needed for the main or test to be ran
#include "../atomics/config_1/Hit_read.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

struct OutputServicedRequestPort: public out_port<string>{};
struct OutputReadRequestPort: public out_port<int>{};


//input stream if requires input file
template<typename T>
class InputReader_Parsed_Request_t : public iestream_input<Parsed_Request_t,T> {
	public: 
		InputReader_Parsed_Request_t () = default; 
		InputReader_Parsed_Request_t (const char* file_path) : iestream_input<Parsed_Request_t,T>(file_path) {}
};

//input stream if requires input file
template<typename T>
class InputReader_Cache_Response_t : public iestream_input<string,T> {
	public: 
		InputReader_Cache_Response_t () = default; 
		InputReader_Cache_Response_t (const char* file_path) : iestream_input<string,T>(file_path) {}
};


int main(){

 	//printf("ENTER MAIN\n");
	// input reader instantiation 
	const char * i_input_data_Parsed_Request = "../input_data/Parsed_Request.txt";
	shared_ptr<dynamic::modeling::model> input_reader_Parsed_Request = dynamic::translate::make_dynamic_atomic_model<InputReader_Parsed_Request_t, TIME, const char*>("input_reader_Parsed_Request", move(i_input_data_Parsed_Request)); 


	//printf("ENTER MAIN\n");
	// input reader instantiation 
	const char * i_input_data_Cache_Response = "../input_data/Cache_read_response.txt";
	shared_ptr<dynamic::modeling::model> input_reader_Cache_Response = dynamic::translate::make_dynamic_atomic_model<InputReader_Cache_Response_t, TIME, const char*>("input_reader_Cache_Response", move(i_input_data_Cache_Response)); 

	
	// Atomic Model instantiation 
	shared_ptr<dynamic::modeling::model> Hit_Read = dynamic::translate::make_dynamic_atomic_model<Hit_read,TIME>("Hit_Read");

	// TOP MODEL
	dynamic::modeling::Ports iports_TOP = {}; 
	dynamic::modeling::Ports oports_TOP = {typeid(OutputServicedRequestPort), typeid(OutputReadRequestPort)};

dynamic::modeling::Models submodels_TOP = {input_reader_Parsed_Request, input_reader_Cache_Response, Hit_Read};

	// EIC links --> External Input Connections --> input to input 
	dynamic::modeling::EICs eics_TOP = {};

	// EOC links --> External Output Connections --> output to output 
	dynamic::modeling::EOCs eocs_TOP = { 

dynamic::translate::make_EOC<Hit_read_defs::Read_request_out,OutputReadRequestPort>("Hit_Read"),

dynamic::translate::make_EOC<Hit_read_defs::Serviced_request_out,OutputServicedRequestPort>("Hit_Read")
};

// IC links --> Internal Connections --> output to input 
dynamic::modeling::ICs ics_TOP = {

dynamic::translate::make_IC<iestream_input_defs<Parsed_Request_t>::out,Hit_read_defs::Request_in>("input_reader_Parsed_Request","Hit_Read"),

dynamic::translate::make_IC<iestream_input_defs<string>::out,Hit_read_defs::Cache_read_in>("input_reader_Cache_Response","Hit_Read")
};

// Don't change below until runner call
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
	
    TOP = make_shared<dynamic::modeling::coupled<TIME>>("TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP);

 /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/Hit_Read_out.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/Hit_Read_out.txt");
    struct oss_sink_state{
        static ostream& sink(){          
            return out_state;
        }
    };


     using state=logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>, oss_sink_state>;
    using log_messages=logger::logger<logger::logger_messages, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_mes=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_sta=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_state>;

    using logger_top=logger::multilogger<state, log_messages, global_time_mes, global_time_sta>;

    /************** Runner call ************************/ 
    dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    r.run_until(NDTime("00:01:00:000"));
    return 0;
}





















