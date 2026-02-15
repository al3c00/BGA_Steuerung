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

	m_cursor_pos = 0;
	m_cursor_active = false;

	m_scroll_position = 0;

}


void GUI::drawBackGroundColor(int r, int g, int b)
{
	SDL_SetRenderDrawColor(m_p_render_instance->getRenderer(), r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(m_p_render_instance->getRenderer(), &m_background_rect);
}

void GUI::prepareCursor()
{
	m_drawn_elements.clear();
	m_pos_selectable_objects.clear();
}



void GUI::drawVisualElement(int x_pos, int y_pos, int width, int height,  int r, int g, int b, bool is_selectable)
{
	SDL_Rect element_rect;
	element_rect.x = x_pos;
	element_rect.y = y_pos;
	element_rect.w = width;
	element_rect.h = height;

	SDL_SetRenderDrawColor(m_p_render_instance->getRenderer(), r, g, b,225);
	SDL_RenderFillRect(m_p_render_instance->getRenderer(), &element_rect);
	SDL_RenderDrawRect(m_p_render_instance->getRenderer(), &element_rect);



	m_drawn_elements.push_back({ "", x_pos, y_pos, width, height, is_selectable});
}


void GUI::drawVisualElement(int x_pos, int y_pos, int width, int height,  std::string name, bool is_selectable)
{
	SDL_Rect draw_rect{ 0 };
	draw_rect.x = x_pos;
	draw_rect.y = y_pos;
	draw_rect.h = height;
	draw_rect.w = width;
	if (SDL_RenderCopy(m_p_render_instance->getRenderer(), m_preloaded_textures_collection.at(name), NULL, &draw_rect) < 0)
	{
		m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " SDL_RenderCopy" + " | Object: " + name, SDL_GetError());
	}

	m_drawn_elements.push_back({ name, x_pos, y_pos, width, height, is_selectable });
}

void GUI::drawVisualElement(int x_pos, int y_pos, int width, int height,  int rotation, std::string name, bool is_selectable)
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
	m_drawn_elements.push_back({ name, x_pos, y_pos, width, height, is_selectable });

}

void GUI::drawVisualElement(int x_pos, int y_pos, int width, int height, int state, bool is_selectable)
{

}



void GUI::drawCursor()
{
	if (m_cursor_active)
	{

		

		for (int i = 0; i < m_drawn_elements.size(); i++)
		{
			if (m_drawn_elements.at(i).selectable)
			{
				m_pos_selectable_objects.push_back(i);
			}
		}

		if (!m_pos_selectable_objects.empty())
		{

			if (m_cursor_pos > m_pos_selectable_objects.size() - 1)
			{
				m_cursor_pos = m_pos_selectable_objects.size() - 1;
			}

			if (m_cursor_pos < 0)
			{
				m_cursor_pos = 0;
			}



			SDL_Rect cursor_rect;
			cursor_rect.x = m_drawn_elements.at(m_pos_selectable_objects.at(m_cursor_pos)).xpos - 1;
			cursor_rect.y = m_drawn_elements.at(m_pos_selectable_objects.at(m_cursor_pos)).ypos - 1;
			cursor_rect.w = m_drawn_elements.at(m_pos_selectable_objects.at(m_cursor_pos)).width + 2;
			cursor_rect.h = m_drawn_elements.at(m_pos_selectable_objects.at(m_cursor_pos)).height + 2;

			SDL_SetRenderDrawColor(m_p_render_instance->getRenderer(), 255, 0, 0, 150);
			SDL_RenderFillRect(m_p_render_instance->getRenderer(), &cursor_rect);
			SDL_RenderDrawRect(m_p_render_instance->getRenderer(), &cursor_rect);

			//std::cout << "Draw cursor on object nmbr: " << m_currently_selected_object_for_cursor << std::endl;

		}
		
	}

	
}

void GUI::switchCursorState()
{
	m_cursor_active = !m_cursor_active;
}

void GUI::setCursorState(bool cursor_state)
{
	m_cursor_active = cursor_state;
}

void GUI::moveCursor(int direction)
{
	if(direction)
	{
		m_cursor_pos += direction;
		//std::cout << "moved cursor: " << direction << " new position: " << m_currently_selected_object_for_cursor << std::endl;
	}

	

	
	
}

void GUI::setScrollPos(int scrollpos)
{
	m_scroll_position = 0;
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

void GUI::drawText_l(int x_pos, int y_pos, std::string text, std::string font, int box_r, int box_g, int box_b)
{
	int nmbr_of_characters = text.length();

	SDL_Rect rect_text;
	SDL_Rect rect_box;
	rect_text.x = x_pos;
	rect_text.y = y_pos;

	rect_box.x = x_pos - 2;
	rect_box.y = y_pos - 2;

	int symbol_width = 0;
	int symbol_height = 0;

	int text_length = 0;

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
		text_length += symbol_width;

		if (SDL_RenderCopy(m_p_render_instance->getRenderer(), s_m_font_collection.at(font).at(single_symbol), NULL, &rect_text) < 0)
		{
			std::cout << "Error SDL_RenderCopy" << std::endl;
			m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " DRAW-TEXT__SDL_RENDER_COPY", SDL_GetError());
		}


		rect_text.x += rect_text.w;
	}

		
		
		rect_box.w = text_length + 2;
		rect_box.h = rect_text.h + 2;

		SDL_SetRenderDrawColor(m_p_render_instance->getRenderer(), box_r, box_g, box_b, 100);
		SDL_RenderFillRect(m_p_render_instance->getRenderer(), &rect_box);
		SDL_RenderDrawRect(m_p_render_instance->getRenderer(), &rect_box);

		//std::cout << "Draw cursor on object nmbr: " << m_currently_selected_object_for_cursor << std::endl;

	

}

void GUI::drawText_l(int x_pos, int y_pos, int numbers, std::string font)
{
	drawText_l(x_pos, y_pos, std::to_string(numbers), font);
}

void GUI::drawList_l(int x_pos, int y_pos, int spacing,  std::vector<std::string> text, std::string font)
{
	//The text that should be displayed on one line
	std::string line_text;
	
	int vertical_text_pos = y_pos;//The y-position of the text line
	
	for (int i = 0; i < text.size(); i++)
	{
		line_text.clear();
		line_text = text.at(i);

		int nmbr_of_characters = line_text.length();

		SDL_Rect rect_text;
		rect_text.x = x_pos;
		rect_text.y = vertical_text_pos;

		int symbol_width = 0;
		int symbol_height = 0;

		for (int i = 0; i < nmbr_of_characters; i++)
		{
			char single_symbol = line_text.at(i);
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
		
		vertical_text_pos += spacing;
		
	}
}

void GUI::drawList_l(int x_pos, int y_pos, int spacing, int width, int height, int scrollpos,  std::vector<std::string> text, std::string font)
{
	//The text that should be displayed on one line
	std::string line_text;

	int vertical_text_pos = y_pos + 5;//The y-position of the text line. Lower the first line by 5 pixels to create distance

	SDL_Rect rect_box;
	rect_box.x = x_pos;
	rect_box.y = y_pos;
	rect_box.w = width;
	rect_box.h = height;

	m_scroll_position += scrollpos;

	if (m_scroll_position < 0)
	{
		m_scroll_position = 0;
	}

	if (m_scroll_position > text.size() - 1)
	{
		m_scroll_position = text.size() - 1;
	}

	SDL_SetRenderDrawColor(m_p_render_instance->getRenderer(), 150, 150, 150, 100);
	SDL_RenderFillRect(m_p_render_instance->getRenderer(), &rect_box);
	SDL_RenderDrawRect(m_p_render_instance->getRenderer(), &rect_box);


	for (int i = 0 + m_scroll_position; i < text.size(); i++)
	{
		line_text.clear();
		line_text = std::to_string(i) + ")" + text.at(i);

		int nmbr_of_characters = line_text.length();

		SDL_Rect rect_text;
		
		rect_text.x = x_pos;
		rect_text.y = vertical_text_pos;

		int symbol_width = 0;
		int symbol_height = 0;

		for (int i = 0; i < nmbr_of_characters; i++)
		{
			char single_symbol = line_text.at(i);
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

		vertical_text_pos += spacing;

		if (vertical_text_pos > y_pos + height - 20)
		{
			drawText_l(x_pos + width / 2 - 20, vertical_text_pos, "==SCROLL==", "ARIAL_Black");
			break;
		}

		


	}

}

void GUI::drawList_l(int x_pos, int y_pos, int spacing, int width, int height, bool scroll_forward, bool scroll_backward, std::vector<std::string> text, std::string font, bool has_background)
{
	//The text that should be displayed on one line
	std::string line_text;

	int vertical_text_pos = y_pos + 5;//The y-position of the text line. Lower the first line by 5 pixels to create distance

	SDL_Rect rect_box;
	rect_box.x = x_pos;
	rect_box.y = y_pos;
	rect_box.w = width;
	rect_box.h = height;

	if (scroll_forward)
	{
		m_scroll_position++;
	}
	else if (scroll_backward)
	{
		m_scroll_position--;
	}
	

	if (m_scroll_position < 0)
	{
		m_scroll_position = 0;
	}

	if (m_scroll_position > text.size() - 1)
	{
		m_scroll_position = text.size() - 1;
	}

	if (has_background)
	{
		SDL_SetRenderDrawColor(m_p_render_instance->getRenderer(), 150, 150, 150, 100);
		SDL_RenderFillRect(m_p_render_instance->getRenderer(), &rect_box);
		SDL_RenderDrawRect(m_p_render_instance->getRenderer(), &rect_box);
	}

	for (int i = 0 + m_scroll_position; i < text.size(); i++)
	{
		line_text.clear();
		line_text = std::to_string(i) + ")" + text.at(i);

		int nmbr_of_characters = line_text.length();

		SDL_Rect rect_text;

		rect_text.x = x_pos;
		rect_text.y = vertical_text_pos;

		int symbol_width = 0;
		int symbol_height = 0;

		for (int i = 0; i < nmbr_of_characters; i++)
		{
			char single_symbol = line_text.at(i);
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

		vertical_text_pos += spacing;

		if (vertical_text_pos > y_pos + height - 20)
		{
			drawText_l(x_pos + width /2 - 20, vertical_text_pos, "==SCROLL==", "ARIAL_Black");
			break;
		}

		


	}
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

void GUI::drawText_r(int x_pos, int y_pos, std::string text, std::string font, int box_r, int box_g, int box_b)
{
	int nmbr_of_characters = text.length();

	SDL_Rect rect_box;
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


	//First, the Top_Left position of the rectangle has to be calculated
	rect_box.x = rect_text.x - 2;
	rect_box.y = y_pos - 2;

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
		total_width += symbol_width;

		if (SDL_RenderCopy(m_p_render_instance->getRenderer(), s_m_font_collection.at(font).at(single_symbol), NULL, &rect_text) < 0)
		{
			std::cout << "Error SDL_RenderCopy" << std::endl;
			m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " DRAW-TEXT__SDL_RENDER_COPY", SDL_GetError());
		}


		rect_text.x += rect_text.w;
	}

	rect_box.w = total_width + 2;
	rect_box.h = rect_text.h + 2;

	SDL_SetRenderDrawColor(m_p_render_instance->getRenderer(), box_r, box_g, box_b, 100);
	SDL_RenderFillRect(m_p_render_instance->getRenderer(), &rect_box);
	SDL_RenderDrawRect(m_p_render_instance->getRenderer(), &rect_box);

	//std::cout << "Draw cursor on object nmbr: " << m_currently_selected_object_for_cursor << std::endl;

}



void GUI::loadTexture(std::string name, std::string path)
{
	std::string temp_path = m_getProjectDirPath() + path;

	SDL_Texture* temp = IMG_LoadTexture(m_p_render_instance->getRenderer(), temp_path.c_str());
	if (!temp)
	{
		m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " IMG_LoadTexture", SDL_GetError());
	}

	m_preloaded_textures_collection.insert({ name, temp});
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
	//Font map including control characters (code 0-31) and printable characters (code 32-127)
	for (int i = 0; i < 127; i++)
	{
		char temp = i;// +'!' - 1
		char buf[2] = { temp, '\0' };//Add nulltermination!!
		const char* symbol_for_Render = buf;
		//std::cout << symbol_for_Render << std::endl;

		s_m_font_vector_SURFACE.push_back(TTF_RenderUTF8_Solid(m_p_SDL_font, symbol_for_Render, s_m_SDL_font_color));


	}
		

	for (int i = 0; i < 127; i++)
	{
		char temp = i; // + '!' -1

		//std::cout << temp << std::endl;

		s_m_font_map_TEXTURE.insert({ temp, SDL_CreateTextureFromSurface(m_p_render_instance->getRenderer(), s_m_font_vector_SURFACE.at(i))});
	}

	s_m_font_collection.insert({ name, s_m_font_map_TEXTURE });

	
	
}



std::string GUI::m_getProjectDirPath()
{
	std::filesystem::path project_dir = std::filesystem::current_path().parent_path().parent_path().parent_path();
	return project_dir.string();
}

