#ifndef CONTROLLAPP_H
#define CONTROLAPP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Log.h"
#include "Render.h"
#include "defines.h"
#include "GUI.h"


//@brief The main class of the application. Organises inputs and output to the electronics and user 
class Control_App
{
public:
	Control_App(Log* logger);//Also establishes SDL

	void Run();

	~Control_App();
	
private:
	SDL_Window* m_p_window;

	Log* m_p_logger;
	Render* m_p_renderer;//Renderer is created on the heap
	GUI* m_p_main_gui;//Used to display the main site (levels of reservoirs, pump/... state)

	std::string m_log_origin;
};

#endif //CONTROLLAPP_H
