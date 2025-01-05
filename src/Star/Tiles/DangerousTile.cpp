#include "pch.h"

#include "DangerousTile.h"

#include "Utilities/ColorUtility.h"


DangerousTile::DangerousTile(float width, float height)
	: WalkableTile(width, height)
{
	SetColor(glm::vec4{ ColorUtility::YELLOW, 1.0f });
}
