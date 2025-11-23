#include <iostream>
#include <chrono>


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
	GUI system_diagram(&logger, &renderer);
	

	system_diagram.loadFont("ARIA_Black", "/fonts/arial.ttf", 0, 0, 0);
	system_diagram.loadFont("ARIAL_Black", "/fonts/arial.ttf", 0, 0, 0);
	system_diagram.loadTexture("Schema", "/resource/schema_bga_v3.png");
	system_diagram.loadTexture("Arrow_Active", "/resource/circle_arrow.png");
	system_diagram.loadTexture("Schieber_Geschlossen", "/resource/schieber_geschlossen_rechteck_rot.png");

	system_diagram.loadXYWHPosition("/resource/drawable_objects_positions.txt");


	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point last_frame = std::chrono::system_clock::now();
	std::chrono::duration<double, std::milli> total_time_last_frame;
	int target_fps = 30;

	auto time = std::chrono::system_clock::now();
	std::time_t current_time; 
	std::string str_current_time;




	//Main loop
	//{
	//	clear renderer
	//	draw things in backbuffer, from back to front
	//	show things
	//}
	system_diagram.setBackGroundColor(175, 175, 175);
	int rotation = 0;
	do
	{

		now = std::chrono::system_clock::now();
		total_time_last_frame = now - last_frame;
		


		if (total_time_last_frame.count() > 1000/target_fps)
		{	
			time = now;
			current_time = std::chrono::system_clock::to_time_t(time);
			str_current_time = std::ctime(&current_time);
			//std::ctime inserts a newline at the end of the time data. Remove it with pop_back
			str_current_time.pop_back();

			renderer.clearRender();
			//m_p_renderer->prepareRender();

			//system_diagram.drawBackGroundColor();
			system_diagram.drawPreloadedTexture(0, 0, 800, 480, "Schema");

			system_diagram.drawPreloadedTextureXYWH("Pumpe1_Aktiv", rotation, "Arrow_Active");
			system_diagram.drawPreloadedTextureXYWH("Pumpe2_Aktiv", rotation, "Arrow_Active");
			system_diagram.drawPreloadedTextureXYWH("Schieber_1_Status", 90, "Schieber_Geschlossen");
			system_diagram.drawPreloadedTextureXYWH("Schieber_2_Status", 90, "Schieber_Geschlossen");
			system_diagram.drawPreloadedTextureXYWH("Schieber_3_Status", "Schieber_Geschlossen");
		
			//system_diagram.drawPreloadedTexture(80, 186, 15, 15, rotation, "Arrow_Active");
			//system_diagram.drawText_l(50, 200, "Hello World", "ARIAL_Black");
			//system_diagram.drawText_l(80, 400, "Hello World", "ARIAL_Black");
			system_diagram.drawText_l(349, 112, std::to_string(4850) + "kg", "ARIAL_Black");
			system_diagram.drawText_l(349, 130, std::to_string(75) + "%", "ARIAL_Black");
			system_diagram.drawText_l(7, 9, str_current_time, "ARIAL_Black");
			system_diagram.drawText_l(231, 9, "FPS: " + std::to_string(target_fps), "ARIAL_Black");
			system_diagram.drawText_r(473, 9, "Time(ms)/frame: " + std::to_string((int)total_time_last_frame.count()), "ARIAL_Black");



			renderer.Show();

			//SDL_Delay(15000);

			rotation++;
			if (rotation > 360)
			{
				rotation = 0;
			}

			last_frame = now;
		}

		

	} while (true);

	std::cout << "Hello" << std::endl;


	logger.writeLog(LogLevel::INFO, log_origin, "SUCCESSFUL");

	return 0;
}