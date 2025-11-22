#include <iostream>


#include "Log.h"
#include "Control_App.h"

int main()
{
	std::string log_origin = "MAIN";//Create such a string variable in every space needed with an name which tells, from where the logger was called
	Log logger("/logs/logs.txt");

	SDL_Window* window;



	//Initialize SDL
	if (SDL_InitSubSystem(NULL) < 0)//Return Value: 0 on success, negative value on failure
	{
		logger.writeLog(LogLevel::ERROR, log_origin + " SDL_INIT", SDL_GetError());
		std::cout << SDL_GetError() << std::endl;
	}


	//Initialize IMG
	//According to description, no return error check, because the function return the flag-values
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	if (TTF_Init() < 0)
	{
		logger.writeLog(LogLevel::ERROR, log_origin + " TTF_INIT", TTF_GetError());
	}

	//Create Window
	window = SDL_CreateWindow("BGA_Steuerung", 0, 0, 480, 800, SDL_WINDOW_FULLSCREEN);
	if (!window)
	{
		logger.writeLog(LogLevel::ERROR, log_origin + " SDL_CREATE_WINDOW", SDL_GetError());
		std::cout << SDL_GetError() << std::endl;
	}

	//Create a renderer, that is used by every GUI instance
	Render renderer(&logger, window);
	
	//Create GUI instance (main gui)
	GUI main_gui(&logger, &renderer);
	





	//Main loop
	//{
	//	clear renderer
	//	draw things in backbuffer, from back to front
	//	show things
	//}
	main_gui.setBackGroundColor(175, 175, 175);
	main_gui.loadTexture("Schema", "/resource/schema_bga_v2.png");

	renderer.clearRender();
	//m_p_renderer->prepareRender();

	//main_gui.drawBackGroundColor();
	main_gui.drawPreloadedTexture(0, 0, 800, 480, "Schema");
	//main_gui.drawText_l(50, 200, "Hello World");
	//main_gui.drawText_r(430, 150, "Hello World");
	//main_gui.TESTDRAWTEXT();


	renderer.Show();


	std::cout << "Hello" << std::endl;

	TTF_CloseFont

	logger.writeLog(LogLevel::INFO, log_origin, "SUCCESSFUL");

	return 0;
}