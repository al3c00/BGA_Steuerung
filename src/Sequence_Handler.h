#ifndef SEQUENCE_HANDLER_H
#define SEQUENCE_HANDLER_H

#include <map>
#include <unordered_map>
#include <thread>
#include <functional>
#include <chrono>

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

	void loadStoredSequences(std::string path);

	void playSequence();



private:

	Log* m_p_logger;
	std::shared_ptr<HW_Con> m_p_hw_con;

	std::mutex m_HW_Con_mutex;

	void m_sequence_wait(int wait_seconds);
	bool m_getDigitalInputState(std::string name);
	int m_getDoubleInputState(std::string name1, std::string name2);
	void m_switchDigitalOutputState(std::string name);
	void m_setDigitalOutputState(bool state, std::string name);

	std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>>m_functions;

	std::vector<std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>>>m_complete_sequences;

	void test();
	//Map with function pointers. The functions are class methods from HW_Con. With the mapping to a string name, they are easier to connect with the function names in the .txt-file
	std::map<std::string, bool(*)(std::string)>m_bool_string_functions;//Map for functions that return bool value and take one string argument
	std::map<std::string, void(*)(std::string)>m_void_string_functions;//Map for functions that return nothing and take one string argument
	std::map<std::string, void(*)(bool, std::string)>m_void_bool_string_functions;//Map for functions that return nothing and take one bool and one string argument
	std::map<std::string, int(*)(std::string, std::string)>m_int_string_string_functions;//Map for functions that return int and take two string arguments


	std::vector<void(*)>functions;

	std::string m_log_origin;
	std::string m_getProjectDirPath();

};

#endif // !SEQUENCE_HANDLER_H



