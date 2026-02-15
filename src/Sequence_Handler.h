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
	int getExecutionStep(std::string name);

	//@brief Gets the individual functions of a sequence stored in a vector
	//@param name name of the sequence as written in the file without #
	//@param number Gets the sequence stored in NUMBER pos in map..use to switch through all the sequences without using their names
	std::vector<std::string> getSequenceFunctions(int number);

	//@brief Gets the ammount of total function steps --> length of the sequence
	//@param number Gets the sequence stored in NUMBER pos in map..use to switch through all the sequences without using their names
	int getSequenceStepAmmount(int number);

	//@brief Use together with getSequenceFunctions(int number)
	std::string getSequenceName(int number);
	
	//@brief Gets the ammount of loaded sequences
	int getAmmountOfLoadedSequences();


private:
	


	Log* m_p_logger;
	std::shared_ptr<HW_Con> m_p_hw_con;

	std::vector<std::thread>m_threads_list;

	std::mutex m_HW_Con_mutex;

	enum SEQ_FUNCTION_TYPE {NOT_DEFINED, WAIT_MS, WAIT_S, WAIT_MIN, WAIT_H, WAIT_UNTIL, JUMP_TO, PROGRESS_IF_1, PROGRESS_IF_2, GET_DIGITAL_INPUT, GET_DOUBLE_DIGITAL_INPUT, GET_ANALOG_INPUT, SET_DIGITAL_OUTPUT, SWITCH_DIGITAL_OUTPUT};

	//Struct to describe one step/function in the sequences. Holds the function type as enum SEQ_FUNCTION_TYPE and 3 int parameters
	struct Seq_Part_Info
	{
		std::string seq_function_name;
		SEQ_FUNCTION_TYPE type;
		int param_int1, param_int2;
		std::string param_string1, param_string2;
	};

	
	std::vector<std::vector<Seq_Part_Info>>m_sequences;//Vector, that holds the loades sequences. 
	std::map<int, std::string>m_sequence_names;//Map to assign the names of sequence to her position in the vector
	
	
	//Struct with informations about the running sequences
	struct RunningSeqInfo
	{
		int current_step;//Current step of the sequence
		bool is_extern_paused;//If the execution of the sequence should be paused by an other part of the programm, set this to true
		
	};

	std::map<std::string, RunningSeqInfo>m_running_sequences_info;//Map that holds information about all the running threads. The key is the name of the sequence

	std::vector<std::string>m_sequence_return_vector;
	int m_sequence_return_number;


	//@brief Start a specific sequence
	//@param name Name of the sequence
	//@param v_seq Reference to the vector of all loaded sequences
	//@param seq_nmbr Position of the sequence wished to start in the vector
	//@param info_map Reference to the map that hold the information about all running and stopped sequences
	void m_playSequenceN(std::string name, std::vector<std::vector<Seq_Part_Info>>& v_seq, int seq_nmbr , std::map<std::string, RunningSeqInfo>& info_map);

	std::string m_log_origin;
	std::string m_getProjectDirPath();

	std::string m_sequence_dir;//Name of the sequence directory. Used to reload sequences


};

#endif // !SEQUENCE_HANDLER_H



