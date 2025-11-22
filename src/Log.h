#ifndef LOG_H
#define LOG_H


#include <iostream>
#include <mutex>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <chrono>
#include <iterator>
#include <locale>


enum struct LogLevel
{
	INFO = 0, WARNING = 1, ERROR = 2
};

//class Log.h
//@brief This class handles the log outputs to a specific file. 
//@brief Different Log Levels and types can be set
class Log
{
public:
	Log(std::string path_from_main_dir);


	//@brief Write Log to already specified log file
	//@param origin The Object which called the logging action. E.g.: "Filehandler"
	void writeLog(LogLevel, std::string origin, std::string msg);





private:


	
	std::string path_to_file;


};




#endif//LOG_H
