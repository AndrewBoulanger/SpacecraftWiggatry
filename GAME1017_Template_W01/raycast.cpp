#include "raycast.h"
#include "CollisionManager.h"
#include "SpriteManager.h"

void raycast::Update()
{
	if (!COMA::SmallTileCollision({ (float)m_pos.x, (float)m_pos.y, 32,32 }, dX, dY))
	{
		m_pos.x += (int)dX;
		m_pos.y += (int)dY;
	}
	accelX = accelY = 0;
}

void raycast::move2stop(double angle)
{
	accelX = accelY = 20;
	MAMA::SetDeltas(MAMA::Deg2Rad(angle), dX, dY, accelX, accelY);
}
