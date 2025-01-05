#pragma once


class Collision
{
public:
	static bool IsCollisionPresent(glm::vec2 min, glm::vec2 max, glm::vec2 testedLocation);
};