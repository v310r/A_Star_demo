#include "pch.h"

#include "Input.h"



bool Input::IsKeyPressed(int32_t keyCode)
{
	int numKeys = 0;
	const uint8_t* states = SDL_GetKeyboardState(&numKeys);
	return states[keyCode] == SDL_PRESSED;
}

bool Input::IsKeyReleased(int32_t keyCode)
{
	int numKeys = 0;
	const uint8_t* keyboardStates = SDL_GetKeyboardState(&numKeys);
	return keyboardStates[keyCode] == SDL_RELEASED;
}

bool Input::IsMouseButtonPressed(uint32_t mouseButton)
{
	int x = 0, y = 0;
	uint32_t mouseState = SDL_GetMouseState(&x, &y);
	return mouseState & SDL_BUTTON(mouseButton);
}

bool Input::IsMouseButtonReleased(uint32_t mouseButton)
{
	return IsMouseButtonPressed(mouseButton) == false;
}

glm::vec2 Input::GetMousePosition()
{
	int x = 0, y = 0;
	uint32_t mouseState = SDL_GetMouseState(&x, &y);
	return { x, y };
}

float Input::GetMousePositionX()
{
	return GetMousePosition().x;
}

float Input::GetMousePositionY()
{
	return GetMousePosition().y;
}
