#include "Sequence_Handler.h"

Sequence_Handler::Sequence_Handler(Log* logger, std::shared_ptr<HW_Con> hw_con)
{
	m_p_logger = logger;
	m_p_hw_con = hw_con;

	m_log_origin = "Sequence_Handler";

	m_sequence_return_vector.clear();
	m_sequence_return_vector.reserve(50);
	m_sequence_return_number = -1;


}





//enum SEQ_FUNCTION_TYPE { NOT_DEFINED, WAIT, PROGRESS_IF_A, GET_DIGITAL_INPUT, GET_DOUBLE_DIGITAL_INPUT, GET_ANALOG_INPUT, SET_DIGITAL_OUTPUT, SWITCH_DIGITAL_OUTPUT };

void Sequence_Handler::loadSequences(std::string path)
{

	m_sequence_dir = path;

	std::string temp;
	std::string single_character;
	temp.reserve(100);

	int sequence_step = 0;
	int param_nmbr = 0;

	bool seq_step_awaits_string_params = false;//Set this variable to false if the sequence step (function) that is loaded takes integer arguments
	bool seq_step_awaits_both_params = false;//Set this variable to true if the sequence step (function) that is loaded takes integer and string arguments. E.g. PROGRESS_IF. There can only (!!) be written one int and one string argument
	bool begin_reading_seq_step_params = false;//Set this variable TRUE if the SEQ_FUNCTION_TYPE is read and the arguments are found in the .txt file (they are in () brackets)

	std::vector<Seq_Part_Info>sequence_parts;

	//Check, how many sequences are stored (numbr of files)
	int nmbr_seq = 0;
	std::filesystem::path path_to_sequences = m_getProjectDirPath() + path;
	
	for (auto& i : std::filesystem::directory_iterator(path_to_sequences))
	{
		nmbr_seq++;
	}

	m_p_logger->writeLog(LogLevel::INFO, m_log_origin + " SEQUENCE_LOADER", "Found " + std::to_string(nmbr_seq) + " sequences");


	int currently_loading_sequence = 0;
	for (auto& i : std::filesystem::recursive_directory_iterator(path_to_sequences))
	{

		std::ifstream file(i.path(), std::ios::binary | std::ios::ate);
		if (!file)
		{
			return;
		}

		std::streamsize length = file.tellg();

		file.seekg(0, std::ios::beg);

		std::vector<char> buffer(length);
		file.read(buffer.data(), length);
		file.close();

		m_p_logger->writeLog(LogLevel::INFO, m_log_origin + "  SEQUENCE_LOADER",
			"Loading: " + i.path().generic_string());

		sequence_parts.clear();
		sequence_parts.reserve(50);

		for (std::size_t i = 0; i < buffer.size(); ++i)
		{
			single_character = buffer[i];

			if (single_character != "#" && single_character != "{" && single_character != " " && single_character != "}" && single_character != "," && single_character != "(" && single_character != ")" && single_character != ";" && single_character != "\n" && single_character != "\r")
			{
				temp.append(single_character);

			}

			if (single_character == "{")
			{
				m_sequence_names.insert({ currently_loading_sequence, temp });
				temp.clear();
			}

			//If Functiontype is found, set the correct enum
			{
				if (temp == "WAIT_MS")
				{
					sequence_parts.push_back({ "WAIT_MS:",  SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					sequence_parts.at(sequence_step).type = SEQ_FUNCTION_TYPE::WAIT_MS;
					seq_step_awaits_string_params = false;
					temp.clear();
				}
				else if (temp == "WAIT_S")
				{
					sequence_parts.push_back({ "WAIT_S:",  SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					sequence_parts.at(sequence_step).type = SEQ_FUNCTION_TYPE::WAIT_S;
					seq_step_awaits_string_params = false;
					temp.clear();
				}
				else if (temp == "WAIT_MIN")
				{
					sequence_parts.push_back({ "WAIT_MIN:",  SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					sequence_parts.at(sequence_step).type = SEQ_FUNCTION_TYPE::WAIT_MIN;
					seq_step_awaits_string_params = false;
					temp.clear();
				}
				else if (temp == "WAIT_H")
				{
					sequence_parts.push_back({ "WAIT_H:",  SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					sequence_parts.at(sequence_step).type = SEQ_FUNCTION_TYPE::WAIT_H;
					seq_step_awaits_string_params = false;
					temp.clear();
				}
				else if (temp == "WAIT_UNTIL")
				{
					sequence_parts.push_back({ "WAIT_UNTIL:",  SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					sequence_parts.at(sequence_step).type = SEQ_FUNCTION_TYPE::WAIT_UNTIL;
					seq_step_awaits_string_params = false;
					temp.clear();
				}
				else if (temp == "JUMP_TO")
				{
					sequence_parts.push_back({ "JUMP_TO", SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					sequence_parts.at(sequence_step).type = SEQ_FUNCTION_TYPE::JUMP_TO;
					seq_step_awaits_string_params = false;
					seq_step_awaits_both_params = false;
					temp.clear();
				}

				else if (temp == "PROGRESS_IF_1")
				{
					sequence_parts.push_back({ "PROGRESS_IF_1", SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					sequence_parts.at(sequence_step).type = SEQ_FUNCTION_TYPE::PROGRESS_IF_1;
					seq_step_awaits_string_params = false;
					seq_step_awaits_both_params = true;
					temp.clear();
				}
				else if (temp == "GET_DIGITAL_INPUT")
				{
					sequence_parts.push_back({ "PROGRESS_IF_2", SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					sequence_parts.at(sequence_step).type = SEQ_FUNCTION_TYPE::GET_DIGITAL_INPUT;
					seq_step_awaits_string_params = true;
					temp.clear();
				}
				else if (temp == "GET_DOUBLE_DIGITAL_INPUT")
				{
					sequence_parts.push_back({ "GET_DOUBLE_DIGITAL_INPUT", SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					sequence_parts.at(sequence_step).type = SEQ_FUNCTION_TYPE::GET_DOUBLE_DIGITAL_INPUT;
					seq_step_awaits_string_params = true;
					temp.clear();
				}
				else if (temp == "SWITCH_DIGITAL_OUTPUT")
				{
					sequence_parts.push_back({ "SWITCH_DIGITAL_OUTPUT", SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					sequence_parts.at(sequence_step).type = SEQ_FUNCTION_TYPE::SWITCH_DIGITAL_OUTPUT;
					seq_step_awaits_string_params = true;
					temp.clear();
				}
				else if (temp == "SET_DIGITAL_OUTPUT")
				{
					sequence_parts.push_back({ "SET_DIGITAL_OUTPUT", SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					sequence_parts.at(sequence_step).type = SEQ_FUNCTION_TYPE::SET_DIGITAL_OUTPUT;
					seq_step_awaits_string_params = true;
					seq_step_awaits_both_params = true;
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
							sequence_parts.at(sequence_step).param_string1 = temp;
							param_nmbr++;
							temp.clear();
						}
						else if (param_nmbr == 1)
						{
							sequence_parts.at(sequence_step).param_string2 = temp;
							param_nmbr = 0;
							temp.clear();
						}

					}
					else if (!seq_step_awaits_string_params && !seq_step_awaits_both_params)
					{
						if (param_nmbr == 0)
						{
							sequence_parts.at(sequence_step).param_int1 = std::stoi(temp);
							param_nmbr++;
							temp.clear();
						}
						else if (param_nmbr == 1)
						{
							sequence_parts.at(sequence_step).param_int2 = std::stoi(temp);
							param_nmbr = 0;
							temp.clear();
						}
					}
					else if (seq_step_awaits_both_params)
					{
						if (param_nmbr == 0)
						{
							sequence_parts.at(sequence_step).param_int1 = std::stoi(temp);
							param_nmbr++;
							temp.clear();
						}
						else if (param_nmbr == 1)
						{
							sequence_parts.at(sequence_step).param_string1 = temp;
							temp.clear();
							param_nmbr++;
						}
						else if (param_nmbr == 2)
						{
							sequence_parts.at(sequence_step).param_string2 = temp;
							temp.clear();

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
				seq_step_awaits_both_params = false;

			}

			//Finish reading hole sequence with "}"
			if (single_character == "}")
			{
				m_sequences.push_back(sequence_parts);
				sequence_step = 0;
				sequence_parts.clear();

			}

		}
		currently_loading_sequence++;
	}
}

void Sequence_Handler::startSequences()
{
	for (int i = 0; i < m_sequences.size(); i++)
	{
		m_running_sequences_info.insert({ m_sequence_names.at(i), {0, false} });
		m_threads_list.push_back(std::thread(& Sequence_Handler::m_playSequenceN, this, m_sequence_names.at(i), std::ref(m_sequences), i, std::ref(m_running_sequences_info)));

	}
}



int Sequence_Handler::getExecutionStep(std::string name)
{
	return m_running_sequences_info.at(name).current_step;
}



std::vector<std::string> Sequence_Handler::getSequenceFunctions(int number)
{
	if (number > m_sequences.size() -1)
	{
		number = m_sequences.size() - 1;
	}
	if (number != m_sequence_return_number)
	{
		m_sequence_return_vector.clear();

		m_sequence_return_number = number;

		
		for (int i = 0; i < m_sequences.at(number).size(); i++)
		{
			//Getting function name
			m_sequence_return_vector.push_back(m_sequences.at(number).at(i).seq_function_name);

			//Getting the params
			//In the return vector .at(0) is START, so the add 1 to i when appending the params
			if (m_sequences.at(number).at(i).param_int1 != -1)
			{
				m_sequence_return_vector.at(i).append("(" + std::to_string(m_sequences.at(number).at(i).param_int1)+ ")");
			}
			if (m_sequences.at(number).at(i).param_int2 != -1)
			{
				m_sequence_return_vector.at(i).append("(" + std::to_string(m_sequences.at(number).at(i).param_int2)+ ")");
			}
			if (!m_sequences.at(number).at(i).param_string1.empty())
			{
				m_sequence_return_vector.at(i).append("(" + m_sequences.at(number).at(i).param_string1 + ")");
			}
			if (m_sequences.at(number).at(i).param_string2.empty())
			{
				m_sequence_return_vector.at(i).append("(" + m_sequences.at(number).at(i).param_string2 + ")");
			}
		}


	}

	return m_sequence_return_vector;
	


}

int Sequence_Handler::getSequenceStepAmmount(int number)
{
	return m_sequences.at(number).size();
}

std::string Sequence_Handler::getSequenceName(int number)
{
	if (number > m_sequences.size() - 1)
	{
		number = m_sequences.size() - 1;
	}
	return m_sequence_names.at(number);
}

int Sequence_Handler::getAmmountOfLoadedSequences()
{
	return m_sequences.size();
}



void Sequence_Handler::m_playSequenceN(std::string name, std::vector<std::vector<Seq_Part_Info>>& v_seq, int seq_nmbr, std::map<std::string, RunningSeqInfo>& info_map)
{
	
	for(int current_step = 0; current_step < v_seq.at(seq_nmbr).size(); current_step++)
	{

		info_map.at(name).current_step = current_step;

		

		//If the sequence is set to stop externaly, enter this condition and just wait 
		if (info_map.at(name).is_extern_paused)
		{
			do
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			} while (info_map.at(name).is_extern_paused);
		}

	

		switch (v_seq.at(seq_nmbr).at(current_step).type)
		{
			//wait functions in different time units: milliseconds, seconds, minutes, hours
		case SEQ_FUNCTION_TYPE::WAIT_MS:
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(v_seq.at(seq_nmbr).at(current_step).param_int1));
		}break;

		case SEQ_FUNCTION_TYPE::WAIT_S:
		{
			std::this_thread::sleep_for(std::chrono::seconds(v_seq.at(seq_nmbr).at(current_step).param_int1));
		}break;
		case SEQ_FUNCTION_TYPE::WAIT_MIN:
		{
			std::this_thread::sleep_for(std::chrono::minutes(v_seq.at(seq_nmbr).at(current_step).param_int1));
		}break;
		case SEQ_FUNCTION_TYPE::WAIT_H:
		{
			std::this_thread::sleep_for(std::chrono::hours(v_seq.at(seq_nmbr).at(current_step).param_int1));
		}break;

		//wait until specific time. Format: HHMM --> 1235 =  12:35, uses two int parameters
		case SEQ_FUNCTION_TYPE::WAIT_UNTIL:
		{
			std::time_t t_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::tm time_struct;
			localtime_r(&t_t, &time_struct);
			std::cout << "Starting to wait at: " << std::put_time(&time_struct, "%H:%M:%S") << std::endl;


			int hour = v_seq.at(seq_nmbr).at(current_step).param_int1;
			int minute = v_seq.at(seq_nmbr).at(current_step).param_int1;

			bool time_reached = false;
			do
			{
				std::this_thread::sleep_for(std::chrono::seconds(30));

				if (time_struct.tm_hour >= hour && time_struct.tm_hour <= hour + 1 && time_struct.tm_min >= minute && time_struct.tm_min <= minute + 1)
				{
					time_reached = true;
				}

			} while (!time_reached);

		}break;

		//Jump to a specific position of the sequence
		case SEQ_FUNCTION_TYPE::JUMP_TO:
		{
			current_step = v_seq.at(seq_nmbr).at(current_step).param_int1 -1;//Need do decrease the target number by one because the for loop increases the step counter before the target function will be executet.
		}break;

		//Progress if the input criteria of ONE input is met
		case SEQ_FUNCTION_TYPE::PROGRESS_IF_1:
		{
			int should_be_value = v_seq.at(seq_nmbr).at(current_step).param_int1;
			std::string input_to_check1 = v_seq.at(seq_nmbr).at(current_step).param_string1;
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

		//Progress if the input of TWO inputs are met
		//Use to check devices with to inputs (e.g. a cylinder where the in and out position both have a seperate sensor)
		//Use the conditions from HW_Con::getDoubleInputState
		case SEQ_FUNCTION_TYPE::PROGRESS_IF_2:
		{
			int should_be_value = v_seq.at(seq_nmbr).at(current_step).param_int1;
			std::string input_to_check1 = v_seq.at(seq_nmbr).at(current_step).param_string1;
			std::string input_to_check2 = v_seq.at(seq_nmbr).at(current_step).param_string2;
			int return_value = 0;

			//Can only be used on digital inputs
			//Sets a sleep time for 50ms and then checks again if the values are not met
			do
			{

				return_value = m_p_hw_con->getDoubleInputState(input_to_check1, input_to_check2);

				std::this_thread::sleep_for(std::chrono::milliseconds(50));

			} while (return_value != should_be_value);
		}break;

		case SEQ_FUNCTION_TYPE::GET_DIGITAL_INPUT:
		{
			m_p_hw_con->getDigitalInputState(v_seq.at(seq_nmbr).at(current_step).param_string1);
		}break;

		case SEQ_FUNCTION_TYPE::SWITCH_DIGITAL_OUTPUT:
		{
			m_p_hw_con->switchDigitalOutputState(v_seq.at(seq_nmbr).at(current_step).param_string1);
		}break;
		case SEQ_FUNCTION_TYPE::SET_DIGITAL_OUTPUT:
		{
			m_p_hw_con->setDigitalOutputState(v_seq.at(seq_nmbr).at(current_step).param_int1, v_seq.at(seq_nmbr).at(current_step).param_string1);
		}break;
		}


	}

}

std::string Sequence_Handler::m_getProjectDirPath()
{
	std::filesystem::path project_dir = std::filesystem::current_path().parent_path().parent_path().parent_path();
	return project_dir.string();
}

