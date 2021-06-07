//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>


#include "../SimParameters/Parameters.h"
#include "../SimParameters/helperFunc.c"

//Data structures needed 
#include "../data_structures/Cache_write.hpp"
#include "../data_structures/Tag_change.hpp"

//Atomic model headers 
#include <cadmium/basic_model/pdevs/iestream.hpp>

//Atomic and Coupled models needed for the main or test to be ran
#include "../atomics/Block.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

struct OutputTagUpdatePort: public out_port<Tag_change_t>{};
struct OutputBlockPort: public out_port<string>{};

template<typename T>
class InputReader_read_request_t : public iestream_input<int,T> {
	public: 
		InputReader_read_request_t () = default; 
		InputReader_read_request_t (const char* file_path) : iestream_input<int,T>(file_path) {}
};

template<typename T>
class InputReader_write_request_t : public iestream_input<Cache_write_t,T> {
	public: 
		InputReader_write_request_t () = default; 
		InputReader_write_request_t (const char* file_path) : iestream_input<Cache_write_t,T>(file_path) {}
};


int main(){

 	//printf("ENTER MAIN\n");
	// input reader instantiation 
	const char * i_input_data_read_request = "../input_data/read_request.txt";
	shared_ptr<dynamic::modeling::model> input_reader_read_request = dynamic::translate::make_dynamic_atomic_model<InputReader_read_request_t, TIME, const char*>("input_reader_read_request", move(i_input_data_read_request)); 

	const char * i_input_data_write_request = "../input_data/write_request.txt";
	shared_ptr<dynamic::modeling::model> input_reader_write_request = dynamic::translate::make_dynamic_atomic_model<InputReader_write_request_t, TIME, const char*>("input_reader_write_request", move(i_input_data_write_request)); 


	// Atomic Model instantiation 
	shared_ptr<dynamic::modeling::model> Blocks = dynamic::translate::make_dynamic_atomic_model<Block,TIME>("Blocks");

	// TOP MODEL
	dynamic::modeling::Ports iports_TOP = {}; 
	dynamic::modeling::Ports oports_TOP = {typeid(OutputTagUpdatePort), typeid(OutputBlockPort)};
	
	dynamic::modeling::Models submodels_TOP = {input_reader_write_request, input_reader_read_request, Blocks};



	// EIC links --> External Input Connections --> input to input 
	dynamic::modeling::EICs eics_TOP = {};

	// EOC links --> External Output Connections --> output to output 
	dynamic::modeling::EOCs eocs_TOP = { 

dynamic::translate::make_EOC<Block_defs::tag_change_out,OutputTagUpdatePort>("Blocks"),

dynamic::translate::make_EOC<Block_defs::block_out,OutputBlockPort>("Blocks")
};

// IC links --> Internal Connections --> output to input 
dynamic::modeling::ICs ics_TOP = {

dynamic::translate::make_IC<iestream_input_defs<int>::out,Block_defs::read_request_in>("input_reader_read_request","Blocks"),

dynamic::translate::make_IC<iestream_input_defs<Cache_write_t>::out,Block_defs::write_request_in>("input_reader_write_request","Blocks")
};

// Don't change below until runner call
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
	
    TOP = make_shared<dynamic::modeling::coupled<TIME>>("TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP);

 /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/block_out.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/block_out.txt");
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
















