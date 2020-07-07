#include "CollisionManager.h"
#include "DebugManager.h"
#include "MathManager.h"
#include "StateManager.h"
#include "States.h"
#include "Sprite.h"
#include "SpriteManager.h"


bool CollisionManager::AABBCheck(const SDL_FRect& object1, const SDL_FRect& object2)
{
	SDL_Rect temp1 = MAMA::ConvertFRect2Rect(object1);
	SDL_Rect temp2 = MAMA::ConvertFRect2Rect(object2);
	return SDL_HasIntersection(&temp1, &temp2);
}

bool CollisionManager::CircleCircleCheck(const SDL_FPoint object1, const SDL_FPoint object2, const double r1, const double r2)
{
	return (MAMA::Distance((double)object1.x, (double)object2.x, (double)object1.y, (double)object2.y) < (r1 + r2));
}

bool CollisionManager::CircleAABBCheck(const SDL_FPoint object1, const double r, const SDL_FRect& object2)
{
	double x1 = (double)object1.x;
	double y1 = (double)object1.y;
	if (object1.x < object2.x) // Circle center, rect left edge.
		x1 = (double)object2.x;
	else if (object1.x > object2.x + object2.w)
		x1 = (double)object2.x + (double)object2.w;
	if (object1.y < object2.y)
		y1 = (double)object2.y;
	else if (object1.y > object2.y + object2.h)
		y1 = (double)object2.y + (double)object2.h;

	return CircleCircleCheck({ (float)x1, (float)y1 }, { (float)object1.x, (float)object1.y }, r);
}

bool CollisionManager::LinePointCheck(const SDL_FPoint object1_start, const SDL_FPoint object1_end, const SDL_FPoint object2)
{
	double distToStart = MAMA::Distance((double)object1_start.x, (double)object2.x, (double)object1_start.y, (double)object2.y);
	double distToEnd = MAMA::Distance((double)object1_end.x, (double)object2.x, (double)object1_end.y, (double)object2.y);

	double lineLength = MAMA::Distance((double)object1_start.x, (double)object1_end.x, (double)object1_start.y, (double)object1_end.y);

	double buffer = 0.2; // Extra distance since line thickness is one pixel.

	if (distToStart + distToEnd <= lineLength + buffer)
		return true;
	return false;
}

bool CollisionManager::PlayerCollision(const SDL_FRect* player, const int dX, const int dY, float offset)
{
	int playerX = (player->x + offset) / 32;
	int playerY = player->y / 32;
	SDL_Rect p = { player->x + dX + 16, player->y + dY + 24, player->w - 48, player->h - 48 }; // Adjusted bounding box.
	Tile* tiles[16] = { Engine::GetLevel()[playerY][playerX ],	// Bottom		players tile											// Left
					   Engine::GetLevel()[playerY][(playerX + 1 >= COLS ? COLS - 1 : playerX + 1)],										// MiddleLeft
					   Engine::GetLevel()[playerY][(playerX + 1 >= COLS ? COLS - 1 : playerX + 2) ],										// MiddleRight
					   Engine::GetLevel()[playerY][(playerX + 1 == COLS ? COLS - 1 : playerX + 3) ],										// Right tile.
					// Second row
					   Engine::GetLevel()[(playerY + 1 >= ROWS ? ROWS - 1 : playerY + 1)][playerX ],
					   Engine::GetLevel()[(playerY + 1 == ROWS ? ROWS - 1 : playerY + 1)][(playerX + 1 >= COLS ? COLS - 1 : playerX + 1) ],
					   Engine::GetLevel()[(playerY + 1 == ROWS ? ROWS - 1 : playerY + 1)][(playerX + 2 == COLS ? COLS - 1 : playerX + 2) ],
					   Engine::GetLevel()[(playerY + 1 == ROWS ? ROWS - 1 : playerY + 1)][(playerX + 3 == COLS ? COLS - 1 : playerX + 3) ],
		// Third row
		   Engine::GetLevel()[(playerY + 2 == ROWS ? ROWS - 1 : playerY + 2)][playerX ],
		   Engine::GetLevel()[(playerY + 2 == ROWS ? ROWS - 1 : playerY + 2)][(playerX + 1 == COLS ? COLS - 1 : playerX + 1) ],
		   Engine::GetLevel()[(playerY + 2 == ROWS ? ROWS - 1 : playerY + 2)][(playerX + 2 == COLS ? COLS - 1 : playerX + 2) ],
		   Engine::GetLevel()[(playerY + 2 == ROWS ? ROWS - 1 : playerY + 2)][(playerX + 3 == COLS ? COLS - 1 : playerX + 3) ],
		// Fourth row hahahahahahahaha
		   Engine::GetLevel()[(playerY + 3 == ROWS ? ROWS - 1 : playerY + 3)][playerX ],
		   Engine::GetLevel()[(playerY + 3 == ROWS ? ROWS - 1 : playerY + 3)][(playerX + 1 == COLS ? COLS - 1 : playerX + 1) ],
		   Engine::GetLevel()[(playerY + 3 == ROWS ? ROWS - 1 : playerY + 3)][(playerX + 2 == COLS ? COLS - 1 : playerX + 2) ],
		   Engine::GetLevel()[(playerY + 3 == ROWS ? ROWS - 1 : playerY + 3)][(playerX + 3 == COLS ? COLS - 1 : playerX + 3) ] };

	for (int i = 0; i < 16; i++)
	{
		SDL_RenderClear(Engine::Instance().GetRenderer());
	
		DEMA::DrawLine({ (int)tiles[i]->GetDstP()->x,(int)tiles[i]->GetDstP()->y }, { (int)tiles[i]->GetDstP()->x + (int)tiles[i]->GetDstP()->w,(int)tiles[i]->GetDstP()->y }, { 255,0,0,0 });
		DEMA::DrawLine({ (int)tiles[i]->GetDstP()->x,(int)tiles[i]->GetDstP()->y }, { (int)tiles[i]->GetDstP()->x ,(int)tiles[i]->GetDstP()->y + (int)tiles[i]->GetDstP()->h }, { 255,0,0,0 });
		SDL_Rect t = MAMA::ConvertFRect2Rect(*(tiles[i]->GetDstP()));
		if (tiles[i]->IsObstacle() && SDL_HasIntersection(&p, &t))
		{
			//std::cout << "Collision!\n";
			return true;
		}
		//if (tiles[i]->IsHazard() && SDL_HasIntersection(&p, &t))
		//{
		//	//std::cout << "Hazard!\n";
		//	return true;
		//}
	}
	return false;
}

bool CollisionManager::PlayerHazardCollision(const SDL_FRect* player, const int dX, const int dY, float offset)
{
	int playerX = (player->x + offset) / 32;
	int playerY = player->y / 32;
	SDL_Rect p = { player->x + dX + 16, player->y + dY + 24, player->w - 48, player->h - 48 }; // Adjusted bounding box.
	Tile* tiles[16] = { Engine::GetLevel()[playerY][playerX],	// Bottom		players tile											// Left
					   Engine::GetLevel()[playerY][(playerX + 1 >= COLS ? COLS - 1 : playerX + 1)],										// MiddleLeft
					   Engine::GetLevel()[playerY][(playerX + 1 >= COLS ? COLS - 1 : playerX + 2)],										// MiddleRight
					   Engine::GetLevel()[playerY][(playerX + 1 == COLS ? COLS - 1 : playerX + 3)],										// Right tile.
					// Second row
					   Engine::GetLevel()[(playerY + 1 >= ROWS ? ROWS - 1 : playerY + 1)][playerX],
					   Engine::GetLevel()[(playerY + 1 == ROWS ? ROWS - 1 : playerY + 1)][(playerX + 1 >= COLS ? COLS - 1 : playerX + 1)],
					   Engine::GetLevel()[(playerY + 1 == ROWS ? ROWS - 1 : playerY + 1)][(playerX + 2 == COLS ? COLS - 1 : playerX + 2)],
					   Engine::GetLevel()[(playerY + 1 == ROWS ? ROWS - 1 : playerY + 1)][(playerX + 3 == COLS ? COLS - 1 : playerX + 3)],
		// Third row
		   Engine::GetLevel()[(playerY + 2 == ROWS ? ROWS - 1 : playerY + 2)][playerX],
		   Engine::GetLevel()[(playerY + 2 == ROWS ? ROWS - 1 : playerY + 2)][(playerX + 1 == COLS ? COLS - 1 : playerX + 1)],
		   Engine::GetLevel()[(playerY + 2 == ROWS ? ROWS - 1 : playerY + 2)][(playerX + 2 == COLS ? COLS - 1 : playerX + 2)],
		   Engine::GetLevel()[(playerY + 2 == ROWS ? ROWS - 1 : playerY + 2)][(playerX + 3 == COLS ? COLS - 1 : playerX + 3)],
		// Fourth row hahahahahahahaha
		   Engine::GetLevel()[(playerY + 3 == ROWS ? ROWS - 1 : playerY + 3)][playerX],
		   Engine::GetLevel()[(playerY + 3 == ROWS ? ROWS - 1 : playerY + 3)][(playerX + 1 == COLS ? COLS - 1 : playerX + 1)],
		   Engine::GetLevel()[(playerY + 3 == ROWS ? ROWS - 1 : playerY + 3)][(playerX + 2 == COLS ? COLS - 1 : playerX + 2)],
		   Engine::GetLevel()[(playerY + 3 == ROWS ? ROWS - 1 : playerY + 3)][(playerX + 3 == COLS ? COLS - 1 : playerX + 3)] };

	for (int i = 0; i < 16; i++)
	{
		SDL_RenderClear(Engine::Instance().GetRenderer());

		DEMA::DrawLine({ (int)tiles[i]->GetDstP()->x,(int)tiles[i]->GetDstP()->y }, { (int)tiles[i]->GetDstP()->x + (int)tiles[i]->GetDstP()->w,(int)tiles[i]->GetDstP()->y }, { 255,0,0,0 });
		DEMA::DrawLine({ (int)tiles[i]->GetDstP()->x,(int)tiles[i]->GetDstP()->y }, { (int)tiles[i]->GetDstP()->x ,(int)tiles[i]->GetDstP()->y + (int)tiles[i]->GetDstP()->h }, { 255,0,0,0 });
		SDL_Rect t = MAMA::ConvertFRect2Rect(*(tiles[i]->GetDstP()));
		if (tiles[i]->IsHazard() && SDL_HasIntersection(&p, &t))
		{
			//std::cout << "Hazard!\n";
			return true;
		}
	}
	return false;
}


bool CollisionManager::SmallTileCollision(const SDL_FRect target, const int dX, const int dY, float offset)
{
	int targetX = (target.x + offset) / 32;
	int targetY = target.y / 32;
	SDL_Rect sprite = { target.x + dX + 6, target.y + dY + 8, target.w - 16, target.h - 16 }; // Adjusted bounding box.
	Tile* tiles[4] = {
		Engine::GetLevel()[targetY][targetX ],	// Bottom		players tile											// Left
		Engine::GetLevel()[targetY][(targetX + 1 >= COLS ? COLS - 1 : targetX + 1)],										// MiddleLeft
		Engine::GetLevel()[(targetY + 1 >= ROWS ? ROWS - 1 : targetY + 1)][targetX ],
		Engine::GetLevel()[(targetY + 1 == ROWS ? ROWS - 1 : targetY + 1)][(targetX + 1 >= COLS ? COLS - 1 : targetX + 1)],									// Right tile.
	};
	for (int i = 0; i < 4; i++)
	{
		SDL_Rect tile = MAMA::ConvertFRect2Rect(*(tiles[i]->GetDstP()));
		if (tiles[i]->IsObstacle() && SDL_HasIntersection(&sprite, &tile))
		{ // Collision!
			//std::cout << "cool\n";
			return true;
		}
	}
	return false;
}