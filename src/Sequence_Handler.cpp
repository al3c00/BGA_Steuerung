#include "Sequence_Handler.h"

Sequence_Handler::Sequence_Handler(Log* logger, std::shared_ptr<HW_Con> hw_con)
{
	m_p_logger = logger;
	m_p_hw_con = hw_con;

	m_log_origin = "Sequence_Handler";

}

//void Sequence_Handler::loadStoredSequences(std::string path)
//{
//		std::string temp;
//		std::string single_character;
//		std::string sequence_name;
//		std::vector<std::string>function_arguments;
//		std::vector<std::string>function_list;
//
//		
//
//		function_list.reserve(10);
//
//		
//
//		std::ifstream file(m_getProjectDirPath() + path);
//		file.seekg(0, file.end);
//		int length = file.tellg();
//		file.seekg(0, file.beg);
//
//		char* buffer = new char[length];
//
//		m_p_logger->writeLog(LogLevel::INFO, m_log_origin + " LOAD_STORED_SEQUENCES", "Reading: " + std::to_string(length) + " characters");
//
//		file.read(buffer, length);
//
//		file.close();
//
//		//Unsafe, file could be corrupted!!
//		for (int i = 0; i < length; i++)
//		{
//			single_character = buffer[i];
//
//			//Check if a function name has been read. If yes clear string and prepare to read and store function arguments
//
//			if (temp == "WAIT")
//			{
//				
//				function_list.push_back("WAIT");
//				temp.clear();
//			}
//
//			if (temp == "GET_DI")
//			{
//				
//				function_list.push_back("GET_DI");
//				temp.clear();
//			}
//
//			if (temp == "GET_DOUBLE_DI")
//			{
//				
//				function_list.push_back("GET_DOUBLE_DI");
//				temp.clear();
//			}
//
//			if (temp == "SWITCH_DO")
//			{
//				
//				function_list.push_back("SWITCH_DO");
//				temp.clear();
//			}
//
//			if (temp == "SET_DO")
//			{
//				
//				function_list.push_back("SET_DO");
//				temp.clear();
//			}
//
//
//			if (single_character != "#" && single_character != "{" && single_character != "}" && single_character != "," && single_character != "-" && single_character != "\n" && single_character != "\r")
//			{
//				temp.append(single_character);
//			}
//
//			//The line from one function ends with "-", if there are two arguments for one functions, they are seperated with ","
//			if (single_character == "," || single_character == "-")
//			{
//				function_arguments.push_back(temp);
//				temp.clear();
//			}
//
//			if (single_character == "}")//The last number doesn't have a comma following, so it has to be read when the bracelet is found
//			{
//				for (int i = 0; i < function_list.size(); i++)
//				{
//					m_complete_sequences.push_back({function_list.at(i),function_arguments.at(i) });
//				}
//				temp.clear();
//			}
//		}
//
//
//
//}

void Sequence_Handler::loadSeq(std::string path)
{
	std::string temp;
	std::string single_character;
	std::string sequence_name;//String to hold the name of the sequence. Used in the map of complete sequences as key
	temp.reserve(100);

	int sequence_step = 0;
	int param_int_nmbr = 0;
	int param_string_nmbr = 0;

	bool seq_step_awaits_string_params = false;//Set this variable to false if the sequence step (function) that is loaded takes integer arguments
	bool begin_reading_seq_step_params = false;//Set this variable TRUE if the SEQ_FUNCTION_TYPE is read and the arguments are found in the .txt file (they are in () brackets)

	std::ifstream file(m_getProjectDirPath() + path, std::ios::binary | std::ios::ate);
	if (!file)
		return;

	std::streamsize length = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> buffer(length);
	file.read(buffer.data(), length);
	file.close();

	m_p_logger->writeLog(LogLevel::INFO, m_log_origin + " LOAD_STORED_SEQUENCES",
		"Reading: " + std::to_string(length) + " characters");

	m_complete_sequence.clear();
	m_complete_sequence.reserve(50);

	for (std::size_t i = 0; i < buffer.size(); ++i)
	{
		single_character = buffer[i];

		if (single_character != "#" && single_character != "{" && single_character != "}" && single_character != "(" && single_character != ")" && single_character != ";" && single_character != "\n" && single_character != "\r")
		{
			temp.append(single_character);
			
		}
			
		if (single_character == "{")
		{
			sequence_name = temp;
			temp.clear();
		}

		//If Functiontype is found, set the correct enum
		{
			if (temp == "WAIT")
			{
				m_complete_sequence.push_back({ SEQ_FUNCTION_TYPE::NOT_DEFINED, 0, 0, "", "" });
				m_complete_sequence.at(sequence_step).type = SEQ_FUNCTION_TYPE::WAIT;
				seq_step_awaits_string_params = false;
				temp.clear();
			}
			else if (temp == "GET_DIGITAL_INPUT")
			{
				m_complete_sequence.push_back({ SEQ_FUNCTION_TYPE::NOT_DEFINED, 0, 0, "", "" });
				m_complete_sequence.at(sequence_step).type = SEQ_FUNCTION_TYPE::GET_DIGITAL_INPUT;
				seq_step_awaits_string_params = true;
				temp.clear();
			}
			else if (temp == "GET_DOUBLE_DIGITAL_INPUT")
			{
				m_complete_sequence.push_back({ SEQ_FUNCTION_TYPE::NOT_DEFINED, 0, 0, "", "" });
				m_complete_sequence.at(sequence_step).type = SEQ_FUNCTION_TYPE::GET_DOUBLE_DIGITAL_INPUT;
				seq_step_awaits_string_params = true;
				temp.clear();
			}
			else if (temp == "SWITCH_DIGITAL_OUTPUT")
			{
				m_complete_sequence.push_back({ SEQ_FUNCTION_TYPE::NOT_DEFINED, 0, 0, "", "" });
				m_complete_sequence.at(sequence_step).type = SEQ_FUNCTION_TYPE::SWITCH_DIGITAL_OUTPUT;
				seq_step_awaits_string_params = true;
				temp.clear();
			}
			else if (temp == "SET_DIGITAL_OUTPUT")
			{
				m_complete_sequence.push_back({ SEQ_FUNCTION_TYPE::NOT_DEFINED, 0, 0, "", "" });
				m_complete_sequence.at(sequence_step).type = SEQ_FUNCTION_TYPE::SET_DIGITAL_OUTPUT;
				seq_step_awaits_string_params = true;
				temp.clear();
			}

			if (single_character == "(")
			{
				begin_reading_seq_step_params = true;
				temp.clear();
			}
		}

		//Reading the function parameters
		{
			if (begin_reading_seq_step_params == true && (single_character == "," || single_character == ")"))
			{
				if (seq_step_awaits_string_params)
				{
					if (param_string_nmbr == 0)
					{
						m_complete_sequence.at(sequence_step).param_string1 = temp;
						param_string_nmbr++;
						temp.clear();
					}
					else if (param_string_nmbr == 1)
					{
						m_complete_sequence.at(sequence_step).param_string2 = temp;

						temp.clear();
					}

				}
				if (!seq_step_awaits_string_params)
				{
					if (param_int_nmbr == 0)
					{
						m_complete_sequence.at(sequence_step).param_int1 = std::stoi(temp);
						param_int_nmbr++;
						temp.clear();
					}
					else if (param_int_nmbr == 1)
					{
						m_complete_sequence.at(sequence_step).param_int2 = std::stoi(temp);
						temp.clear();
					}
				}


			}
		}

		//Finish reading one sequence step with ";"
		if (single_character == ";")
		{
			sequence_step++;
			param_int_nmbr = 0;
			param_string_nmbr = 0;
		}

		//Finish reading hole sequence with "}"
		if (single_character == "}")
		{
			m_complete_sequence_map.insert({ sequence_name, m_complete_sequence });
			sequence_step = 0;
			m_complete_sequence.clear();

		}

	}
}

void Sequence_Handler::startSequence(std::string name)
{
	std::thread(&Sequence_Handler::m_playSequence, this, name).detach();
}

void Sequence_Handler::m_playSequence(std::string name)
{
	for (int i = 0; i < m_complete_sequence_map.at(name).size(); i++)
	{
		switch (m_complete_sequence_map.at(name).at(i).type)
		{
		case SEQ_FUNCTION_TYPE::WAIT:
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(m_complete_sequence_map.at(name).at(i).param_int1));
		}break;
		case SEQ_FUNCTION_TYPE::GET_DIGITAL_INPUT:
		{
			m_p_hw_con->getDigitalInputState(m_complete_sequence_map.at(name).at(i).param_string1);
		}break;
		case SEQ_FUNCTION_TYPE::SWITCH_DIGITAL_OUTPUT:
		{
			m_p_hw_con->switchDigitalOutputState(m_complete_sequence_map.at(name).at(i).param_string1);
		}break;
		}
	}
}











std::string Sequence_Handler::m_getProjectDirPath()
{
	std::filesystem::path project_dir = std::filesystem::current_path().parent_path().parent_path().parent_path();
	return project_dir.string();
}

