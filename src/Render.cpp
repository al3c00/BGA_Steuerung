#include "Render.h"




Render::Render(Log* logger, SDL_Window* window)
{
	m_p_logger = logger;
	m_log_origin = "RENDERER";

	m_SDLrenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);
	if (!m_SDLrenderer)
	{
		m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " | SDL_CREATE_RENDERER", SDL_GetError());
	}
	else
	{
		
		if (SDL_GetRendererInfo(m_SDLrenderer, &m_SDLrendererinfo) < 0)
		{
			m_p_logger->writeLog(LogLevel::ERROR, m_log_origin + " | SDL_GET_RENDERER_INFO", SDL_GetError());
		}
		else
		{
			const char* renderer_name = m_SDLrendererinfo.name;
			m_p_logger->writeLog(LogLevel::INFO, m_log_origin + " | SDL_GET_RENDERER_INFO", "RenderContext: " + std::string(renderer_name));
		}
	}

	SDL_SetRenderDrawBlendMode(m_SDLrenderer, SDL_BLENDMODE_BLEND);

	std::cout << "Created Renderer" << m_SDLrenderer << std::endl;

	full_display_rect.x = 0;
	full_display_rect.y = 0;
	full_display_rect.h = 800;
	full_display_rect.w = 480;
}

Render::~Render()
{
	SDL_DestroyRenderer(m_SDLrenderer);
}


void Render::clearRender()
{
	SDL_RenderClear(m_SDLrenderer);
}

void Render::prepareRender()
{
	SDL_SetRenderDrawColor(m_SDLrenderer, 50, 50, 100, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(m_SDLrenderer, &full_display_rect);
}

void Render::Show()
{
	
	SDL_RenderPresent(m_SDLrenderer);

}

SDL_Rect Render::getDisplaySize()
{

	return full_display_rect;
}

SDL_Renderer* Render::getRenderer()
{
	return m_SDLrenderer;
}


std::string Render::getProjectDirPath()
{
	std::filesystem::path project_dir = std::filesystem::current_path().parent_path().parent_path().parent_path();
	return project_dir.string();
}
