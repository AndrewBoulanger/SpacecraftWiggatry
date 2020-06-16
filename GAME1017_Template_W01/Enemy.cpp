#include "Enemy.h"

Enemy::Enemy(SDL_Rect s, SDL_FRect d, SDL_Renderer* r, SDL_Texture* t, int sstart, int smin, int smax, int nf, int hp, int dmg)
	:Character(s, d, r, t, sstart, smin, smax, nf, hp, dmg) 
{
	enemysWig = (new Wig({ 0,0,50,50 }, { d.x,d.y,40,40 }, Engine::Instance().GetRenderer(), TEMA::GetTexture("wig")));
	
	hasWig = true;
}

void Enemy::Update()
{
	if (hasWig)
	{
		enemysWig->SetPos({ (int)(m_dst.x + m_dst.w/1.5) , (int)(m_dst.y +20)});
	}
}

void Enemy::Render()
{
	Sprite::Render();
	if (hasWig)
	{
		enemysWig->RenderFlipped();
	}
}