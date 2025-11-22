#ifndef RENDER_H
#define RENDER_H

#include <mutex>

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>

#include "Log.h"


//@brief This class handles all the render pipeline. 
//@brief All objects that appear on the screen call this class
class Render
{
public:
	Render(Log* logger, SDL_Window* window);
	~Render();
	void clearRender();//Clear the backbuffer, call at first position in loop
	void prepareRender();//Fill the backbuffer with different objects
	void Show();
	
	SDL_Rect getDisplaySize();

	SDL_Renderer* getRenderer();
	
private:

	SDL_Renderer* m_SDLrenderer;//member pointer to the SDL_Render instance
	SDL_RendererInfo m_SDLrendererinfo;
	SDL_Rect full_display_rect;

	

	static bool is_SDL_defined;

	Log* m_p_logger;
	std::string m_log_origin;

	std::string getProjectDirPath();
};


#endif//RENDER_H

