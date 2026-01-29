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
//@brief The display height is 800, the display width is 480
class GUI
{
public:
	GUI(Log* logger, Render* renderer);

	void drawBackGroundColor(int r, int g, int b);
	
	//@brief Call before drawing anything in the frame
	void prepareCursor();

	//@brief Function to draw Textures, with the position set in the file: "drawable_objects_positions.txt"
	//@param x_pos,y_pos Bottom left position of the element on the screen --> 0,0 is the top left corner
	//@param r,g,b To draw a simple rectangle, give the colours that it should have
	//@param pos_object_name Name as the position data is called in the file
	//@param texture_name Name of the texture to draw
	//@param rotation Rotation value of the texture in degrees
	//@param state Use this function together with HW_CON::getDoubleInputState(), mainly for valves, that have to position sensors (one active when open, one active when closed)
	void drawVisualElement(int x_pos, int y_pos, int width, int height, int r, int g, int b, bool is_selectable);
	void drawVisualElement(int x_pos, int y_pos, int width, int height, std::string texture_name, bool is_selectable);
	void drawVisualElement(int x_pos, int y_pos, int width, int height, int rotation, std::string texture_name, bool is_selectable);
	void drawVisualElement(int x_pos, int y_pos, int width, int height, int state, bool is_selectable);


	//@brief Function to draw the text in the backbuffer
	//@param x_pos x-Position (Top left of the text element)
	//@param y_pos y-Position (Top left of the text element)
	//@param text/numbers Value, that should be displayed
	//@param font Font-variant from collection that should be used
	//@param box_r,box_g,box_b Color (RGB) that should surround the text
	void drawText_l(int x_pos, int y_pos, std::string text, std::string font);
	void drawText_l(int x_pos, int y_pos, std::string text, std::string font, int box_r, int box_g, int box_b);
	void drawText_l(int x_pos, int y_pos, int numbers, std::string font);

	//@brief Function to draw a vertical list in the backbuffer. The list components are given in a vector
	//@param x_pos x-Position (Top left of the text element)
	//@param y_pos y-Position (Top left of the text element)
	//@param height,width if the occupied space of text should not exceed a specific area, use those values
	//@param scrollpos if the text doesn't fit in the predefined space, change the first object of the vector that is getting displayed
	//@param spacing Vertical distance beetwen the bottom of texts
	//@param text Value that should be displayed
	//@param font Font-variant from collection that should be used
	void drawList_l(int x_pos, int y_pos, int spacing,  std::vector<std::string> text, std::string font);
	void drawList_l(int x_pos, int y_pos, int spacing, int width, int height, int scrollpos, std::vector<std::string> text, std::string font);
	void drawList_l(int x_pos, int y_pos, int spacing, int width, int height, bool scroll_forward, bool scroll_backward, std::vector<std::string> text, std::string font);
	

	//@brief Function to draw the text in the backbuffer
	//@param x_pos x-Position (Top right of the text element)
	//@param y_pos y-Position (Top right of the text element)
	//@param text/numbers Value, that should be displayed
	//@param font Font-variant from collection that should be used
	//@param box_r,box_g,box_b Color (RGB) that should surround the text
	void drawText_r(int x_pos, int y_pos, std::string text, std::string font);
	void drawText_r(int x_pos, int y_pos, int numbers, std::string font);
	void drawText_r(int x_pos, int y_pos, std::string text, std::string font, int box_r, int box_g, int box_b);
	

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

	void setScrollPos(int scrollpos);

	//void TESTDRAWTEXT();



private:

	//Currently object to draw the cursor around
	int m_cursor_pos;
	bool m_cursor_active;

	Render* m_p_render_instance;
	Log* m_p_logger;

	//last scrolling position, used for the list
	int m_scroll_position;

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


	//Struct that holds information about the currently drawn elements by the GUI.
	//Fill in the information in the "draw" call
	struct VisualElementInfo
	{
		std::string name;
		int xpos;
		int ypos;
		int width;
		int height;
		bool selectable;
	};

	std::vector<VisualElementInfo> m_drawn_elements;
	std::vector<int>m_pos_selectable_objects;//Position of the selectable objects in the m_drawn_elements vector
	
	std::map<std::string, SDL_Texture*> m_preloaded_textures_collection;
	
	

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