#ifndef SEQUENCE_HANDLER_H
#define SEQUENCE_HANDLER_H

#include <map>
#include "Log.h"

//@brief Class to create, save, load and use sequences
//The sequences are saved in to a .txt-file. The file containes the function names and the according parameters
//The functions are created as member functions of this class and stored with theyr function pointer in to a map with the name (std::string) as the key
class Sequence_Handler
{

public:

	void createFunction();

	void saveFunction();

	void loadFunctions();

	void playFunctions();



private:

	//declaration of function pointer
	void(*function_pntr)(int);


	//Map with function pointers. All function need to have the same format (in this case no return value and one int parameter)
	std::map<std::string, void(*)(int)>m_functions_map;

};

#endif // !SEQUENCE_HANDLER_H



