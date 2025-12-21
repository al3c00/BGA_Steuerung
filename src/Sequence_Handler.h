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
//The sequences are saved in to a .txt-file. The file containes the function names and the according parameters
//The functions are created as member functions of this class and stored with theyr function pointer in to a map with the name (std::string) as the key
class Sequence_Handler 
{

public:

	Sequence_Handler(Log* log, std::shared_ptr<HW_Con> hw_con);

	//Handles for the .txt file where the functions are stored
	void createSequence();

	void saveSequence();

	void loadSeq(std::string path);

	void startSequence(std::string name);

	



private:
	void m_playSequence(std::string name);


	Log* m_p_logger;
	std::shared_ptr<HW_Con> m_p_hw_con;

	std::mutex m_HW_Con_mutex;

	enum SEQ_FUNCTION_TYPE {NOT_DEFINED, WAIT, WAIT_UNTIL, PROGRESS_IF, GET_DIGITAL_INPUT, GET_DOUBLE_DIGITAL_INPUT, GET_ANALOG_INPUT, SET_DIGITAL_OUTPUT, SWITCH_DIGITAL_OUTPUT};

	//Struct to describe one step/function in the sequences. Holds the function type as enum SEQ_FUNCTION_TYPE and 3 int parameters
	struct Seq_Part_Info
	{
		SEQ_FUNCTION_TYPE type;
		int param_int1, param_int2;
		std::string param_string1, param_string2;
	};

	//A vector that holds all the steps/functions from a complete sequence
	//All vectors are copied into a map, where they can be addressed by a string-key
	std::vector<Seq_Part_Info>m_complete_sequence;

	std::map<std::string, std::vector<Seq_Part_Info>>m_complete_sequence_map;//Map that holds all the loades sequences. The sequences can be addressed by their string key




	std::string m_log_origin;
	std::string m_getProjectDirPath();


};

#endif // !SEQUENCE_HANDLER_H



