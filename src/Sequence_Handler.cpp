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
	int param_nmbr = 0;
	

	bool seq_step_awaits_string_params = false;//Set this variable to false if the sequence step (function) that is loaded takes integer arguments
	bool seq_step_awaits_both_params = false;//Set this variable to true if the sequence step (function) that is loaded takes integer and string arguments. E.g. PROGRESS_IF. There can only (!!) be written one int and one string argument
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

		if (single_character != "#" && single_character != "{" && single_character != " " && single_character != "}" && single_character != "," && single_character != "(" && single_character != ")" && single_character != ";" && single_character != "\n" && single_character != "\r")
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
			else if (temp == "PROGRESS_IF")
			{
				m_complete_sequence.push_back({ SEQ_FUNCTION_TYPE::NOT_DEFINED, 0, 0, "", "" });
				m_complete_sequence.at(sequence_step).type = SEQ_FUNCTION_TYPE::PROGRESS_IF;
				seq_step_awaits_string_params = false;
				seq_step_awaits_both_params = true;
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
				if (seq_step_awaits_string_params && !seq_step_awaits_both_params)
				{
					if (param_nmbr == 0)
					{
						m_complete_sequence.at(sequence_step).param_string1 = temp;
						param_nmbr++;
						temp.clear();
					}
					else if (param_nmbr == 1)
					{
						m_complete_sequence.at(sequence_step).param_string2 = temp;
						param_nmbr = 0;
						temp.clear();
					}

				}
				else if (!seq_step_awaits_string_params && !seq_step_awaits_both_params)
				{
					if (param_nmbr == 0)
					{
						m_complete_sequence.at(sequence_step).param_int1 = std::stoi(temp);
						param_nmbr++;
						temp.clear();
					}
					else if (param_nmbr == 1)
					{
						m_complete_sequence.at(sequence_step).param_int2 = std::stoi(temp);
						param_nmbr = 0;
						temp.clear();
					}
				}
				else if (seq_step_awaits_both_params)
				{
					if (param_nmbr == 0)
					{
						m_complete_sequence.at(sequence_step).param_int1 = std::stoi(temp);
						param_nmbr++;
						temp.clear();
					}
					else if (param_nmbr == 1)
					{
						m_complete_sequence.at(sequence_step).param_string1 = temp;
						temp.clear();
						param_nmbr++;
					}
					else if (param_nmbr == 2)
					{
						m_complete_sequence.at(sequence_step).param_string2 = temp;
						temp.clear();
						seq_step_awaits_both_params = false;
						param_nmbr = 0;

					}
				}


			}
		}

		//Finish reading one sequence step with ";"
		if (single_character == ";")
		{
			sequence_step++;
			param_nmbr = 0;
			
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

//enum SEQ_FUNCTION_TYPE { NOT_DEFINED, WAIT, PROGRESS_IF_A, GET_DIGITAL_INPUT, GET_DOUBLE_DIGITAL_INPUT, GET_ANALOG_INPUT, SET_DIGITAL_OUTPUT, SWITCH_DIGITAL_OUTPUT };

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
		case SEQ_FUNCTION_TYPE::PROGRESS_IF_1:
		{
			int should_be_value = m_complete_sequence_map.at(name).at(i).param_int1;
			std::string input_to_check1 = m_complete_sequence_map.at(name).at(i).param_string1;
			bool check_digital = false;
			int return_value;

			//Very weird and messed up part :) To get the type of input that should be checked, use pop_back on the string until only the firs symbol (A/D) is left
		
			char check_symbol = input_to_check1.at(0);


			//Check if a Digital or Analog Input should be read. Remove the last few symbols from the param to check (A_In_Xx or D_In_Xx) Check only the first symbol 
			if (check_symbol == 'A')
			{
				check_digital = false;
			}
			else if (check_symbol == 'D')
			{
				check_digital = true;
			}
			do
			{
				if (check_digital)
				{
					return_value = m_p_hw_con->getDigitalInputState(input_to_check1);
				}
				else if (!check_digital)
				{
					//return_value = m_p_hw_con->getDigitalInputState(m_complete_sequence_map.at(name).at(i).param_string1);
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(50));

			} while (return_value != should_be_value);

		}break;

		case SEQ_FUNCTION_TYPE::PROGRESS_IF_2:
		{
			int should_be_value = m_complete_sequence_map.at(name).at(i).param_int1;
			std::string input_to_check1 = m_complete_sequence_map.at(name).at(i).param_string1;
			std::string input_to_check2 = m_complete_sequence_map.at(name).at(i).param_string2;
			int return_value = 0;

			//Can only be used on digital inputs
		
			do
			{
				
				return_value = m_p_hw_con->getDoubleInputState(input_to_check1, input_to_check2);
			
				std::this_thread::sleep_for(std::chrono::milliseconds(50));

			} while (return_value != should_be_value);
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

