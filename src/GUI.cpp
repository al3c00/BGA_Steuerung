#include "GUI.h"

SDL_Surface* GUI::s_m_p_ascii_symbols_surface_buffer[94] = { 0 };
std::vector<SDL_Surface*> GUI::s_m_font_vector_SURFACE;

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





void GUI::drawBackGroundColor(int r, int g, int b)
{
	SDL_SetRenderDrawColor(m_p_render_instance->getRenderer(), r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(m_p_render_instance->getRenderer(), &m_background_rect);
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
	if (SDL_RenderCopy(m_p_render_instance->getRenderer(), m_preloaded_textures_collection.at(name), NULL, &draw_rect) < 0)
	{
		m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " SDL_RenderCopy" + " | Object: " + name, SDL_GetError());
	}
}

void GUI::drawPreloadedTexture(int x_pos, int y_pos, int height, int width, int rotation, std::string name)
{
	SDL_Rect draw_rect;
	draw_rect.x = x_pos;
	draw_rect.y = y_pos;
	draw_rect.h = height;
	draw_rect.w = width;
	if (SDL_RenderCopyEx(m_p_render_instance->getRenderer(), m_preloaded_textures_collection.at(name), NULL, &draw_rect, rotation, NULL, SDL_FLIP_NONE) < 0)
	{
		m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " SDL_RenderCopyEx" + " | Object: " + name, SDL_GetError());
	}
}

void GUI::drawPreloadedTextureXYWH(std::string object_name, std::string texture_name)
{
	
	drawPreloadedTexture(m_loaded_elements_pos.at(object_name).xpos, m_loaded_elements_pos.at(object_name).ypos, m_loaded_elements_pos.at(object_name).width,
		m_loaded_elements_pos.at(object_name).height, texture_name);
}

void GUI::drawPreloadedTextureXYWH(std::string object_name, int rotation, std::string texture_name)
{
	drawPreloadedTexture(m_loaded_elements_pos.at(object_name).xpos, m_loaded_elements_pos.at(object_name).ypos, m_loaded_elements_pos.at(object_name).width,
		m_loaded_elements_pos.at(object_name).height, rotation, texture_name);
}

void GUI::drawPreloadedTextureXYWH(std::string object_name, int state)
{
	if (state == 0)
	{
		drawPreloadedTexture(m_loaded_elements_pos.at(object_name).xpos, m_loaded_elements_pos.at(object_name).ypos, m_loaded_elements_pos.at(object_name).width,
			m_loaded_elements_pos.at(object_name).height, "Schieber_Geschlossen");
	}
	if (state == 1)
	{
		drawPreloadedTexture(m_loaded_elements_pos.at(object_name).xpos, m_loaded_elements_pos.at(object_name).ypos, m_loaded_elements_pos.at(object_name).width,
			m_loaded_elements_pos.at(object_name).height, "Schieber_Offen");
	}
	if (state == 2)
	{
		drawPreloadedTexture(m_loaded_elements_pos.at(object_name).xpos, m_loaded_elements_pos.at(object_name).ypos, m_loaded_elements_pos.at(object_name).width,
			m_loaded_elements_pos.at(object_name).height, "Schieber_Unbekannt");
	}

}

void GUI::drawCursor()
{
	for (auto const& i : m_loaded_elements_pos)
	{
		if (i.second.selectable == true)
		{
			SDL_Rect cursor_rect;
			cursor_rect.x = i.second.xpos -1;
			cursor_rect.y = i.second.ypos -1;
			cursor_rect.w = i.second.width +2;
			cursor_rect.h = i.second.height +2;

			SDL_SetRenderDrawColor(m_p_render_instance->getRenderer(), 255, 0, 0, 255);
			SDL_RenderFillRect(m_p_render_instance->getRenderer(), &cursor_rect);
			SDL_RenderDrawRect(m_p_render_instance->getRenderer(),&cursor_rect);

		}
	}
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
	
	//std::cout << "Text height: " << rect_text.h << std::endl;

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

void GUI::prepareXYWHPosition(int xpos, int ypos, int width, int height, std::string name)
{
	m_element_pos.xpos = xpos;
	m_element_pos.ypos = ypos;
	m_element_pos.width = width;
	m_element_pos.height = height;

	m_loaded_elements_pos.insert({ name, m_element_pos});
}

void GUI::loadXYWHPosition(std::string path)
{
	std::string temp;
	std::string single_character;
	std::string object_name;

	enum struct VARIANTS{XPOS = 0, YPOS = 1, WDATA = 2, HDATA = 3, SELECTABLE = 4} variants;

	std::ifstream object_position_file(m_getProjectDirPath() + path);
	object_position_file.seekg(0, object_position_file.end);
	int length = object_position_file.tellg();
	object_position_file.seekg(0, object_position_file.beg);

	char* buffer = new char[length];

	m_p_logger->writeLog(LogLevel::INFO, m_log_origin + " LOAD_XYWH_DATA", "Reading: " + std::to_string(length) + " characters");

	object_position_file.read(buffer, length);

	object_position_file.close();

	//Unsafe, file could be corrupted!!
	for (int i = 0; i < length; i++)
	{
		single_character = buffer[i];

		if (single_character != "#" && single_character != "{" && single_character != "}" && single_character != "," && single_character != "\n" && single_character != "\r")
		{
			temp.append(single_character);
		}

		//Check if a variant code word had been read (xpos,ypos,wdata,hdata,sel). If yes clear string and prepare to read and store actual data in struct
		{
			if (temp == "xpos")
			{
				variants = VARIANTS::XPOS;
				temp.clear();
			}
			else if (temp == "ypos")
			{
				variants = VARIANTS::YPOS;
				temp.clear();
			}
			else if (temp == "wdata")
			{
				variants = VARIANTS::WDATA;
				temp.clear();
			}
			else if (temp == "hdata")
			{
				variants = VARIANTS::HDATA;
				temp.clear();
			}
			else if (temp == "sel")
			{
				variants = VARIANTS::SELECTABLE;
				temp.clear();
			}
		}

		if (single_character == "{")
		{
			object_name = temp;
			temp.clear();
		}
		if (single_character == ",")
		{
			switch (variants)
			{
			case VARIANTS::XPOS:
			{
				m_element_pos.xpos = std::stoi(temp);
				temp.clear();
			}break;
			case VARIANTS::YPOS:
			{
				m_element_pos.ypos = std::stoi(temp);
				temp.clear();
			}break;
			case VARIANTS::WDATA:
			{
				m_element_pos.width = std::stoi(temp);
				temp.clear();
			}break;
			case VARIANTS::HDATA: 
			{
				m_element_pos.height = std::stoi(temp);
				temp.clear();
			}break;
			default:
				break;
			}
		}
		if (single_character == "}")//The last number doesn't have a comma following, so it has to be read when the bracelet is found
		{
			m_element_pos.selectable = std::stoi(temp);
			temp.clear();
			m_loaded_elements_pos.insert({ object_name, m_element_pos });
		}
		

	}
}

void GUI::loadTexture(std::string name, std::string path)
{
	std::string temp_path = m_getProjectDirPath() + path;
	m_preloaded_textures_collection.insert({ name, IMG_LoadTexture(m_p_render_instance->getRenderer(), temp_path.c_str()) });
}

void GUI::loadFont(std::string name, std::string path, int r, int g, int b)
{


	//Load TTF_Font
	m_p_SDL_font = TTF_OpenFont((m_getProjectDirPath() + path).c_str(),15);
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

//void GUI::TESTDRAWTEXT()
//{
//	SDL_Surface* surface = TTF_RenderUTF8_Solid(m_p_SDL_font, "TEST", m_SDL_color_black);
//	SDL_Texture* Message = SDL_CreateTextureFromSurface(m_p_render_instance->getRenderer(), surface);
//	SDL_Rect Message_rect; //create a rect
//	Message_rect.x = 100;  //controls the rect's x coordinate 
//	Message_rect.y = 100; // controls the rect's y coordinte
//	Message_rect.w = 100; // controls the width of the rect
//	Message_rect.h = 100; // controls the height of the rect
//
//	SDL_RenderCopy(m_p_render_instance->getRenderer(), Message, NULL, &Message_rect);
//}

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

		s_m_font_vector_SURFACE.push_back(TTF_RenderUTF8_Solid(m_p_SDL_font, symbol_for_Render, s_m_SDL_font_color));


	}
		

	for (int i = 0; i < 95; i++)
	{
		char temp = i + '!' -1;

		std::cout << temp << std::endl;

		s_m_font_map_TEXTURE.insert({ temp, SDL_CreateTextureFromSurface(m_p_render_instance->getRenderer(), s_m_font_vector_SURFACE.at(i))});
	}

	s_m_font_collection.insert({ name, s_m_font_map_TEXTURE });

	
	
}



std::string GUI::m_getProjectDirPath()
{
	std::filesystem::path project_dir = std::filesystem::current_path().parent_path().parent_path().parent_path();
	return project_dir.string();
}

