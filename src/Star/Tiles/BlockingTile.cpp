#include "pch.h"

#include "BlockingTile.h"

#include "Utilities/ColorUtility.h"


BlockingTile::BlockingTile(float width, float height)
	: BaseTile(width, height)
{
	SetColor(glm::vec4{ColorUtility::RED, 1.0f});
}
