#include "pch.h"

#include "WalkableTile.h"

#include "Utilities/ColorUtility.h"


WalkableTile::WalkableTile(float width, float height)
	: BaseTile(width, height)
{
	SetColor(glm::vec4{ ColorUtility::WHITE, 1.0f });
}
