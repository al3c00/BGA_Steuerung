#include "HW_Con.h"

HW_Con::HW_Con(Log* logger)
{
	m_p_logger = logger;
	m_log_origin = "HW_Handle";


	m_PCF_IO_32 = wiringPiI2CSetupInterface("/dev/i2c-1", 32);
	m_PCF_IO_33 = wiringPiI2CSetupInterface("/dev/i2c-1", 33);
	m_PCF_IO_34 = wiringPiI2CSetupInterface("/dev/i2c-1", 34);
	m_PCF_IO_35 = wiringPiI2CSetupInterface("/dev/i2c-1", 35);
	m_PCF_IO_36 = wiringPiI2CSetupInterface("/dev/i2c-1", 36);

}

void HW_Con::loadDigitalInputAdresses(std::string path)
{
	std::string temp;
	std::string single_character;
	std::string object_name;

	enum struct VARIANTS { PCF_ADRESSE = 0, PORT = 1 } variants;

	std::ifstream object_position_file(m_getProjectDirPath() + path);
	object_position_file.seekg(0, object_position_file.end);
	int length = object_position_file.tellg();
	object_position_file.seekg(0, object_position_file.beg);

	char* buffer = new char[length];

	m_p_logger->writeLog(LogLevel::INFO, m_log_origin + " LOAD_DIGITALINPUT_DATA", "Reading: " + std::to_string(length) + " characters");

	object_position_file.read(buffer, length);

	object_position_file.close();

	//Unsafe, file could be corrupted!!
	for (int i = 0; i < length; i++)
	{
		single_character = buffer[i];

		if (single_character != "#" && single_character != "{" && single_character != "}" && single_character != "," && single_character != "\n" && single_character != "\r")
		{
			temp.append(single_character);
		}

		//Check if a variant code word had been read (dec for PCF8574 7-Bit Adresse, p for the Port). If yes clear string and prepare to read and store actual data in struct
		{
			if (temp == "dec")
			{
				variants = VARIANTS::PCF_ADRESSE;
				temp.clear();
			}
			else if (temp == "p")
			{
				variants = VARIANTS::PORT;
				temp.clear();
			}
			
		}

		if (single_character == "{")
		{
			object_name = temp;
			temp.clear();
		}
		if (single_character == ",")
		{
			switch (variants)
			{
			
			case VARIANTS::PCF_ADRESSE:
			{
				m_io_object.PCF8574_adresse = std::stoi(temp);
				temp.clear();
			}break;
			case VARIANTS::PORT:
			{
				m_io_object.PCF_8574_port = std::stoi(temp);
				temp.clear();
			}
			default:
				break;
			}
		}
		if (single_character == "}")//The last number doesn't have a comma following, so it has to be read when the bracelet is found
		{
			m_io_object.PCF_8574_port = std::stoi(temp);
			temp.clear();
			m_io_object.is_active = false;
			m_PCF8574_DigitalIn_Adresse_collection.insert({ object_name, m_io_object });
		}


	}

}

void HW_Con::loadAnalogInputAdresses(std::string path)
{
	std::string temp;
	std::string single_character;
	std::string object_name;

	enum struct VARIANTS { PCF_ADRESSE = 0, PORT = 1 } variants;

	std::ifstream object_position_file(m_getProjectDirPath() + path);
	object_position_file.seekg(0, object_position_file.end);
	int length = object_position_file.tellg();
	object_position_file.seekg(0, object_position_file.beg);

	char* buffer = new char[length];

	m_p_logger->writeLog(LogLevel::INFO, m_log_origin + " LOAD_ANALOGINPUT_DATA", "Reading: " + std::to_string(length) + " characters");

	object_position_file.read(buffer, length);

	object_position_file.close();

	//Unsafe, file could be corrupted!!
	for (int i = 0; i < length; i++)
	{
		single_character = buffer[i];

		if (single_character != "#" && single_character != "{" && single_character != "}" && single_character != "," && single_character != "\n" && single_character != "\r")
		{
			temp.append(single_character);
		}

		//Check if a variant code word had been read (dec for PCF8574 7-Bit Adresse, p for the Port). If yes clear string and prepare to read and store actual data in struct
		{
			if (temp == "dec")
			{
				variants = VARIANTS::PCF_ADRESSE;
				temp.clear();
			}
			else if (temp == "p")
			{
				variants = VARIANTS::PORT;
				temp.clear();
			}

		}

		if (single_character == "{")
		{
			object_name = temp;
			temp.clear();
		}
		if (single_character == ",")
		{
			switch (variants)
			{

			case VARIANTS::PCF_ADRESSE:
			{
				m_io_object.PCF8574_adresse = std::stoi(temp);
				temp.clear();
			}break;
			case VARIANTS::PORT:
			{
				m_io_object.PCF_8574_port = std::stoi(temp);
				temp.clear();
			}
			default:
				break;
			}
		}
		if (single_character == "}")//The last number doesn't have a comma following, so it has to be read when the bracelet is found
		{
			m_io_object.PCF_8574_port = std::stoi(temp);
			temp.clear();
			m_io_object.is_active = false;
			m_PCF8574_AnalogIn_Adresse_collection.insert({ object_name, m_io_object });
		}


	}

}

void HW_Con::loadDigitalOutputAdresses(std::string path)
{
	std::string temp;
	std::string single_character;
	std::string object_name;

	enum struct VARIANTS { PCF_ADRESSE = 0, PORT = 1 } variants;

	std::ifstream object_position_file(m_getProjectDirPath() + path);
	object_position_file.seekg(0, object_position_file.end);
	int length = object_position_file.tellg();
	object_position_file.seekg(0, object_position_file.beg);

	char* buffer = new char[length];

	m_p_logger->writeLog(LogLevel::INFO, m_log_origin + " LOAD_DIGITALINPUT_DATA", "Reading: " + std::to_string(length) + " characters");

	object_position_file.read(buffer, length);

	object_position_file.close();

	//Unsafe, file could be corrupted!!
	for (int i = 0; i < length; i++)
	{
		single_character = buffer[i];

		if (single_character != "#" && single_character != "{" && single_character != "}" && single_character != "," && single_character != "\n" && single_character != "\r")
		{
			temp.append(single_character);
		}

		//Check if a variant code word had been read (dec for PCF8574 7-Bit Adresse, p for the Port). If yes clear string and prepare to read and store actual data in struct
		{
			if (temp == "dec")
			{
				variants = VARIANTS::PCF_ADRESSE;
				temp.clear();
			}
			else if (temp == "p")
			{
				variants = VARIANTS::PORT;
				temp.clear();
			}

		}

		if (single_character == "{")
		{
			object_name = temp;
			temp.clear();
		}
		if (single_character == ",")
		{
			switch (variants)
			{

			case VARIANTS::PCF_ADRESSE:
			{
				m_io_object.PCF8574_adresse = std::stoi(temp);
				temp.clear();
			}break;
			case VARIANTS::PORT:
			{
				m_io_object.PCF_8574_port = std::stoi(temp);
				temp.clear();
			}
			default:
				break;
			}
		}
		if (single_character == "}")//The last number doesn't have a comma following, so it has to be read when the bracelet is found
		{
			m_io_object.PCF_8574_port = std::stoi(temp);
			temp.clear();
			m_io_object.is_active = false;
			m_PCF8574_DigitalOut_Adresse_collection.insert({ object_name, m_io_object });
		}


	}

}

void HW_Con::loadIOMapConfig(std::string path)
{
	std::string temp;
	std::string single_character;
	std::string object_name;


	std::ifstream object_position_file(m_getProjectDirPath() + path);
	object_position_file.seekg(0, object_position_file.end);
	int length = object_position_file.tellg();
	object_position_file.seekg(0, object_position_file.beg);

	char* buffer = new char[length];

	m_p_logger->writeLog(LogLevel::INFO, m_log_origin + " LOAD_IO_MAP_DATA", "Reading: " + std::to_string(length) + " characters");

	object_position_file.read(buffer, length);

	object_position_file.close();

	//Unsafe, file could be corrupted!!
	for (int i = 0; i < length; i++)
	{
		single_character = buffer[i];

		if (single_character != "#" && single_character != "{" && single_character != "}" && single_character != "," && single_character != "\n" && single_character != "\r")
		{
			temp.append(single_character);
		}

		if (single_character == "{")
		{
			object_name = temp;
			temp.clear();
		}


		if (single_character == "}")//The last number doesn't have a comma following, so it has to be read when the bracelet is found
		{
			m_hw_map_config.insert({ object_name, temp });
		}
	}
}

void HW_Con::initialisePCBRelayState()
{
	//Put together a 8-Bit message with all the ports (including the changed one) to send to the two D_Out addresses

	uint8_t msg_0_7 = 0;
	uint8_t msg_8_15 = 0;

	msg_0_7 = ((m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_0").is_active) << 7 |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_1").is_active << 6) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_2").is_active << 5) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_3").is_active << 4) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_4").is_active << 3) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_5").is_active << 2) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_6").is_active << 1) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_7").is_active));

	msg_8_15 = ((m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_8").is_active) << 7 |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_9").is_active << 6) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_10").is_active << 5) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_11").is_active << 4) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_12").is_active << 3) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_13").is_active << 2) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_14").is_active << 1) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_15").is_active));

	wiringPiI2CWrite(m_PCF_IO_35, msg_0_7);
	wiringPiI2CWrite(m_PCF_IO_36, msg_8_15);
}

bool HW_Con::getDigitalInputState(std::string name)
{
	return m_PCF8574_DigitalIn_Adresse_collection.at(name).is_active;
}

int HW_Con::getDoubleInputState(std::string name1, std::string name2)
{
	//Value in map is true when Input at D_in_X is inactive
	uint8_t input1 = m_PCF8574_DigitalIn_Adresse_collection.at(name1).is_active;
	uint8_t input2 = m_PCF8574_DigitalIn_Adresse_collection.at(name2).is_active;

	if (input2 < input1)
	{
		return 0;
	}
	if (input1 < input2)
	{
		return 1;
	}
	if (input2 == input1)
	{
		return 2;
	}

	
}

void HW_Con::switchDigitalOutputState(std::string name)
{
	//Get all the states as they are currently in the pcf8574
	//refreshDigitalOutputStates();

	//Switch the value, that we want to change
	m_PCF8574_DigitalOut_Adresse_collection.at(name).is_active = !m_PCF8574_DigitalOut_Adresse_collection.at(name).is_active;

	//Put together a 8-Bit message with all the ports (including the changed one) to send to the two D_Out addresses

	uint8_t msg_0_7 = 0;
	uint8_t msg_8_15 = 0;

	msg_0_7 = ((m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_0").is_active) << 7 |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_1").is_active << 6) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_2").is_active << 5) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_3").is_active << 4) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_4").is_active << 3) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_5").is_active << 2) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_6").is_active << 1) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_7").is_active));
		
	msg_8_15 = ((m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_8").is_active) << 7 |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_9").is_active << 6) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_10").is_active << 5) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_11").is_active << 4) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_12").is_active << 3) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_13").is_active << 2) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_14").is_active << 1) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_15").is_active));

	wiringPiI2CWrite(m_PCF_IO_35, msg_0_7);
	wiringPiI2CWrite(m_PCF_IO_36, msg_8_15);


}

void HW_Con::setDigitalOutputState(bool state, std::string name)
{
	if (!m_PCF8574_DigitalOut_Adresse_collection.at(name).is_active == state)
	{
	
		m_PCF8574_DigitalOut_Adresse_collection.at(name).is_active = state;
	
	uint8_t msg_0_7 = 0;
	uint8_t msg_8_15 = 0;

	msg_0_7 = ((m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_0").is_active) << 7 |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_1").is_active << 6) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_2").is_active << 5) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_3").is_active << 4) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_4").is_active << 3) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_5").is_active << 2) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_6").is_active << 1) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_7").is_active));

	msg_8_15 = ((m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_8").is_active) << 7 |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_9").is_active << 6) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_10").is_active << 5) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_11").is_active << 4) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_12").is_active << 3) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_13").is_active << 2) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_14").is_active << 1) |
		(m_PCF8574_DigitalOut_Adresse_collection.at("D_Out_15").is_active));

	wiringPiI2CWrite(m_PCF_IO_35, msg_0_7);
	wiringPiI2CWrite(m_PCF_IO_36, msg_8_15);
	}
}

bool HW_Con::getDigitalOutputState(std::string name)
{
	return false;
}



void HW_Con::refreshDigitalInputStates()
{
	uint8_t Input = wiringPiI2CRead(m_PCF_IO_32);//Get D_In 0-7

	bool ports[8];

	for (int i = 0; i < 8; i++)
	{
		ports[i] = 0 != (Input & (1 << i));
	}
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_0").is_active = ports[0];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_1").is_active = ports[1];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_2").is_active = ports[2];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_3").is_active = ports[3];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_4").is_active = ports[4];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_5").is_active = ports[5];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_6").is_active = ports[6];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_7").is_active = ports[7];

	Input = wiringPiI2CRead(m_PCF_IO_33);//Get D_In 8-15

	for (int i = 0; i < 8; i++)
	{
		ports[i] = 0 != (Input & (1 << i));
	}
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_8").is_active = ports[0];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_9").is_active = ports[1];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_10").is_active = ports[2];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_11").is_active = ports[3];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_12").is_active = ports[4];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_13").is_active = ports[5];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_14").is_active = ports[6];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_15").is_active = ports[7];

	Input = wiringPiI2CRead(m_PCF_IO_34);//Get D_In 16-23

	for (int i = 0; i < 8; i++)
	{
		ports[i] = 0 != (Input & (1 << i));
	}
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_16").is_active = ports[0];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_17").is_active = ports[1];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_18").is_active = ports[2];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_19").is_active = ports[3];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_20").is_active = ports[4];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_21").is_active = ports[5];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_22").is_active = ports[6];
	m_PCF8574_DigitalIn_Adresse_collection.at("D_In_23").is_active = ports[7];

}



std::string HW_Con::m_getProjectDirPath()
{
	std::filesystem::path project_dir = std::filesystem::current_path().parent_path().parent_path().parent_path();
	return project_dir.string();
}