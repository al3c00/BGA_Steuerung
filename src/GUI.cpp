#include "GUI.h"

SDL_Surface* GUI::s_m_p_ascii_symbols_surface_buffer[94] = { 0 };
std::map<char, SDL_Surface*> GUI::s_m_font_map_SURFACE;

SDL_Texture* GUI::s_m_p_ascii_symbols_texture_buffer[94] = {0};
std::map<char, SDL_Texture*> GUI::s_m_font_map_TEXTURE;

std::map<std::string, std::map<char, SDL_Texture*>> GUI::s_m_font_collection;

SDL_Color GUI::s_m_SDL_font_color;

GUI::GUI(Log* logger, Render* renderer)
{
	m_log_origin = "GUI";
	m_p_logger = logger;

	m_p_render_instance = renderer;


	m_background_rect.x = 0;
	m_background_rect.y = 0;
	m_background_rect.w = m_p_render_instance->getDisplaySize().w;
	m_background_rect.h = m_p_render_instance->getDisplaySize().h;

}



void GUI::setBackGroundColor(int r, int g, int b)
{
	/*Uint32 r_mask, g_mask, b_mask, a_mask;
	r_mask = 0xFF000000;
	g_mask = 0x00FF0000;
	b_mask = 0x0000FF00;
	a_mask = 0x000000FF;

	m_background_surface = SDL_CreateRGBSurface(NULL, m_background_rect.w, m_background_rect.h, 32, r_mask, g_mask, b_mask, a_mask);
	if (!m_background_surface)
	{
		m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " SDL_CREATE_BACKGROUND_SURFACE", SDL_GetError());
	}
	m_background_texture = SDL_CreateTextureFromSurface(m_p_render_instance->getRenderer(), m_background_surface);
	if (!m_background_texture)
	{
		m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " SDL_CREATE_BACKGROUND_TEXTURE", SDL_GetError());
	}*/

	SDL_SetRenderDrawColor(m_p_render_instance->getRenderer(), r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(m_p_render_instance->getRenderer(), &m_background_rect);
}

void GUI::drawBackGroundColor()
{
	SDL_RenderCopy(m_p_render_instance->getRenderer(), m_background_texture, NULL, &m_background_rect);
}

void GUI::drawObject( int x_pos, int y_pos, int height, int width, std::string path)
{

}

void GUI::drawObject(int x_pos, int y_pos, int height, int width, int r, int g, int b)
{

}

void GUI::drawPreloadedTexture(int x_pos, int y_pos, int height, int width, std::string name)
{
	SDL_Rect draw_rect;
	draw_rect.x = x_pos;
	draw_rect.y = y_pos;
	draw_rect.h = height;
	draw_rect.w = width;
	SDL_RenderCopy(m_p_render_instance->getRenderer(), m_preloaded_textures_map.at(name), NULL, &draw_rect);
}



void GUI::drawText_l(int x_pos, int y_pos, std::string text, std::string font)
{
	int nmbr_of_characters = text.length();
	
	SDL_Rect rect_text;
	rect_text.x = x_pos;
	rect_text.y = y_pos;

	int symbol_width = 0;
	int symbol_height = 0;

	for (int i = 0; i < nmbr_of_characters; i++)
	{
		char single_symbol = text.at(i);
		//std::cout << "Single symbol: " <<single_symbol << std::endl;
		
		if (SDL_QueryTexture(s_m_font_collection.at(font).at(single_symbol), NULL, NULL, &symbol_width, &symbol_height) < 0)
		{
			m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " SDL_QUERY_TEXTURE", SDL_GetError());
		}
		
		rect_text.w = symbol_width;
		rect_text.h = symbol_height;

		if (SDL_RenderCopy(m_p_render_instance->getRenderer(), s_m_font_collection.at(font).at(single_symbol), NULL, &rect_text) < 0)
		{
			std::cout << "Error SDL_RenderCopy" << std::endl;
			m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " DRAW-TEXT__SDL_RENDER_COPY", SDL_GetError());
		}


		rect_text.x += rect_text.w;
	}
	
	std::cout << "Text height: " << rect_text.h << std::endl;

}

void GUI::drawText_l(int x_pos, int y_pos, int numbers, std::string font)
{
	drawText_l(x_pos, y_pos, std::to_string(numbers), font);
}

void GUI::drawText_r(int x_pos, int y_pos, std::string text, std::string font)
{
	int nmbr_of_characters = text.length();

	SDL_Rect rect_text;
	rect_text.x = x_pos;
	rect_text.y = y_pos;

	int symbol_width = 0;
	int symbol_height = 0;

	int total_width = 0;

	//Check and add up the total width of all the symbols
	for (int i = 0; i < nmbr_of_characters; i++)
	{
		char single_symbol = text.at(i);

		if (SDL_QueryTexture(s_m_font_collection.at(font).at(single_symbol), NULL, NULL, &symbol_width, &symbol_height) < 0)
		{
			m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " SDL_QUERY_TEXTURE", SDL_GetError());
		}

		total_width += symbol_width;
	}

	rect_text.x -= total_width;

	for (int i = 0; i < nmbr_of_characters; i++)
	{
		char single_symbol = text.at(i);
		//std::cout << "Single symbol: " << single_symbol << std::endl;

		if (SDL_QueryTexture(s_m_font_collection.at(font).at(single_symbol), NULL, NULL, &symbol_width, &symbol_height) < 0)
		{
			m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " SDL_QUERY_TEXTURE", SDL_GetError());
		}

		rect_text.w = symbol_width;
		rect_text.h = symbol_height;

		if (SDL_RenderCopy(m_p_render_instance->getRenderer(), s_m_font_collection.at(font).at(single_symbol), NULL, &rect_text) < 0)
		{
			std::cout << "Error SDL_RenderCopy" << std::endl;
			m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " DRAW-TEXT__SDL_RENDER_COPY", SDL_GetError());
		}


		rect_text.x += rect_text.w;
	}



}

void GUI::drawText_r(int x_pos, int y_pos, int numbers, std::string font)
{
	drawText_r(x_pos, y_pos, std::to_string(numbers), font);
}

void GUI::loadTexture(std::string name, std::string path)
{
	std::string temp_path = getProjectDirPath() + path;
	m_preloaded_textures_map.insert({ name, IMG_LoadTexture(m_p_render_instance->getRenderer(), temp_path.c_str()) });
}

void GUI::loadFont(std::string name, std::string path, int r, int g, int b)
{


	//Load TTF_Font
	m_p_SDL_font = TTF_OpenFont((getProjectDirPath() + path).c_str(),15);
	if (!m_p_SDL_font)
	{
		m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " TTF_OPEN_FONT", SDL_GetError());
	}

	s_m_SDL_font_color.r = r;
	s_m_SDL_font_color.g = g;
	s_m_SDL_font_color.b = b;

	m_insertFontInMap(name);

	TTF_CloseFont(m_p_SDL_font);
}

void GUI::TESTDRAWTEXT()
{
	SDL_Surface* surface = TTF_RenderUTF8_Solid(m_p_SDL_font, "TEST", m_SDL_color_black);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(m_p_render_instance->getRenderer(), surface);
	SDL_Rect Message_rect; //create a rect
	Message_rect.x = 100;  //controls the rect's x coordinate 
	Message_rect.y = 100; // controls the rect's y coordinte
	Message_rect.w = 100; // controls the width of the rect
	Message_rect.h = 100; // controls the height of the rect

	SDL_RenderCopy(m_p_render_instance->getRenderer(), Message, NULL, &Message_rect);
}

void GUI::m_insertFontInMap(std::string name)
{
	//The first visible character in the ascii table i "!" at DEC33, the last is "~" at DEC 126
	//DEC32 (Space) is also included '!' has ASCII Code DEC33
	for (int i = 0; i < 95; i++)
	{
		char temp = i + '!' -1;
		char buf[2] = { temp, '\0' };//Add nulltermination!!
		const char* symbol_for_Render = buf;
		std::cout << symbol_for_Render << std::endl;

		s_m_font_map_SURFACE.insert({ temp, TTF_RenderUTF8_Solid(m_p_SDL_font, symbol_for_Render, s_m_SDL_font_color)});


	}
		

	for (int i = 0; i < 95; i++)
	{
		char temp = i + '!' -1;

		std::cout << temp << std::endl;

		s_m_font_map_TEXTURE.insert({ temp, SDL_CreateTextureFromSurface(m_p_render_instance->getRenderer(), s_m_font_map_SURFACE.at(temp)) });
	}

	s_m_font_collection.insert({ name, s_m_font_map_TEXTURE });

	
	
}



std::string GUI::getProjectDirPath()
{
	std::filesystem::path project_dir = std::filesystem::current_path().parent_path().parent_path().parent_path();
	return project_dir.string();
}

