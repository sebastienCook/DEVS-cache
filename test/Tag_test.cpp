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
#include "../data_structures/Tag_change.hpp"


#include "../SimParameters/Parameters.h"
#include "../SimParameters/helperFunc.c"

//Atomic model headers 
#include <cadmium/basic_model/pdevs/iestream.hpp>

//Atomic and Coupled models needed for the main or test to be ran
#include "../atomics/Tag.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

struct OutputPort: public out_port<int>{};

template<typename T>
class InputReader_tag_check_t : public iestream_input<Tag_change_t,T> {
	public: 
		InputReader_tag_check_t () = default; 
		InputReader_tag_check_t (const char* file_path) : iestream_input<Tag_change_t,T>(file_path) {}
};

template<typename T>
class InputReader_tag_change_t : public iestream_input<Tag_change_t,T> {
	public: 
		InputReader_tag_change_t () = default; 
		InputReader_tag_change_t (const char* file_path) : iestream_input<Tag_change_t,T>(file_path) {}
};


int main(){

 	//printf("ENTER MAIN\n");
	// input reader instantiation 
	const char * i_input_data_tag_check = "../input_data/tag_checks.txt";
	shared_ptr<dynamic::modeling::model> input_reader_tag_check = dynamic::translate::make_dynamic_atomic_model<InputReader_tag_check_t, TIME, const char*>("input_reader_tag_check", move(i_input_data_tag_check)); 

	const char * i_input_data_tag_change = "../input_data/tag_changes.txt";
	shared_ptr<dynamic::modeling::model> input_reader_tag_change = dynamic::translate::make_dynamic_atomic_model<InputReader_tag_change_t, TIME, const char*>("input_reader_tag_change", move(i_input_data_tag_change)); 


	// Atomic Model instantiation 
	shared_ptr<dynamic::modeling::model> Tag_list = dynamic::translate::make_dynamic_atomic_model<Tag,TIME>("Tag_list");

	// TOP MODEL
	dynamic::modeling::Ports iports_TOP = {}; 
	dynamic::modeling::Ports oports_TOP = {typeid(OutputPort)};
	
	dynamic::modeling::Models submodels_TOP = {input_reader_tag_check, input_reader_tag_change, Tag_list};



	// EIC links --> External Input Connections --> input to input 
	dynamic::modeling::EICs eics_TOP = {};

	// EOC links --> External Output Connections --> output to output 
	dynamic::modeling::EOCs eocs_TOP = { 

dynamic::translate::make_EOC<Tag_defs::hit_or_miss_out,OutputPort>("Tag_list")
};

// IC links --> Internal Connections --> output to input 
dynamic::modeling::ICs ics_TOP = {

dynamic::translate::make_IC<iestream_input_defs<Tag_change_t>::out,Tag_defs::tag_check_in>("input_reader_tag_check","Tag_list"),

dynamic::translate::make_IC<iestream_input_defs<Tag_change_t>::out,Tag_defs::tag_change_in>("input_reader_tag_change","Tag_list")
};

// Don't change below until runner call
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
	
    TOP = make_shared<dynamic::modeling::coupled<TIME>>("TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP);

 /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/tag_out.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/tag_out.txt");
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
















