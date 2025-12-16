#include "Sequence_Handler.h"

Sequence_Handler::Sequence_Handler(Log* logger, std::shared_ptr<HW_Con> hw_con)
{
	m_p_logger = logger;
	m_p_hw_con = hw_con;

	m_log_origin = "Sequence_Handler";

	
	m_functions["GET_DI"] = [this](const  std::vector<std::string>& args)
	{
		bool result = m_getDigitalInputState(args.at(0));
		std::cout << "GET_DI result: " << result << std::endl;
	};

	m_functions["GET_DOUBLE_DI"] = [this](const std::vector<std::string>& args)
		{
			int result = m_getDoubleInputState(args.at(0), args.at(1));
			std::cout << "GET_DOUBLE_DI result: " << result << std::endl;
		};

	m_functions["SWITCH_DO"] = [this](const std::vector<std::string>& args)
		{
			m_switchDigitalOutputState(args.at(0));
		};

	m_functions["SET_DO"] = [this](const std::vector<std::string>& args)
		{
			bool state = (args.at(0) == "true");
			m_setDigitalOutputState(state, args.at(1));
		};

	m_functions["WAIT"] = [](const std::vector<std::string>& args)
	{
			int ms = std::stoi(args.at(0));
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	};
}

void Sequence_Handler::loadStoredSequences(std::string path)
{
		std::string temp;
		std::string single_character;
		std::string sequence_name;
		std::vector<std::string>function_arguments;
		std::vector<std::string>function_list;

		

		function_list.reserve(10);

		enum struct FUNCTION_TYPES { GET_DIGITAL_INPUT = 0, GET_DOUBLE_DIGITAL_INPUT = 1, SET_DIGITAL_OUTPUT = 2, SWITCH_DIGITAL_OUTPUT = 3, WAIT = 4 } function_types;

		std::ifstream file(m_getProjectDirPath() + path);
		file.seekg(0, file.end);
		int length = file.tellg();
		file.seekg(0, file.beg);

		char* buffer = new char[length];

		m_p_logger->writeLog(LogLevel::INFO, m_log_origin + " LOAD_STORED_SEQUENCES", "Reading: " + std::to_string(length) + " characters");

		file.read(buffer, length);

		file.close();

		//Unsafe, file could be corrupted!!
		for (int i = 0; i < length; i++)
		{
			single_character = buffer[i];

			//Check if a function name has been read. If yes clear string and prepare to read and store function arguments

			if (temp == "WAIT")
			{
				function_types = FUNCTION_TYPES::WAIT;
				function_list.push_back("WAIT");
				temp.clear();
			}

			if (temp == "GET_DI")
			{
				function_types = FUNCTION_TYPES::GET_DIGITAL_INPUT;
				function_list.push_back("GET_DI");
				temp.clear();
			}

			if (temp == "GET_DOUBLE_DI")
			{
				function_types = FUNCTION_TYPES::GET_DOUBLE_DIGITAL_INPUT;
				function_list.push_back("GET_DOUBLE_DI");
				temp.clear();
			}

			if (temp == "SWITCH_DO")
			{
				function_types = FUNCTION_TYPES::SWITCH_DIGITAL_OUTPUT;
				function_list.push_back("SWITCH_DO");
				temp.clear();
			}

			if (temp == "SET_DO")
			{
				function_types = FUNCTION_TYPES::SET_DIGITAL_OUTPUT;
				function_list.push_back("SET_DO");
				temp.clear();
			}


			if (single_character != "#" && single_character != "{" && single_character != "}" && single_character != "\n" && single_character != "\r")
			{
				temp.append(single_character);
			}

			//The line from one function ends with "-", if there are two arguments for one functions, they are seperated with ","
		/*	if (single_character == "," || single_character == "-")
			{
				function_arguments.push_back(temp);
				temp.clear();
			}*/

			if (single_character == "}")//The last number doesn't have a comma following, so it has to be read when the bracelet is found
			{
				/*for (int i = 0; i < function_list.size(); i++)
				{
					m_complete_sequences.push_back({function_list.at(i),function_arguments.at(i) });
				}*/
				temp.clear();
			}
		}



}

void Sequence_Handler::playSequence()
{
	std::thread(&Sequence_Handler::test, this).detach();
}



void Sequence_Handler::m_sequence_wait(int wait_seconds)
{
	delay(wait_seconds);
}

bool Sequence_Handler::m_getDigitalInputState(std::string name)
{
	return m_p_hw_con->getDigitalInputState(name);
}

int Sequence_Handler::m_getDoubleInputState(std::string name1, std::string name2)
{
	return m_p_hw_con->getDoubleInputState(name1, name2);
}

void Sequence_Handler::m_switchDigitalOutputState(std::string name)
{
	m_p_hw_con->switchDigitalOutputState(name);
}

void Sequence_Handler::m_setDigitalOutputState(bool state, std::string name)
{
	m_p_hw_con->setDigitalOutputState(state, name);
}



void Sequence_Handler::test()
{
	for (int i = 0; i < 100; i++)
	{
		m_p_hw_con->switchDigitalOutputState("D_Out_9");
		m_sequence_wait(1000);
	}


	std::cout << "Finished test sequence" << std::endl;

}



std::string Sequence_Handler::m_getProjectDirPath()
{
	std::filesystem::path project_dir = std::filesystem::current_path().parent_path().parent_path().parent_path();
	return project_dir.string();
}

