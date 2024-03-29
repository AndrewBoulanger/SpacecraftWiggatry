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
	DEMA::QueueRect(temp1, { 255,0,0,255 });
	
	return SDL_HasIntersection(&temp1, &temp2);
}

bool CollisionManager::CircleCircleCheck(const SDL_FPoint object1, const SDL_FPoint object2, const double r1, const double r2)
{
//	DEMA::QueueLine({ (int)object1.x, (int)object1.y
	//	}, { (int)object2.x, (int)object2.y }, { 255,255,255,255 });
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

	//DEMA::QueueLine({ (int)object1.x, (int)object1.y
	//	}, { (int)object2.x, (int)object2.y }, { 255,255,255,255 });


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

bool CollisionManager::PlayerCollision(const SDL_FRect* player, const int dX, const int dY)
{
	int playerX = (player->x + SPMR::getOffset()) / 32;
	int playerY = player->y / 32;
	SDL_Rect p = { player->x + dX + 16, player->y + dY + 24, player->w *.5, player->h *.6 }; // Adjusted bounding box.
	
	playerX = std::min(std::max(playerX, 0), COLS-1);
	playerY = std::min(std::max(playerY, 0), ROWS - 1);

	Tile* tiles[12] = { Engine::GetLevel()[playerY][playerX],	// Bottom		players tile											// Left
					   Engine::GetLevel()[playerY][(playerX + 1 >= COLS ? COLS - 1 : playerX + 1)],										// MiddleLeft
					   Engine::GetLevel()[playerY][(playerX + 2 >= COLS ? COLS - 1 : playerX + 2)],									// MiddleRight
					 //  Engine::GetLevel()[playerY][(playerX + 1 == COLS ? COLS - 1 : playerX + 3) ],										// Right tile.
					// Second row
					   Engine::GetLevel()[(playerY + 1 >= ROWS ? ROWS - 1 : playerY + 1)][playerX],
					   Engine::GetLevel()[(playerY + 1 >= ROWS ? ROWS - 1 : playerY + 1)][(playerX + 1 >= COLS ? COLS - 1 : playerX + 1)],
					   Engine::GetLevel()[(playerY + 1 >= ROWS ? ROWS - 1 : playerY + 1)][(playerX + 2 >= COLS ? COLS - 1 : playerX + 2)],
					   //  Engine::GetLevel()[(playerY + 1 == ROWS ? ROWS - 1 : playerY + 1)][(playerX + 3 == COLS ? COLS - 1 : playerX + 3) ],
			   // Third row
				  Engine::GetLevel()[(playerY + 2 >= ROWS ? ROWS - 1 : playerY + 2)][playerX],
				  Engine::GetLevel()[(playerY + 2 >= ROWS ? ROWS - 1 : playerY + 2)][(playerX + 1 >= COLS ? COLS - 1 : playerX + 1)],
				  Engine::GetLevel()[(playerY + 2 >= ROWS ? ROWS - 1 : playerY + 2)][(playerX + 2 >= COLS ? COLS - 1 : playerX + 2)],
				  // Engine::GetLevel()[(playerY + 2 == ROWS ? ROWS - 1 : playerY + 2)][(playerX + 3 == COLS ? COLS - 1 : playerX + 3) ],
				// Fourth row hahahahahahahaha
				   Engine::GetLevel()[(playerY + 3 >= ROWS ? ROWS - 1 : playerY + 3)][playerX],
				   Engine::GetLevel()[(playerY + 3 >= ROWS ? ROWS - 1 : playerY + 3)][(playerX + 1 >= COLS ? COLS - 1 : playerX + 1)],
				   Engine::GetLevel()[(playerY + 3 >= ROWS ? ROWS - 1 : playerY + 3)][(playerX + 2 >= COLS ? COLS - 1 : playerX + 2)] };
		//   Engine::GetLevel()[(playerY + 3 == ROWS ? ROWS - 1 : playerY + 3)][(playerX + 3 == COLS ? COLS - 1 : playerX + 3) ] };

	DEMA::QueueRect(p, { 255,0,0,0 });
	for (int i = 0; i < 12; i++)
	{
		SDL_RenderClear(Engine::Instance().GetRenderer());
	
		SDL_Rect t = MAMA::ConvertFRect2Rect(*(tiles[i]->GetDstP()));
		DEMA::QueueRect(t, { 255,0,255,0 });
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

bool CollisionManager::PlayerHazardCollision(const SDL_FRect* player, const int dX, const int dY)
{
	int playerX = (player->x + SPMR::getOffset()) / 32;
	int playerY = player->y / 32;
	SDL_Rect p = { player->x + dX + 16, player->y + dY + 24, player->w - 48, player->h - 48 }; // Adjusted bounding box.
	playerX = std::min(std::max(playerX, 0), COLS - 1);
	playerY = std::min(std::max(playerY, 0), ROWS - 1);
	Tile* tiles[12] = { Engine::GetLevel()[playerY][playerX],	// Bottom		players tile											// Left
					   Engine::GetLevel()[playerY][(playerX + 1 >= COLS ? COLS - 1 : playerX + 1)],										// MiddleLeft
					   Engine::GetLevel()[playerY][(playerX + 2 >= COLS ? COLS - 1 : playerX + 2)],										// MiddleRight
					 //  Engine::GetLevel()[playerY][(playerX + 1 == COLS ? COLS - 1 : playerX + 3)],										// Right tile.
					// Second row
					   Engine::GetLevel()[(playerY + 1 >= ROWS ? ROWS - 1 : playerY + 1)][playerX],
					   Engine::GetLevel()[(playerY + 1 >= ROWS ? ROWS - 1 : playerY + 1)][(playerX + 1 >= COLS ? COLS - 1 : playerX + 1)],
					   Engine::GetLevel()[(playerY + 1 >= ROWS ? ROWS - 1 : playerY + 1)][(playerX + 2 == COLS ? COLS - 1 : playerX + 2)],
		//   Engine::GetLevel()[(playerY + 1 == ROWS ? ROWS - 1 : playerY + 1)][(playerX + 3 == COLS ? COLS - 1 : playerX + 3)],
// Third row
   Engine::GetLevel()[(playerY + 2 >= ROWS ? ROWS - 1 : playerY + 2)][playerX],
   Engine::GetLevel()[(playerY + 2 >= ROWS ? ROWS - 1 : playerY + 2)][(playerX + 1 >= COLS ? COLS - 1 : playerX + 1)],
   Engine::GetLevel()[(playerY + 2 >= ROWS ? ROWS - 1 : playerY + 2)][(playerX + 2 >= COLS ? COLS - 1 : playerX + 2)],
		//  Engine::GetLevel()[(playerY + 2 == ROWS ? ROWS - 1 : playerY + 2)][(playerX + 3 == COLS ? COLS - 1 : playerX + 3)],
	   // Fourth row hahahahahahahaha
		  Engine::GetLevel()[(playerY + 3 >= ROWS ? ROWS - 1 : playerY + 3)][playerX],
		  Engine::GetLevel()[(playerY + 3 >= ROWS ? ROWS - 1 : playerY + 3)][(playerX + 1 >= COLS ? COLS - 1 : playerX + 1)],
		  Engine::GetLevel()[(playerY + 3 >= ROWS ? ROWS - 1 : playerY + 3)][(playerX + 2 >= COLS ? COLS - 1 : playerX + 2)] };
		 //  Engine::GetLevel()[(playerY + 3 == ROWS ? ROWS - 1 : playerY + 3)][(playerX + 3 == COLS ? COLS - 1 : playerX + 3)] };
//	DEMA::QueueRect(p, { 255,0,0,0 });
	for (int i = 0; i < 12; i++)
	{
		SDL_RenderClear(Engine::Instance().GetRenderer());

		SDL_Rect t = MAMA::ConvertFRect2Rect(*(tiles[i]->GetDstP()));
	//	DEMA::QueueRect(t, { 0,255,255,0 });
		if (tiles[i]->IsHazard() && SDL_HasIntersection(&p, &t))
		{
			//std::cout << "Hazard!\n";
			return true;
		}
	}
	return false;
}


bool CollisionManager::SmallTileCollision(const SDL_FRect target, const int dX, const int dY)
{
	int targetX = (target.x + SPMR::getOffset()) / 32;
	int targetY = target.y / 32;
	SDL_Rect sprite = { target.x + dX, target.y + dY + 8, target.w*.5, target.h *.5 }; // Adjusted bounding box.
	targetX = std::min(std::max(targetX, 0), COLS - 1);
	targetY = std::min(std::max(targetY, 0), ROWS - 1); //stops it from checking outside of the bounds of the tiles
	Tile* tiles[4] = {
		Engine::GetLevel()[targetY][targetX ],	// Bottom		players tile											// Left
		Engine::GetLevel()[targetY][(targetX + 1 >= COLS ? COLS - 1 : targetX + 1)],										// MiddleLeft
		Engine::GetLevel()[(targetY + 1 >= ROWS ? ROWS - 1 : targetY + 1)][targetX ],
		Engine::GetLevel()[(targetY + 1 >= ROWS ? ROWS - 1 : targetY + 1)][(targetX + 1 >= COLS ? COLS - 1 : targetX + 1)],									// Right tile.
	};
	DEMA::QueueRect(sprite, { 255,0,0,0 });
	for (int i = 0; i < 4; i++)
	{
		SDL_Rect tile = MAMA::ConvertFRect2Rect(*(tiles[i]->GetDstP()));
		DEMA::QueueRect(tile, { 0, 255, 255,0 });
		if (tiles[i]->IsObstacle() && SDL_HasIntersection(&sprite, &tile))
		{ // Collision!
			//std::cout << "cool\n";
			return true;
		}
	}
	return false;
}

bool CollisionManager::PointRectCheck(const SDL_Point p, const SDL_Rect r)
{
	return SDL_PointInRect(&p, &r);
}
