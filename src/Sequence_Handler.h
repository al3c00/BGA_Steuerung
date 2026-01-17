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

	//Handles for the .txt file where the functions are stored
	void createSequence();

	void saveSequence();

	void loadSeq(std::string path);

	void startSequence(std::string name);

	//@brief Gets the step of the execution the asked sequence is in right now
	//@param name The name of the sequence. The same as in the Sequences.txt file, without the #
	int getExecutionStep(std::string name);

	//@brief Gets the individual functions of a sequence stored in a vector
	//@param name name of the sequence as written in the file without #
	//@param number Gets the sequence stored in NUMBER pos in map..use to switch through all the sequences without using their names
	std::vector<std::string> getSequenceFunctions(std::string name);
	std::vector<std::string> getSequenceFunctions(int number);


	//@brief Use together with getSequenceFunctions(int number)
	std::string getSequenceName(int number);
	
	//@brief Gets the ammount of loaded sequences
	int getAmmountOfLoadedSequences();


private:
	


	Log* m_p_logger;
	std::shared_ptr<HW_Con> m_p_hw_con;

	std::mutex m_HW_Con_mutex;

	enum SEQ_FUNCTION_TYPE {NOT_DEFINED, WAIT, JUMP_TO, PROGRESS_IF_1, PROGRESS_IF_2, GET_DIGITAL_INPUT, GET_DOUBLE_DIGITAL_INPUT, GET_ANALOG_INPUT, SET_DIGITAL_OUTPUT, SWITCH_DIGITAL_OUTPUT};

	//Struct to describe one step/function in the sequences. Holds the function type as enum SEQ_FUNCTION_TYPE and 3 int parameters
	struct Seq_Part_Info
	{
		std::string seq_function_name;
		SEQ_FUNCTION_TYPE type;
		int param_int1, param_int2;
		std::string param_string1, param_string2;
	};

	//A vector that holds all the steps/functions from a complete sequence
	//All vectors are copied into a map, where they can be addressed by a string-key
	std::vector<Seq_Part_Info>m_complete_sequence;

	std::vector<std::string> m_loaded_sequences_names;//Used mainly to iterate throug the map "m_complete_sequence_map" while getting the functions as a list in getSequenceFunctions
	std::map<std::string, std::vector<Seq_Part_Info>>m_complete_sequence_map;//Map that holds ALL the loades sequences. The sequences can be addressed by their string key (name)

	//To not having to reconstruct the vector every frame, just do it when the functions of a new sequence are asked
	std::vector<std::string> m_sequence_return_vector;
	std::string m_sequence_return_name;

	int m_sequence_return_number;
	
	//Struct with informations about the running sequences
	struct RunningSeqInfo
	{
		int current_step;//Current step of the sequence
	};

	std::map<std::string, RunningSeqInfo>m_running_sequences_info;//Map that holds information about all the running threads. The key is the name of the sequence



	void m_playSequence(std::string name, std::map<std::string, RunningSeqInfo>& running_sequences);

	std::string m_log_origin;
	std::string m_getProjectDirPath();


};

#endif // !SEQUENCE_HANDLER_H



