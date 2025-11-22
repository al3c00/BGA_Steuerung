#include "Log.h"



Log::Log(std::string path_from_main_dir)
{
	std::filesystem::path project_dir = std::filesystem::current_path().parent_path().parent_path().parent_path();


	path_to_file = project_dir.string() + path_from_main_dir;
	std::cout << path_to_file << std::endl;
}



void Log::writeLog(LogLevel loglevel, std::string origin, std::string msg)
{

	std::string log_level_output;

	switch (loglevel)
	{
	case LogLevel::INFO:
	{
		log_level_output = "INFO";
	}break;

	case LogLevel::WARNING:
	{
		log_level_output = "WARNING";
	}break;
	
	case LogLevel::ERROR:
	{
		log_level_output = "ERROR";
	}break;
	default:
		break;
	}


	auto time = std::chrono::system_clock::now();
	std::time_t current_time = std::chrono::system_clock::to_time_t(time);
	std::string str_current_time = std::ctime(&current_time);

	//std::ctime inserts a newline at the end of the time data. Remove it with pop_back
	str_current_time.pop_back();

	std::ofstream log_file;
	log_file.open(path_to_file, std::ofstream::out | std::ofstream::app);

	if (!log_file.is_open())
	{
		std::cout << "Error opening log file!" << std::endl;
	}

	log_file << "[" +  str_current_time + "]" << " " << "[" + log_level_output +"]" + " " << origin + ": " << msg << "\n";


	log_file.close();

}

