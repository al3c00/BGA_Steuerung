#ifndef GUI_H
#define GUI_H

#include <map>


#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_blendmode.h>


#include "Log.h"
#include "Render.h"

//class GUI.h
//@brief class to handle the shown object on the display. Possible to create multiple instances of this class to make changing of shown objects on the display simpler
//@brief All instances should use the same renderer
class GUI
{
public:
	GUI(Log* logger, Render* renderer);

	
	void drawBackGroundColor(int r, int g, int b);

	void drawPreloadedTexture(int x_pos, int y_pos, int height, int width, std::string name);
	void drawPreloadedTexture(int x_pos, int y_pos, int height, int width, int rotation, std::string name);


	//@brief Function to draw Textures, with the position set in the file: "drawable_objects_positions.txt"
	//@param object_name Name as the position data is called in the file
	//@param texture_name Name of the texture to draw
	//@param rotation Rotation value of the texture in degrees
	//@param state Use this function together with HW_CON::getDoubleInputState(), mainly for valves, that have to position sensors (one active when open, one active when closed)
	void drawPreloadedTextureXYWH(std::string object_name,  std::string texture_name);
	void drawPreloadedTextureXYWH(std::string object_name, int rotation, std::string texture_name);
	void drawPreloadedTextureXYWH(std::string object_name, int state);


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

	//@brief Function to write a position (x and y value) of an object that needs frequent texture changing into a map
	//@param xpos X-Position of the object
	//@param ypos Y-Position of the object
	//@param width Width of the object
	//@param height Height of the object
	//@param name Name of the object (refer with the same in the function drawPreloadedTextureXYWH)
	void prepareXYWHPosition(int xpos, int ypos, int width, int height, std::string name);


	//@brief Loads the given position and size attributes from a text file and stores them into a map
	//@brief Basicly the same functionality as prepareXYWHPosition, except, that the positions are read from a file
	//@brief How to write the file: #Object_Name (refer with the same in the function drawPreloadedTextureXYWH) NEWLINE { NEWLINE xposXX,yposYY,wdataWW,hdataHH NEWLINE }
	//@brief Don't write empty spaces between the bracelets
	//@param path Path to the file
	void loadXYWHPosition(std::string path);

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


	//@brief Function to draw a rectangle arround the currently selected object
	//@brief Call this function one time per frame. The cursor is only drawn when "m_cursor_active" is active. Set with switchCursorActive
	void drawCursor();

	//@brief Switches the cursors state (On/Off)
	void switchCursorState();
	
	//@brief Sets the cursor into specific state and doesn't care, if it's already in this state
	//@param cursor_state True to show the cursor, false to hide
	void setCursorState(bool cursor_state);

	void useCursorInput();

	//@brief Function to change the selected object
	//@param direction Direction in which the cursor should be moved (-1 / +1)
	void moveCursor(int direction);

	//void TESTDRAWTEXT();


private:

	//Currently object to draw the cursor around
	int m_currently_selected_object_for_cursor;
	int m_ammount_selectable_objects;
	bool m_cursor_active;

	Render* m_p_render_instance;
	Log* m_p_logger;

	//Font Handling

	//@brief Create a SDL_Surface and from that an SDL_Texture instance for every visible symbol in the ascii table (DEC33-DEC126)
	//@brief The instances ar saved in a map (font_map) and can be called by runtime to display text dynamicly. The key is a char type
	//@brief DEC32 (Space) is also included
	void m_insertFontInMap(std::string name);

	TTF_Font* m_p_SDL_font;//Pointer to a opened font 
	
	static SDL_Surface* s_m_p_ascii_symbols_surface_buffer[94];
	static std::vector<SDL_Surface*> s_m_font_vector_SURFACE;

	static SDL_Texture* s_m_p_ascii_symbols_texture_buffer[94];
	static std::map<char, SDL_Texture*> s_m_font_map_TEXTURE;

	static std::map<std::string, std::map<char, SDL_Texture*>> s_m_font_collection;

	static SDL_Color s_m_SDL_font_color;
	//End Font Handling


	//Struct that holds information about the drawn elements.
	//Fill in the information in the "draw" call
	struct DrawnElements
	{
		std::string name;
		int xpos;
		int ypos;
		int width;
		int height;
		bool selectable;
		int nmbr_selectable_object; //Holds the number used by the cursor to address it

	}m_drawn_elements_list;

	std::vector<DrawnElements> m_drawn_elements_list_vector;

	


	
	std::map<std::string, SDL_Texture*> m_preloaded_textures_collection;
	
	struct Element_Pos
	{
		uint16_t xpos;
		uint16_t ypos;
		uint16_t width;
		uint16_t height;
		int nmbr_selectable_object; //Holds the number used by the cursor to address it
	}m_element_pos;//Struct to save the element info temporary

	std::map<std::string, Element_Pos> m_loaded_elements_pos;//Map to save the element info while the programm is running. Get infos about a specific object by the name it is called in the .txt file

	SDL_Surface* m_background_surface;
	SDL_Texture* m_background_texture;

	SDL_Surface* m_cursor_surface;
	SDL_Surface* m_cursor_texture;

	SDL_Color m_SDL_color_background;
	SDL_Rect m_background_rect;
	
	std::string m_getProjectDirPath();

	std::string m_log_origin;
};



#endif //!GUI_H