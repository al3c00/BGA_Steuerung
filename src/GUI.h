#ifndef GUI_H
#define GUI_H

#include <map>


#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>


#include "Log.h"
#include "Render.h"

//class GUI.h
//@brief class to handle the shown object on the display. Possible to create multiple instances of this class to make changing of shown objects on the display simpler
//@brief All instances should use the same renderer
class GUI
{
public:
	GUI(Log* logger, Render* renderer);

	void setBackGroundColor(int r, int g, int b);//Set the Background color
	void drawBackGroundColor();
	void drawObject(int x_pos, int y_pos, int height, int width, std::string path);
	void drawObject(int x_pos, int y_pos, int height, int width, int r, int g, int b);
	void drawPreloadedTexture(int x_pos, int y_pos, int height, int width, std::string name);

	//@brief Function to draw the text in the backbuffer
	//@param x_pos x-Position (Bottom left of the text element)
	//@param y_pos y-Position (Bottom left of the text element)
	//@param text/numbers Value, that should be displayed
	//@param font Font-variant from collection that should be used
	void drawText_l(int x_pos, int y_pos, std::string text, std::string font);
	void drawText_l(int x_pos, int y_pos, int numbers, std::string font);

	//@brief Function to draw the text in the backbuffer
	//@param x_pos x-Position (Bottom right of the text element)
	//@param y_pos y-Position (Bottom right of the text element)
	//@param text/numbers Value, that should be displayed
	//@param font Font-variant from collection that should be used
	void drawText_r(int x_pos, int y_pos, std::string text, std::string font);
	void drawText_r(int x_pos, int y_pos, int numbers, std::string font);

	//@brief Function to preload textures into a map
	//@param name Key(name) to refer to the texture
	//@param path Path to the texture
	void loadTexture(std::string name, std::string path);


	//@brief Function to preload fonts into a map. Structure: map<fontname, map_of_font>
	//@brief Specifically: This functions pushes a map (key:char, value: SDL_Texture*) into a "collection" map (key: string, value: the texture map)
	//@param name Name for the font: Should be compount of size and color
	//@param path The path to the font file
	//@param r,g,b The font color
	void loadFont(std::string name, std::string path, int r, int g, int b);

	void TESTDRAWTEXT();


private:
	//@brief Create a SDL_Surface and from that an SDL_Texture instance for every visible symbol in the ascii table (DEC33-DEC126)
	//@brief The instances ar saved in a map (font_map) and can be called by runtime to display text dynamicly. The key is a char type
	//@brief DEC32 (Space) is also included
	void m_insertFontInMap(std::string name);


	
	//Font Handling
	TTF_Font* m_p_SDL_font;//Pointer to a opened font 
	
	static SDL_Surface* s_m_p_ascii_symbols_surface_buffer[94];
	static std::map<char, SDL_Surface*> s_m_font_map_SURFACE;

	static SDL_Texture* s_m_p_ascii_symbols_texture_buffer[94];
	static std::map<char, SDL_Texture*> s_m_font_map_TEXTURE;

	static std::map<std::string, std::map<char, SDL_Texture*>> s_m_font_collection;

	static SDL_Color s_m_SDL_font_color;
	//End Font Handling


	Render* m_p_render_instance;
	Log* m_p_logger;

	
	std::map<std::string, SDL_Texture*> m_preloaded_textures_map;
	


	SDL_Color m_SDL_color_black;

	SDL_Surface* m_background_surface;
	SDL_Texture* m_background_texture;
	SDL_Color m_SDL_color_background;
	SDL_Rect m_background_rect;
	
	std::string getProjectDirPath();

	std::string m_log_origin;
};



#endif //!GUI_H