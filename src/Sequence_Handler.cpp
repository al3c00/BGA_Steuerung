#include "Sequence_Handler.h"

Sequence_Handler::Sequence_Handler(Log* logger, std::shared_ptr<HW_Con> hw_con)
{
	m_p_logger = logger;
	m_p_hw_con = hw_con;

	m_log_origin = "Sequence_Handler";

	m_sequence_return_vector.clear();
	m_sequence_return_vector.reserve(50);
	m_sequence_return_number = -1;

	m_running_threads = 0;
	m_stopped_threads = 0;
	m_total_threads = 0;

	m_currently_asked_sequence = 0;

	m_threads_list.reserve(20);
}



//enum SEQ_FUNCTION_TYPE { NOT_DEFINED, WAIT, PROGRESS_IF_A, GET_DIGITAL_INPUT, GET_DOUBLE_DIGITAL_INPUT, GET_ANALOG_INPUT, SET_DIGITAL_OUTPUT, SWITCH_DIGITAL_OUTPUT };

void Sequence_Handler::loadSequences(std::string path)
{

	m_sequence_dir = path;

	std::string temp;
	std::string single_character;
	temp.reserve(20);

	int sequence_step = 0;
	int param_nmbr = 0;

	bool seq_step_awaits_string_params = false;//Set this variable to false if the sequence step (function) that is loaded takes integer arguments
	bool seq_step_awaits_both_params = false;//Set this variable to true if the sequence step (function) that is loaded takes integer and string arguments. E.g. PROGRESS_IF. There can only (!!) be written one int and one string argument
	bool begin_reading_seq_step_params = false;//Set this variable TRUE if the SEQ_FUNCTION_TYPE is read and the arguments are found in the .txt file (they are in () brackets)

	m_running_threads = 0;

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
			"Loading: " + i.path().generic_string() + '\n');

		
		m_threads_list.push_back({ 0, true, "" }); //Create a new instance of ThreadInfo in the m_threads_list vector
		m_threads_list.at(currently_loading_sequence).sequence_functions.reserve(20);

		for (std::size_t i = 0; i < buffer.size(); ++i)
		{
			single_character = buffer[i];

			if (single_character != "#" && single_character != "{" && single_character != " " && single_character != "}" && single_character != "," && single_character != "(" && single_character != ")" && single_character != ";" && single_character != "\n" && single_character != "\r")
			{
				temp.append(single_character);

			}

			if (single_character == "{")
			{
				m_threads_list.at(currently_loading_sequence).sequence_name = temp;  
				temp.clear();
			}

			//If Functiontype is found, set the correct enum
			{
				if (temp == "WAIT_MS")
				{
					m_threads_list.at(currently_loading_sequence).sequence_functions.push_back({ "WAIT_MS:",  SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).type = SEQ_FUNCTION_TYPE::WAIT_MS;
					seq_step_awaits_string_params = false;
					temp.clear();
				}
				else if (temp == "WAIT_S")
				{
					m_threads_list.at(currently_loading_sequence).sequence_functions.push_back({ "WAIT_S:",  SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).type = SEQ_FUNCTION_TYPE::WAIT_S;
					seq_step_awaits_string_params = false;
					temp.clear();
				}
				else if (temp == "WAIT_MIN")
				{
					m_threads_list.at(currently_loading_sequence).sequence_functions.push_back({ "WAIT_MIN:",  SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).type = SEQ_FUNCTION_TYPE::WAIT_MIN;
					seq_step_awaits_string_params = false;
					temp.clear();
				}
				else if (temp == "WAIT_H")
				{
					m_threads_list.at(currently_loading_sequence).sequence_functions.push_back({ "WAIT_H:",  SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).type = SEQ_FUNCTION_TYPE::WAIT_H;
					seq_step_awaits_string_params = false;
					temp.clear();
				}
				else if (temp == "WAIT_UNTIL")
				{
					m_threads_list.at(currently_loading_sequence).sequence_functions.push_back({ "WAIT_UNTIL:",  SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).type = SEQ_FUNCTION_TYPE::WAIT_UNTIL;
					seq_step_awaits_string_params = false;
					temp.clear();
				}
				else if (temp == "JUMP_TO")
				{
					m_threads_list.at(currently_loading_sequence).sequence_functions.push_back({ "JUMP_TO", SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).type = SEQ_FUNCTION_TYPE::JUMP_TO;
					seq_step_awaits_string_params = false;
					seq_step_awaits_both_params = false;
					temp.clear();
				}

				else if (temp == "PROGRESS_IF_1")
				{
					m_threads_list.at(currently_loading_sequence).sequence_functions.push_back({ "PROGRESS_IF_1", SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).type = SEQ_FUNCTION_TYPE::PROGRESS_IF_1;
					seq_step_awaits_string_params = false;
					seq_step_awaits_both_params = true;
					temp.clear();
				}
				else if (temp == "GET_DIGITAL_INPUT")
				{
					m_threads_list.at(currently_loading_sequence).sequence_functions.push_back({ "PROGRESS_IF_2", SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).type = SEQ_FUNCTION_TYPE::GET_DIGITAL_INPUT;
					seq_step_awaits_string_params = true;
					temp.clear();
				}
				else if (temp == "GET_DOUBLE_DIGITAL_INPUT")
				{
					m_threads_list.at(currently_loading_sequence).sequence_functions.push_back({ "GET_DOUBLE_DIGITAL_INPUT", SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).type = SEQ_FUNCTION_TYPE::GET_DOUBLE_DIGITAL_INPUT;
					seq_step_awaits_string_params = true;
					temp.clear();
				}
				else if (temp == "SWITCH_DIGITAL_OUTPUT")
				{
					m_threads_list.at(currently_loading_sequence).sequence_functions.push_back({ "SWITCH_DIGITAL_OUTPUT", SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).type = SEQ_FUNCTION_TYPE::SWITCH_DIGITAL_OUTPUT;
					seq_step_awaits_string_params = true;
					temp.clear();
				}
				else if (temp == "SET_DIGITAL_OUTPUT")
				{
					m_threads_list.at(currently_loading_sequence).sequence_functions.push_back({ "SET_DIGITAL_OUTPUT", SEQ_FUNCTION_TYPE::NOT_DEFINED, -1, -1, "", "" });
					m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).type = SEQ_FUNCTION_TYPE::SET_DIGITAL_OUTPUT;
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
							m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).param_string1 = temp;
							param_nmbr++;
							temp.clear();
						}
						else if (param_nmbr == 1)
						{
							m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).param_string2 = temp;
							param_nmbr = 0;
							temp.clear();
						}

					}
					else if (!seq_step_awaits_string_params && !seq_step_awaits_both_params)
					{
						if (param_nmbr == 0)
						{
							m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).param_int1 = std::stoi(temp);
							param_nmbr++;
							temp.clear();
						}
						else if (param_nmbr == 1)
						{
							m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).param_int2 = std::stoi(temp);
							param_nmbr = 0;
							temp.clear();
						}
					}
					else if (seq_step_awaits_both_params)
					{
						if (param_nmbr == 0)
						{
							m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).param_int1 = std::stoi(temp);
							param_nmbr++;
							temp.clear();
						}
						else if (param_nmbr == 1)
						{
							m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).param_string1 = temp;
							temp.clear();
							param_nmbr++;
						}
						else if (param_nmbr == 2)
						{
							m_threads_list.at(currently_loading_sequence).sequence_functions.at(sequence_step).param_string2 = temp;
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
				sequence_step = 0;
			}

		}
		currently_loading_sequence++;
		m_running_threads++;
	}

}

void Sequence_Handler::startSequences()
{
	for (int i = 0; i < m_running_threads; i++)
	{
		std::thread(&Sequence_Handler::m_playSequence, this, std::ref(m_threads_list), m_total_threads).detach();
		m_total_threads++;
	}
}

int Sequence_Handler::getSequenceInfo_CurrentStep()
{
	return m_threads_list.at(m_currently_asked_sequence).current_execution_step;
}





std::vector<std::string> Sequence_Handler::getSequenceInfo_Functions()
{

	if (m_currently_asked_sequence != m_sequence_return_number)
	{
		m_sequence_return_vector.clear();

		m_sequence_return_number = m_currently_asked_sequence;

		
		for (int i = 0; i < m_threads_list.at(m_currently_asked_sequence).sequence_functions.size(); i++)
		{
			//Getting function name
			m_sequence_return_vector.push_back(m_threads_list.at(m_currently_asked_sequence).sequence_functions.at(i).seq_function_name);

			//Getting the params
			//In the return vector .at(0) is START, so the add 1 to i when appending the params
			if (m_threads_list.at(m_currently_asked_sequence).sequence_functions.at(i).param_int1 != -1)
			{
				m_sequence_return_vector.at(i).append("(" + std::to_string(m_threads_list.at(m_currently_asked_sequence).sequence_functions.at(i).param_int1)+ ")");
			}
			if (m_threads_list.at(m_currently_asked_sequence).sequence_functions.at(i).param_int2 != -1)
			{
				m_sequence_return_vector.at(i).append("(" + std::to_string(m_threads_list.at(m_currently_asked_sequence).sequence_functions.at(i).param_int2)+ ")");
			}
			if (!m_threads_list.at(m_currently_asked_sequence).sequence_functions.at(i).param_string1.empty())
			{
				m_sequence_return_vector.at(i).append("(" + m_threads_list.at(m_currently_asked_sequence).sequence_functions.at(i).param_string1 + ")");
			}
			if (m_threads_list.at(m_currently_asked_sequence).sequence_functions.at(i).param_string2.empty())
			{
				m_sequence_return_vector.at(i).append("(" + m_threads_list.at(m_currently_asked_sequence).sequence_functions.at(i).param_string2 + ")");
			}
		}


	}

	return m_sequence_return_vector;
	


}

int Sequence_Handler::getSequenceInfo_TotalSteps()
{
	return m_threads_list.at(m_currently_asked_sequence).sequence_functions.size();
}

std::string Sequence_Handler::getSequenceInfo_Name()
{
	return m_threads_list.at(m_currently_asked_sequence).sequence_name;
}

int Sequence_Handler::getNmbrOfRunningSequences()
{
	return m_total_threads;
}


void Sequence_Handler::switchThroughSequences()
{
	m_currently_asked_sequence++;
	if (m_currently_asked_sequence > m_threads_list.size() - 1)
	{
		m_currently_asked_sequence = m_total_threads - m_running_threads;
	}
}

int Sequence_Handler::getAskedSequence()
{
	return m_currently_asked_sequence;
}





void Sequence_Handler::m_playSequence(std::vector<ThreadInfo>& r_thread_info, int thread_number)
{
	int thread_nmbr = thread_number;

	for (int current_step = 0; current_step < r_thread_info.at(thread_nmbr).sequence_functions.size(); current_step++)
	{

		r_thread_info.at(thread_nmbr).current_execution_step = current_step;


		if (!m_threads_list.at(thread_nmbr).is_active)
		{
			break;
		}

		switch (r_thread_info.at(thread_nmbr).sequence_functions.at(current_step).type)
		{
			//wait functions in different time units: milliseconds, seconds, minutes, hours
		case SEQ_FUNCTION_TYPE::WAIT_MS:
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(r_thread_info.at(thread_nmbr).sequence_functions.at(current_step).param_int1));
		}break;

		case SEQ_FUNCTION_TYPE::WAIT_S:
		{
			std::this_thread::sleep_for(std::chrono::seconds(r_thread_info.at(thread_nmbr).sequence_functions.at(current_step).param_int1));
		}break;
		case SEQ_FUNCTION_TYPE::WAIT_MIN:
		{
			std::this_thread::sleep_for(std::chrono::minutes(r_thread_info.at(thread_nmbr).sequence_functions.at(current_step).param_int1));
		}break;
		case SEQ_FUNCTION_TYPE::WAIT_H:
		{
			std::this_thread::sleep_for(std::chrono::hours(r_thread_info.at(thread_nmbr).sequence_functions.at(current_step).param_int1));
		}break;

		//wait until specific time. Format: HHMM --> 1235 =  12:35, uses two int parameters
		case SEQ_FUNCTION_TYPE::WAIT_UNTIL:
		{
			std::time_t t_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			std::tm time_struct;
			localtime_r(&t_t, &time_struct);
			std::cout << "Starting to wait at: " << std::put_time(&time_struct, "%H:%M:%S") << std::endl;


			int hour = r_thread_info.at(thread_nmbr).sequence_functions.at(current_step).param_int1;
			int minute = r_thread_info.at(thread_nmbr).sequence_functions.at(current_step).param_int1;

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
			current_step = r_thread_info.at(thread_nmbr).sequence_functions.at(current_step).param_int1 - 1;//Need do decrease the target number by one because the for loop increases the step counter before the target function will be executet.
		}break;

		//Progress if the input criteria of ONE input is met
		case SEQ_FUNCTION_TYPE::PROGRESS_IF_1:
		{
			int should_be_value = r_thread_info.at(thread_nmbr).sequence_functions.at(current_step).param_int1;
			std::string input_to_check1 = r_thread_info.at(thread_nmbr).sequence_functions.at(current_step).param_string1;
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
			int should_be_value = r_thread_info.at(thread_nmbr).sequence_functions.at(current_step).param_int1;
			std::string input_to_check1 = r_thread_info.at(thread_nmbr).sequence_functions.at(current_step).param_string1;
			std::string input_to_check2 = r_thread_info.at(thread_nmbr).sequence_functions.at(current_step).param_string2;
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
			m_p_hw_con->getDigitalInputState(r_thread_info.at(thread_nmbr).sequence_functions.at(current_step).param_string1);
		}break;

		case SEQ_FUNCTION_TYPE::SWITCH_DIGITAL_OUTPUT:
		{
			m_p_hw_con->switchDigitalOutputState(r_thread_info.at(thread_nmbr).sequence_functions.at(current_step).param_string1);
		}break;
		case SEQ_FUNCTION_TYPE::SET_DIGITAL_OUTPUT:
		{
			m_p_hw_con->setDigitalOutputState(r_thread_info.at(thread_nmbr).sequence_functions.at(current_step).param_int1, r_thread_info.at(thread_nmbr).sequence_functions.at(current_step).param_string1);
		}break;
		}


	}
}

std::string Sequence_Handler::m_getProjectDirPath()
{
	std::filesystem::path project_dir = std::filesystem::current_path().parent_path().parent_path().parent_path();
	return project_dir.string();
}

