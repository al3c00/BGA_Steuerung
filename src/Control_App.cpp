#include "Control_App.h"






Control_App::Control_App(Log* logger)
{
	m_log_origin = "CONTROL_APP";

	m_p_logger = logger;

	SDL_Window* window;

	//Initialize SDL
	if (SDL_InitSubSystem(NULL) < 0)//Return Value: 0 on success, negative value on failure
	{
		m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " SDL_INIT", SDL_GetError());
		std::cout << SDL_GetError();
	}
	

	//Initialize IMG
	//According to description, no erro, because the function return the flag-values
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	if (TTF_Init() < 0)
	{
		m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " TTF_INIT", TTF_GetError());
	}

	//Create Window
	window = SDL_CreateWindow("BGA_Steuerung", 0, 0, 480, 800, SDL_WINDOW_FULLSCREEN);
	if (!window)
	{
		logger->writeLog(LogLevel::ERROR, m_log_origin + " SDL_CREATE_WINDOW", SDL_GetError());
		std::cout << SDL_GetError();
	}

	//Create a renderer, that is used by every GUI instance
	Render renderer(logger,window);
	

	//Create GUI instance (main gui)
	GUI main_gui(&logger,&renderer);
	m_p_main_gui = &main_gui;





	//Main loop
	//{
	//	clear renderer
	//	draw things in backbuffer, from back to front
	//	show things
	//}
	main_gui.setBackGroundColor(175, 175, 175);
	main_gui.loadTexture("Schema", "/resource/schema_bga_v2.png");

	m_p_renderer->clearRender();
	//m_p_renderer->prepareRender();

	//main_gui.drawBackGroundColor();
	main_gui.drawPreloadedTexture(0, 0, 800, 480, "Schema");
	//main_gui.drawText_l(50, 200, "Hello World");
	//main_gui.drawText_r(430, 150, "Hello World");
	//main_gui.TESTDRAWTEXT();


	m_p_renderer->Show();

}


void Control_App::Run()
{

}

Control_App::~Control_App()
{
	
	SDL_Quit();
	IMG_Quit();

	m_p_logger->writeLog(LogLevel::INFO, m_log_origin + " | DESTRUCTOR", "Terminated SDL and IMG");
}
