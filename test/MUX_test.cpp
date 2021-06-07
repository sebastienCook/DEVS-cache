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
#include "../atomics/Mux.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

// define input ports for coupled models 
struct InputPorts: public in_port<CPU_Request_t>{};
struct SelectInput: public in_port<int>{};
// define outputs ports for coupled models 
struct OutputPort: public out_port<Parsed_Request_t>{};

//input stream if requires input file
template<typename T>
class InputReader_CPU_Request_t : public iestream_input<CPU_Request_t,T> {
	public: 
		InputReader_CPU_Request_t () = default; 
		InputReader_CPU_Request_t (const char* file_path) : iestream_input<CPU_Request_t,T>(file_path) {}
};
template<typename T>
class InputReader_Select_t : public iestream_input<int,T> {
	public: 
		InputReader_Select_t () = default; 
		InputReader_Select_t (const char* file_path) : iestream_input<int,T>(file_path) {}
};

int main(){

	// input reader instantiation 
	const char * i_input_data_CPU_Request = "../input_data/CPU_Requests.txt";
	shared_ptr<dynamic::modeling::model> input_reader_CPU_Request = dynamic::translate::make_dynamic_atomic_model<InputReader_CPU_Request_t, TIME, const char*>("input_reader_CPU_Request", move(i_input_data_CPU_Request));

	const char * i_input_data_Select = "../input_data/ConfigSelect.txt";
	shared_ptr<dynamic::modeling::model> input_reader_Select = dynamic::translate::make_dynamic_atomic_model<InputReader_Select_t, TIME, const char*>("input_reader_Select", move(i_input_data_Select));

	// Atomic Model instantiation 
	shared_ptr<dynamic::modeling::model> RequestParser = dynamic::translate::make_dynamic_atomic_model<RequestManager,TIME>("RequestParser");

	shared_ptr<dynamic::modeling::model> MUX = dynamic::translate::make_dynamic_atomic_model<Mux,TIME>("MUX");

	// TOP MODEL
	dynamic::modeling::Ports iports_TOP = {}; 
	dynamic::modeling::Ports oports_TOP = {typeid(OutputPort)};
	
	dynamic::modeling::Models submodels_TOP = {input_reader_CPU_Request, input_reader_Select, MUX, RequestParser};

// EIC links --> External Input Connections --> input to input 
dynamic::modeling::EICs eics_TOP = {};

// EOC links --> External Output Connections --> output to output 
dynamic::modeling::EOCs eocs_TOP = { 

dynamic::translate::make_EOC<Mux_defs:: Config0_out,OutputPort>("MUX"),

dynamic::translate::make_EOC<Mux_defs:: Config1_out,OutputPort>("MUX")
};

// IC links --> Internal Connections --> output to input 
dynamic::modeling::ICs ics_TOP = {

dynamic::translate::make_IC<iestream_input_defs<CPU_Request_t>::out,RequestManager_defs::CPU_Request_in>("input_reader_CPU_Request","RequestParser"),

dynamic::translate::make_IC<iestream_input_defs<int>::out,Mux_defs::Select_in>("input_reader_Select","MUX"),

dynamic::translate::make_IC<RequestManager_defs::Parsed_Request_out,Mux_defs::Request_in>("RequestParser","MUX")
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
	// Change simulation enlapsed time
    dynamic::engine::runner<NDTime, logger_top> r(TOP,{0});
    r.run_until(NDTime("00:00:10:000"));
    return 0;
}



















