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
#include "../data_structures/Data_structure_name.hpp"

//Atomic model headers 
#include <cadmium/basic_model/pdevs/iestream.hpp>

//Atomic and Coupled models needed for the main or test to be ran
#include "../coupled/Coupled_model_name.cpp"
#include "../atomics/Atomic_model_name.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

// define input ports for coupled models 
struct InputPorts: public in_port<MESSAGE_TYPE>{};
// define outputs ports for coupled models 
struct OutputPort: public out_port<MESSAGE_TYPE>{};

//input stream if requires input file
template<typename T>
class InputReader_Data_structure_t : public iestream_input<Data_structure_t,T> {
	public: 
		InputReader_Data_structure_t () = default; 
		InputReader_Data_structure_t (const char* file_path) : iestream_input<Data_structure_t,T>(file_path) {}
};

int main(){

	// input reader instantiation 
	const char * i_input_data_name = "../input_data/file_name.txt";
	shared_ptr<dynamic::modeling::model> input_reader_name = dynamic::translate::make_dynamic_atomic_model<InputReader_Data_structure_t, TIME, const char*>("input_reader_name", move(i_input_data_name));

	// Atomic Model instantiation 
	shared_ptr<dynamic::modeling::model> Temp_Model_Name = dynamic::translate::make_dynamic_atomic_model<Atomic_Model_Name,TIME>("Temp_Model_Name");

	// if the model constructor requires an inpute --> change int to val input type
	shared_ptr<dynamic::modeling::model> Temp_Model_Name = dynamic::translate::make_dynamic_atomic_model<InTrainQueue,TIME,int>("Temp_Model_Name",1);

	// Coupled model instantiation 
	shared_ptr<dynamic::modeling::coupled<TIME>> Temp_Coupled_Model_Name = make_shared<dynamic::modeling::coupled<TIME>>("Temp_Coupled_Model_Name", Temp_Model_Name, ... all other atomic models);

	// TOP MODEL
	dynamic::modeling::Ports iports_TOP = {typeid(InputPorts)}; 
	dynamic::modeling::Ports oports_TOP = {typeid(OutputPorts), ... };
	
	dynamic::modeling::Models submodels_TOP = {input_reader_name, Temp_Coupled_Model_Name, Temp_Model_Name}, ...;

// EIC links --> External Input Connections --> input to input 
dynamic::modeling::EICs eics_Model_Name = {
	// Each individual connections 
	dynamic::translate::make_EIC<InputPorts,Atomic_Model_nam_defs::Atomic_Port_name_In>("Temp_name_of_atomic_model_used"),
// Can connect many to a single input port or if many input ports
dynamic::...
};

// EOC links --> External Output Connections --> output to output 
dynamic::modeling::EOCs eocs_Model_Name = { 

dynamic::translate::make_EOC<Atomic_Model_nam_defs::Atomic_Port_name_Out,OutputPort>("Temp_name_of_atomic_model_used"),
// Can connect many to a single input port or if many input ports
dynamic::...
};

// IC links --> Internal Connections --> output to input 
dynamic::modeling::ICs ics_Model_Name = {

dynamic::translate::make_IC<Atomic_Model_nam_defs::Atomic_Port_name_Out,Atomic_Model_nam_defs::Atomic_Port_name_In>("Temp_name_of_atomic_model_used","Temp_name_of_atomic_model_used"),

dynamic::...
};

// Don't change below until runner call
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
	
    TOP = make_shared<dynamic::modeling::coupled<TIME>>("TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP);

 /*************** Loggers *******************/
    static ofstream out_messages("../simulation_results/Output_File_Name.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../simulation_results/Output_File_Name.txt");
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



















