#ifndef SEQUENCE_HANDLER_H
#define SEQUENCE_HANDLER_H

#include <map>
#include <unordered_map>
#include <thread>
#include <functional>
#include <chrono>
#include <vector>

#include "Log.h"
#include "HW_Con.h"

//@brief Class to create, save, load and use sequences
//The sequences are saved in to a .txt-file. Each sequence consists of multiple functions. Those functions are predefined in the enum SEQ_FUNCTION_TYPE
//Each sequence is stored as a vector. Every sequence-step is written in to the struct Seq_Part_Info
//For easier access, the sequence-vectors are itself stored in the map "m_complete_sequence_map" and can be accessed by their name (without the #) as the key
class Sequence_Handler 
{

public:

	Sequence_Handler(Log* log, std::shared_ptr<HW_Con> hw_con);


	//@brief Use this function with some GUI input after the sequence files (.txt) in the directory has been changed
	//@brief Automatically starts all sequences
	void reloadSequences();


	//@brief Function who checks how many sequences are in the folder and loads them into a vector
	//@param path Path to the sequence folder, not the individual sequence files
	void loadSequences(std::string path);

	//@brief Function to start all loaded sequences at once. Call at starting up the programm after loading the sequences.
	void startSequences();


	//@brief Gets the step of the execution the asked sequence is in right now
	//@param name The name of the sequence. The same as in the Sequences.txt file, without the #
	int getSequenceInfo_CurrentStep();

	//@brief Gets the individual functions of a sequence stored in a vector
	//@param name name of the sequence as written in the file without #
	//@param number Gets the sequence stored in NUMBER pos in map..use to switch through all the sequences without using their names
	std::vector<std::string> getSequenceInfo_Functions();

	//@brief Gets the ammount of total function steps --> length of the sequence
	//@param number Gets the sequence stored in NUMBER pos in map..use to switch through all the sequences without using their names
	int getSequenceInfo_TotalSteps();

	//@brief Use together with getSequenceFunctions(int number)
	std::string getSequenceInfo_Name();
	
	//@brief Gets the ammount of loaded sequences
	int getNmbrOfRunningSequences();

	void switchThroughSequences();
	int getAskedSequence();


private:
	
	int m_running_threads;//Use this number to count the newly loaded functions in "loadFunctions". Begin from zero by every function call
	int m_stopped_threads;//Use this number to count stopped threads to know, where the active threads in the vector "m_threads_list" begin
	int m_total_threads;//Increase this number every time a new thread is started. Use it to find the correct position in the vector "m_threads_list"
	
	int m_currently_asked_sequence;

	Log* m_p_logger;
	std::shared_ptr<HW_Con> m_p_hw_con;

	enum SEQ_FUNCTION_TYPE { NOT_DEFINED, WAIT_MS, WAIT_S, WAIT_MIN, WAIT_H, WAIT_UNTIL, JUMP_TO, PROGRESS_IF_1, PROGRESS_IF_2, GET_DIGITAL_INPUT, GET_DOUBLE_DIGITAL_INPUT, GET_ANALOG_INPUT, SET_DIGITAL_OUTPUT, SWITCH_DIGITAL_OUTPUT };

	//Struct to describe one step/function in the sequences. Holds the function type as enum SEQ_FUNCTION_TYPE and 3 int parameters
	struct Seq_Part_Info
	{
		std::string seq_function_name;
		SEQ_FUNCTION_TYPE type;
		int param_int1, param_int2;
		std::string param_string1, param_string2;
	};

	//Struct to hold infos about all the threads. Infos about stopped threads (which have finished the sequence) remain in the vector that contains the struct to organize the forcfull killing of them
	struct ThreadInfo
	{
		int current_execution_step;
		bool is_active;
		std::string sequence_name;
		std::vector<Seq_Part_Info> sequence_functions;
	};


	std::vector<ThreadInfo>m_threads_list;//Vector, that holds information about the running threads

	std::mutex m_HW_Con_mutex;

	



	
	
	std::vector<std::string>m_sequence_return_vector;
	int m_sequence_return_number;


	//@brief Start a specific sequence
	//@param r_thread_info Reference to the vector that holds information about all loaded threads, even if they are stopped
	//@param thread_number Use a counter to assign a number to each loaded thread. The number is used to find the correct position in the vector to get inforamtion
	void m_playSequence(std::vector<ThreadInfo>& r_thread_info, int thread_number);

	std::string m_log_origin;
	std::string m_getProjectDirPath();

	std::string m_sequence_dir;//Name of the sequence directory. Used to reload sequences


};

#endif // !SEQUENCE_HANDLER_H



