#include "pch.h"

#include "Collision.h"


bool Collision::IsCollisionPresent(glm::vec2 min, glm::vec2 max, glm::vec2 testedLocation)
{
	const bool bIsInsideX = (testedLocation.x >= min.x && testedLocation.x <= max.x);
	const bool bIsInsideY = (testedLocation.y >= min.y && testedLocation.y <= max.y);
	return bIsInsideX && bIsInsideY;
}

