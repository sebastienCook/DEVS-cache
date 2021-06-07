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
#include "../data_structures/CPU_Request.hpp"
#include "../data_structures/Parsed_Request.hpp"


#include "../SimParameters/Parameters.h"
#include "../SimParameters/helperFunc.c"

//Atomic model headers 
#include <cadmium/basic_model/pdevs/iestream.hpp>

//Atomic and Coupled models needed for the main or test to be ran
#include "../atomics/RequestManager.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;


// define outputs ports for coupled models 
struct OutputPort: public out_port<Parsed_Request_t>{};

//input stream if requires input file
template<typename T>
class InputReader_CPU_Request_t : public iestream_input<CPU_Request_t,T> {
	public: 
		InputReader_CPU_Request_t () = default; 
		InputReader_CPU_Request_t (const char* file_path) : iestream_input<CPU_Request_t,T>(file_path) {}
};

int main(){

 	//printf("ENTER MAIN\n");
	// input reader instantiation 
	const char * i_input_data_CPU_Request = "../input_data/CPU_Requests.txt";
	shared_ptr<dynamic::modeling::model> input_reader_CPU_Request = dynamic::translate::make_dynamic_atomic_model<InputReader_CPU_Request_t, TIME, const char*>("input_reader_CPU_Request", move(i_input_data_CPU_Request)); 
	
	//printf("input reader made\n");
	// Atomic Model instantiation 
	shared_ptr<dynamic::modeling::model> RequestManager_1 = dynamic::translate::make_dynamic_atomic_model<RequestManager,TIME>("RequestManager_1");

	// TOP MODEL
	dynamic::modeling::Ports iports_TOP = {}; 
	dynamic::modeling::Ports oports_TOP = {typeid(OutputPort)};
	
	dynamic::modeling::Models submodels_TOP = {input_reader_CPU_Request, RequestManager_1};

// EIC links --> External Input Connections --> input to input 
dynamic::modeling::EICs eics_TOP = {};

// EOC links --> External Output Connections --> output to output 
dynamic::modeling::EOCs eocs_TOP = { 

dynamic::translate::make_EOC<RequestManager_defs::Parsed_Request_out,OutputPort>("RequestManager_1")
};

// IC links --> Internal Connections --> output to input 
dynamic::modeling::ICs ics_TOP = {

dynamic::translate::make_IC<iestream_input_defs<CPU_Request_t>::out,RequestManager_defs::CPU_Request_in>("input_reader_CPU_Request","RequestManager_1")
};

// Don't change below until runner call
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
	
    TOP = make_shared<dynamic::modeling::coupled<TIME>>("TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP);

 /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/ParsedRequests.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/ParsedRequests.txt");
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



















