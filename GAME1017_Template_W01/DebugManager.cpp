#include "DebugManager.h"
#include "Engine.h"

void DebugManager::DrawLine(const SDL_Point start, const SDL_Point end, const SDL_Color col)
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), col.r, col.g, col.b, col.a);
	SDL_RenderDrawLine(Engine::Instance().GetRenderer(), start.x, start.y, end.x, end.y);
}

void DebugManager::QueueLine(const SDL_Point start, const SDL_Point end, const SDL_Color col)
{
	if (s_debugMode)
	{
		s_points.push_back(start);
		s_points.push_back(end);
		s_colors.push_back(col);
	}
}

void DebugManager::QueueRect(const SDL_Rect r, const SDL_Color col)
{
	if (s_debugMode)
	{
		s_points.push_back({ r.x, r.y });// line 1 - left
		s_points.push_back({ r.x, r.y + r.h });

		s_points.push_back({ r.x, r.y });   // line 2 - top
		s_points.push_back({ r.x + +r.w, r.y });

		s_points.push_back({ r.x , r.y + r.h }); // bottom
		s_points.push_back( { r.x + r.w, r.y + r.h });

		s_points.push_back({ r.x +r.w , r.y  }); // right
		s_points.push_back({ r.x + r.w, r.y + r.h });

		for(int i = 0; i<4; i++) // push the colour 4 times
		s_colors.push_back(col);
	}
}

void DebugManager::FlushLines()
{
	for (unsigned i = 0, j = 0; i < s_points.size(); i+=2, j++)
		DrawLine(s_points[i], s_points[i+1], s_colors[j]);
	Quit(); // Clears out vectors.
}

void DebugManager::Quit()
{
	s_points.clear();
	s_colors.clear();
	s_points.shrink_to_fit();
	s_colors.shrink_to_fit();
}

bool DebugManager::s_debugMode = 0;
std::vector<SDL_Point> DebugManager::s_points;
std::vector<SDL_Color> DebugManager::s_colors;