#include "Sequence_Handler.h"

Sequence_Handler::Sequence_Handler(Log* logger, std::shared_ptr<HW_Con> hw_con)
{
	m_p_logger = logger;
	m_p_hw_con = hw_con;


	
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

	m_functions["DELAY"] = [](const std::vector<std::string>& args)
	{
			int ms = std::stoi(args.at(0));
			std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	};
}

void Sequence_Handler::playSequence()
{
	std::thread(&Sequence_Handler::test, this).detach();
}

Sequence_Handler::~Sequence_Handler()
{

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
