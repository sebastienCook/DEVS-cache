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
#include "../data_structures/CPU_Request.hpp"

//Atomic model headers 
#include <cadmium/basic_model/pdevs/iestream.hpp>

//Atomic and Coupled models needed for the main or test to be ran
#include "../coupled/Cache.cpp"
#include "../coupled/config_0.cpp"
#include "../coupled/config_1.cpp"
#include "../atomics/Memory_request_manager.hpp"
#include "../atomics/Mux.hpp"
#include "../atomics/RequestManager.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

// define input ports for coupled models 
struct OutputServicedPort: public in_port<string>{};


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
	const char * i_input_data_CPU_Request = "../input_data/AGP_trace.txt";
	shared_ptr<dynamic::modeling::model> input_reader_CPU_Request = dynamic::translate::make_dynamic_atomic_model<InputReader_CPU_Request_t, TIME, const char*>("input_reader_CPU_Request", move(i_input_data_CPU_Request)); 

	// Atomic Model instantiation 
	shared_ptr<dynamic::modeling::model> MEM = dynamic::translate::make_dynamic_atomic_model<Memory_request_manager,TIME>("MEM");

	shared_ptr<dynamic::modeling::model> Request_Manager = dynamic::translate::make_dynamic_atomic_model<RequestManager,TIME>("Request_Manager");

	shared_ptr<dynamic::modeling::model> MUX = dynamic::translate::make_dynamic_atomic_model<Mux,TIME>("MUX");


	// Coupled model instantiation 
	shared_ptr<dynamic::modeling::coupled<TIME>> CACHE = make_shared<dynamic::modeling::coupled<TIME>>("CACHE", submodels_cache, iports_cache_in, oports_cache_out, eics_cache, eocs_cache, ics_cache);

	shared_ptr<dynamic::modeling::coupled<TIME>> Config_0 = make_shared<dynamic::modeling::coupled<TIME>>("Config_0", submodels_config_0, iports_config_0_in, oports_config_0_out, eics_config_0, eocs_config_0, ics_config_0);
		
	shared_ptr<dynamic::modeling::coupled<TIME>> Config_1 = make_shared<dynamic::modeling::coupled<TIME>>("Config_1", submodels_config_1, iports_config_1_in, oports_config_1_out, eics_config_1, eocs_config_1, ics_config_1);

	// TOP MODEL
	dynamic::modeling::Ports iports_TOP = {}; 
	dynamic::modeling::Ports oports_TOP = {typeid(OutputServicedPort)};
	
	dynamic::modeling::Models submodels_TOP = {input_reader_CPU_Request, CACHE, Config_0, Config_1, MEM, MUX, Request_Manager};

// EIC links --> External Input Connections --> input to input 
dynamic::modeling::EICs eics_TOP = {
};

// EOC links --> External Output Connections --> output to output 
dynamic::modeling::EOCs eocs_TOP = { 
	dynamic::translate::make_EOC<Config_0_defs::Serviced_request_out,OutputServicedPort>("Config_0"),

	dynamic::translate::make_EOC<Config_1_defs::Serviced_request_out,OutputServicedPort>("Config_1")
};

// IC links --> Internal Connections --> output to input 
dynamic::modeling::ICs ics_TOP = {

dynamic::translate::make_IC<iestream_input_defs<CPU_Request_t>::out,RequestManager_defs::CPU_Request_in>("input_reader_CPU_Request","Request_Manager"),

dynamic::translate::make_IC<RequestManager_defs::Parsed_Request_out,Mux_defs::Request_in>("Request_Manager","MUX"),

dynamic::translate::make_IC<Mux_defs::Config1_out,Config_1_defs::Parsed_request_in>("MUX","Config_1"),

dynamic::translate::make_IC<Cache_defs::Hit_or_miss_out,Config_1_defs::hit_miss_in>("CACHE","Config_1"),

dynamic::translate::make_IC<Memory_request_manager_defs::Read_out,Config_1_defs::mem_read_in>("MEM","Config_1"),

dynamic::translate::make_IC<Cache_defs::Block_out,Config_1_defs::cache_read_in>("CACHE","Config_1"),

dynamic::translate::make_IC<Config_1_defs::Cache_write_out,Cache_defs::Write_request_in>("Config_1","CACHE"),

dynamic::translate::make_IC<Config_1_defs::Mem_write_out,Memory_request_manager_defs::Write_request_in>("Config_1","MEM"),

dynamic::translate::make_IC<Config_1_defs::Cache_read_request_out,Cache_defs::Read_request_in>("Config_1","CACHE"),

dynamic::translate::make_IC<Config_1_defs::Mem_read_request_out,Memory_request_manager_defs::Read_request_in>("Config_1","MEM"),

dynamic::translate::make_IC<Config_1_defs::tag_check_out,Cache_defs::Tag_check_in>("Config_1","CACHE"),

dynamic::translate::make_IC<Config_1_defs::Serviced_request_out,RequestManager_defs::Serviced_request_in>("Config_1","Request_Manager")

};

// Don't change below until runner call
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
	
    TOP = make_shared<dynamic::modeling::coupled<TIME>>("TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP);

 /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/Top_Test.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/Top_Test.txt");
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
    r.run_until(NDTime("01:00:00:000"));
    return 0;
}



















