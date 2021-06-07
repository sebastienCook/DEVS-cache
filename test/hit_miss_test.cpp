//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>

//Data structures needed 
#include "../data_structures/Parsed_Request.hpp"
#include "../data_structures/Tag_change.hpp"


#include "../SimParameters/Parameters.h"
#include "../SimParameters/helperFunc.c"

//Atomic model headers 
#include <cadmium/basic_model/pdevs/iestream.hpp>

//Atomic and Coupled models needed for the main or test to be ran
#include "../atomics/config_1/hit_or_miss.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;


// define outputs ports for coupled models 
struct OutputHitReadPort: public out_port<Parsed_Request_t>{};
struct OutputHitWritePort: public out_port<Parsed_Request_t>{};
struct OutputCheckPort: public out_port<Tag_change_t>{};
struct OutputMissReadPort: public out_port<Parsed_Request_t>{};
struct OutputMissWritePort: public out_port<Parsed_Request_t>{};



//input stream if requires input file
template<typename T>
class InputReader_Parsed_Request_t : public iestream_input<Parsed_Request_t,T> {
	public: 
		InputReader_Parsed_Request_t () = default; 
		InputReader_Parsed_Request_t (const char* file_path) : iestream_input<Parsed_Request_t,T>(file_path) {}
};

//input stream if requires input file
template<typename T>
class InputReader_Cache_Response_t : public iestream_input<int,T> {
	public: 
		InputReader_Cache_Response_t () = default; 
		InputReader_Cache_Response_t (const char* file_path) : iestream_input<int,T>(file_path) {}
};


int main(){

 	//printf("ENTER MAIN\n");
	// input reader instantiation 
	const char * i_input_data_Parsed_Request = "../input_data/Parsed_Request.txt";
	shared_ptr<dynamic::modeling::model> input_reader_Parsed_Request = dynamic::translate::make_dynamic_atomic_model<InputReader_Parsed_Request_t, TIME, const char*>("input_reader_Parsed_Request", move(i_input_data_Parsed_Request)); 


	//printf("ENTER MAIN\n");
	// input reader instantiation 
	const char * i_input_data_Cache_Response = "../input_data/Cache_Response.txt";
	shared_ptr<dynamic::modeling::model> input_reader_Cache_Response = dynamic::translate::make_dynamic_atomic_model<InputReader_Cache_Response_t, TIME, const char*>("input_reader_Cache_Response", move(i_input_data_Cache_Response)); 

	
	// Atomic Model instantiation 
	shared_ptr<dynamic::modeling::model> Hit_Miss = dynamic::translate::make_dynamic_atomic_model<hit_or_miss,TIME>("Hit_Miss");

	// TOP MODEL
	dynamic::modeling::Ports iports_TOP = {}; 
	dynamic::modeling::Ports oports_TOP = {typeid(OutputHitReadPort), typeid(OutputHitWritePort), typeid(OutputCheckPort), typeid(OutputMissReadPort), typeid(OutputMissWritePort)};
	
	dynamic::modeling::Models submodels_TOP = {input_reader_Parsed_Request, input_reader_Cache_Response, Hit_Miss};

	// EIC links --> External Input Connections --> input to input 
	dynamic::modeling::EICs eics_TOP = {};

	// EOC links --> External Output Connections --> output to output 
	dynamic::modeling::EOCs eocs_TOP = { 

dynamic::translate::make_EOC<hit_or_miss_defs::hit_read_out,OutputHitReadPort>("Hit_Miss"),

dynamic::translate::make_EOC<hit_or_miss_defs::hit_write_out,OutputHitWritePort>("Hit_Miss"),

dynamic::translate::make_EOC<hit_or_miss_defs::miss_read_out,OutputMissReadPort>("Hit_Miss"),

dynamic::translate::make_EOC<hit_or_miss_defs::miss_write_out,OutputMissWritePort>("Hit_Miss"),

dynamic::translate::make_EOC<hit_or_miss_defs::tag_check_out,OutputCheckPort>("Hit_Miss")
};

// IC links --> Internal Connections --> output to input 
dynamic::modeling::ICs ics_TOP = {

dynamic::translate::make_IC<iestream_input_defs<Parsed_Request_t>::out,hit_or_miss_defs::Request_in>("input_reader_Parsed_Request","Hit_Miss"),

dynamic::translate::make_IC<iestream_input_defs<int>::out,hit_or_miss_defs::hit_miss_in>("input_reader_Cache_Response","Hit_Miss")
};

// Don't change below until runner call
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
	
    TOP = make_shared<dynamic::modeling::coupled<TIME>>("TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP);

 /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/hit_miss_out.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/hit_miss_out.txt");
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





















