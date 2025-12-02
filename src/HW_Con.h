#ifndef HW_CON.h
#define HW_CON.h

#include <map>
#include <bitset>

#include "wiringPi.h"
#include "wiringPiI2C.h"
#include "Log.h"

//Relays use pull-up resistors-->When the PCF port opens to gnd, the relay is not active 
class HW_Con
{
public:

	HW_Con(Log* logger);
	
	void loadDigitalInputAdresses(std::string path);
	void loadAnalogInputAdresses(std::string path);
	void loadDigitalOutputAdresses(std::string path);
	void loadIOMapConfig(std::string path);


	//@brief Set all relays outputs to inactive
	//@brief When the PCF8574 start up, the ports are closed to GND. The pull-up resistors activate the relays
	//@brief Call this function right after initialising the class instance to deactivate the relays
	//@brief The correct states are already set when loading in the config
	void initialisePCBRelayState();


	//@brief Gets the state of a digital input
	//@param name Name of the input pin ("D_in_X")
	//@return Returns true if the input is active (Signal line is tied to GND)
	bool getDigitalInputState(std::string name);

	//@brief Use this function to get a simplified Input state of two seperate digital inputs, that check the same thing. E.g. a valve has a sensor D_In_0 for OPEN and a sensor D_In_1 for CLOSED
	//@param name1 Name "D_In_X" of the first digitalInput on the PCB
	//@param name2 Name "D_In_X" of the second digitalInput on the PCB
	//@return Returns 0 if the valve is closed (x active, x+1 inactive, 1 if the valve is open (x inactive, x+1 active), 2 if undefined (both the same state)
	int getDoubleInputState(std::string name1, std::string name2);

	//@brief Switch the output state of one Ports 
	//@param name Name of the port according to the PCB-Design ("D_Out_X")
	void switchDigitalOutputState(std::string name);

	//@brief Sets the state and does't care if its already like that
	//@param state True --> Output active, False --> Outpute not active
	//@param name Name of the port according to the PCB-Design ("D_Out_X")
	void setDigitalOutputState(bool state, std::string name);
	
	

	//@brief Gets the SHOULD BE state according to the struct IO_Object, collected in the map xxx_DigitalOut_xxx_collection
	//@param name Name "D_Out_X"
	//@return Returns true if the port is closed (Relay is active), false if the port is open to GND (Relay is not active)
	bool getDigitalOutputState(std::string name);


	void refreshDigitalInputStates();
	

private:


	Log* m_p_logger;


	//wiringPi I2C adresses
	uint16_t m_PCF_IO_32;//Digital_Input 0-7
	uint16_t m_PCF_IO_33;//Digital_Input 8-15
	uint16_t m_PCF_IO_34;//Digital_Input 16-23
	uint16_t m_PCF_IO_35;//Digital_Output 0-7
	uint16_t m_PCF_IO_36;//Digital_Output 8-15

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


