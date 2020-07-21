#include "UIElement.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "MathManager.h"
#include <string>

UIElement::UIElement(SDL_Rect d, SDL_Renderer* r, SDL_Color c)
	:m_dst(d), m_pRend(r), m_color(c), m_state(STATE_UP) {}


UISlider::UISlider(SDL_Rect d, SDL_Renderer* r, SDL_Color col, SDL_Color colText, const char* name, int startVal, int minVal, int maxVal)
	: UIElement(d, r, col), m_val(startVal), m_minVal(minVal), m_maxVal(maxVal)
{
	m_handle = { (int)(d.x + 2 + ((d.w - d.h) * ((double)(m_val - m_minVal) / (double)(m_maxVal - m_minVal)))) , d.y + 2, d.h - 4, d.h - 4 };
	m_pName = new Label("fontSmall", 0, 0, name, colText);
	m_pName->SetPos(d.x - m_pName->GetRect().w - 8, d.y + d.h / 2 - m_pName->GetRect().h / 2);
	m_pVal = new Label("fontSmall", 0, 0, std::to_string(m_val).c_str(), colText);
	m_pVal->SetPos(m_handle.x + m_handle.w / 2 - m_pVal->GetRect().w / 2, d.y - m_pVal->GetRect().h - 4);
	m_pMin = new Label("fontSmall", 0, 0, std::to_string(m_minVal).c_str(), colText);
	m_pMin->SetPos(d.x + 2 + m_handle.w / 2 - m_pMin->GetRect().w / 2, (float)(d.y + d.h + 4));
	m_pMax = new Label("fontSmall", 0, 0, std::to_string(m_maxVal).c_str(), colText);
	m_pMax->SetPos((float)(d.x + d.w - m_handle.w / 2 - 2 - m_pMax->GetRect().w / 2), (float)(d.y + d.h + 4));
}

UISlider::~UISlider()
{
	// std::cout << "Destroying slider!" << std::endl;
	delete m_pName;
	delete m_pVal;
	delete m_pMin;
	delete m_pMax;
}

bool UISlider::Update()
{
	SDL_Rect temp = { m_handle.x - 6, m_handle.y - 6, m_handle.w + 12, m_handle.h + 12 };
	bool col = COMA::PointRectCheck(EVMA::GetMousePos(), temp);
	switch (m_state)
	{
	case STATE_UP:
		if (col)
		{
			EVMA::SetCursor(SDL_SYSTEM_CURSOR_SIZEWE);
			m_state = STATE_OVER;
		}
		break;
	case STATE_OVER:
		if (!col)
		{
			EVMA::SetCursor(SDL_SYSTEM_CURSOR_ARROW);
			m_state = STATE_UP;
		}
		else
		{
			if (EVMA::MouseHeld(1))
			{
				m_handle.x = MAMA::ClampI((EVMA::GetMousePos().x - m_handle.w / 2), (m_dst.x + 2), (m_dst.x + 2 + m_dst.w - m_dst.h));
				m_val = m_minVal + (int)((double)(m_maxVal - m_minVal) * ((double)m_handle.x - (m_dst.x + 2.0)) / ((m_dst.x + 2.0 + m_dst.w - m_dst.h) - (m_dst.x + 2.0)));
				m_pVal->SetText(std::to_string(m_val).c_str());
				m_pVal->SetPos(m_handle.x + m_handle.w / 2 - m_pVal->GetRect().w / 2, m_dst.y - m_pVal->GetRect().h - 4);
			}
			if (EVMA::MouseReleased(1))
			{
				SOMA::PlaySound("beep", 0, 6);
			}
		}
		break;
	}
	return 0;
}

void UISlider::Render()
{
	// Render labels first!
	m_pName->Render();
	m_pMin->Render();
	m_pMax->Render();
	m_pVal->Render();
	// Then geometry.
	SDL_SetRenderDrawColor(m_pRend, ClampRGB(m_color.r - 48), ClampRGB(m_color.g - 48), ClampRGB(m_color.b - 48), m_color.a);
	SDL_RenderFillRect(m_pRend, &m_dst);
	SDL_SetRenderDrawColor(m_pRend, ClampRGB(m_color.r - 64), ClampRGB(m_color.g - 64), ClampRGB(m_color.b - 64), m_color.a);
	SDL_RenderDrawRect(m_pRend, &m_dst);
	SDL_SetRenderDrawColor(m_pRend, ClampRGB(m_color.r - 96), ClampRGB(m_color.g - 96), ClampRGB(m_color.b - 96), m_color.a);
	SDL_RenderDrawLine(m_pRend, m_dst.x + m_dst.h / 2, m_dst.y + m_dst.h / 2, m_dst.x + m_dst.w - m_dst.h / 2, m_dst.y + m_dst.h / 2);
	SDL_SetRenderDrawColor(m_pRend, m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_RenderFillRect(m_pRend, &m_handle);
	SDL_SetRenderDrawColor(m_pRend, ClampRGB(m_color.r - 64), ClampRGB(m_color.g - 64), ClampRGB(m_color.b - 64), m_color.a);
	for (int i = -1; i <= 1; i++)
		SDL_RenderDrawLine(m_pRend, m_handle.x + m_handle.w / 2 - (i * m_handle.h / 6), m_handle.y + m_handle.h / 2 - m_handle.h / 6,
			m_handle.x + m_handle.w / 2 - (i * m_handle.h / 6), m_handle.y + m_handle.h / 2 + m_handle.h / 6);
	SDL_SetRenderDrawColor(m_pRend, ClampRGB(m_color.r + 48), ClampRGB(m_color.g + 48), ClampRGB(m_color.b + 48), m_color.a);
	SDL_RenderDrawLine(m_pRend, m_dst.x, m_dst.y, m_dst.x, m_dst.y + m_dst.h - 1);
	SDL_RenderDrawLine(m_pRend, m_dst.x, m_dst.y, m_dst.x + m_dst.w - 1, m_dst.y);
}

void UISlider::SetValue(int i)
{
	m_val = std::min(std::max(i, m_minVal), m_maxVal);
	m_handle.x = (int)(m_dst.x + 2 + ((double)(m_dst.w - m_dst.h) * ((double)(m_val - m_minVal) / (double)(m_maxVal - m_minVal))));
	m_pVal->SetText(std::to_string(m_val).c_str());
	m_pVal->SetPos(m_handle.x + m_handle.w / 2 - m_pVal->GetRect().w / 2, m_dst.y - m_pVal->GetRect().h - 4);
}
