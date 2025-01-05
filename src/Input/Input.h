#pragma once


class Input
{
public:
	static bool IsKeyPressed(int32_t keyCode);
	static bool IsKeyReleased(int32_t keyCode);

	static bool IsMouseButtonPressed(uint32_t mouseButton);
	static bool IsMouseButtonReleased(uint32_t mouseButton);

	static glm::vec2 GetMousePosition();
	static float GetMousePositionX();
	static float GetMousePositionY();
};