#ifndef HW_CON.h
#define HW_CON.h

#include <map>
#include <bitset>

#include "wiringPi.h"
#include "wiringPiI2C.h"
#include "Log.h"


class HW_Con
{
public:

	HW_Con(Log* logger);
	
	void loadDigitalInputAdresses(std::string path);
	void loadAnalogInputAdresses(std::string path);
	void loadDigitalOutputAdresses(std::string path);
	void loadIOMapConfig(std::string path);

	bool getDigitalInputState(std::string name);

	//@brief Use this function to get a simplified Input state of two seperate digital inputs, that check the same thing. E.g. a valve has a sensor D_In_0 for OPEN and a sensor D_In_1 for CLOSED
	//@param name1 Name "D_InX" of the first digitalInput on the PCB
	//@param name2 Name "D_InX" of the second digitalInput on the PCB
	//@return Returns 0 if the valve is closed (x active, x+1 inactive, 1 if the valve is open (x inactive, x+1 active), 2 if undefined (both the same state)
	int getDoubleInputState(std::string name1, std::string name2);

	void refreshDigitalInputStates();


private:


	Log* m_p_logger;


	//wiringPi I2C adresses
	uint16_t m_PCF_IO_32;
	uint16_t m_PCF_IO_33;
	uint16_t m_PCF_IO_34;
	uint16_t m_PCF_IO_35;
	uint16_t m_PCF_IO_36;

	struct IO_Object
	{
		int PCF8574_adresse;//Adresse of the PCF8574 (7-Bit number)
		int PCF_8574_port;//Port 0-7
		bool is_active;
	}m_io_object;

	std::map<std::string, IO_Object> m_PCF8574_DigitalIn_Adresse_collection;//is_active --> True if the Input is not active
	std::map<std::string, IO_Object> m_PCF8574_AnalogIn_Adresse_collection;
	std::map<std::string, IO_Object> m_PCF8574_DigitalOut_Adresse_collection;

	std::map<std::string, std::string> m_hw_map_config;//Map to connect the name of the IO_Adresse on the PCB with the real Hardware 



	std::string m_getProjectDirPath();
	

	std::string m_log_origin;

};






#endif // !HW_CON.h


