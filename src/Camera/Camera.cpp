#include "pch.h"

#include "Camera.h"
#include "Input/Input.h"


Camera::Camera(float widthView, float heightView)
	: m_OrthoWidth(widthView), m_OrthoHeight(heightView)
{
	m_OrthographicProjectionMatrix = glm::ortho(0.0f, m_OrthoWidth, m_OrthoHeight, 0.0f, -1.0f, 1.0f);

	RecalculateViewMatrix();
}

void Camera::Update(float deltaTime)
{
	if (Input::IsKeyPressed(SDL_SCANCODE_W))
	{
		AddPositionOffset(glm::vec3{ 0.0f, -1.0f, 0.0f }, deltaTime);
	}

	if (Input::IsKeyPressed(SDL_SCANCODE_S))
	{
		AddPositionOffset(glm::vec3{ 0.0f, 1.0f, 0.0f }, deltaTime);
	}

	if (Input::IsKeyPressed(SDL_SCANCODE_A))
	{
		AddPositionOffset(glm::vec3{ -1.0f, 0.0f, 0.0f }, deltaTime);
	}

	if (Input::IsKeyPressed(SDL_SCANCODE_D))
	{
		AddPositionOffset(glm::vec3{ 1.0f, 0.0f, 0.0f }, deltaTime);
	}

	RecalculateViewMatrix();
}

void Camera::ImGuiFrameUpdate()
{
	if (ImGui::CollapsingHeader("Camera Settings", ImGuiTreeNodeFlags_None))
	{
		ImGui::InputFloat("Camera Speed", &m_Speed);
		ImGui::InputFloat("Camera Position X", &m_Position.x);
		ImGui::InputFloat("Camera Position Y", &m_Position.y);
	}
}

const glm::mat4& Camera::GetOrthographicProjectionMatrix() const
{
	return m_OrthographicProjectionMatrix;
}

void Camera::AddPositionOffset(glm::vec3 offset, float deltaTime)
{
	m_Position += offset * m_Speed * deltaTime;
}

void Camera::SetMovementSpeed(float speed)
{
	m_Speed = speed;
}

glm::vec2 Camera::GetPositionVec2() const
{
	return glm::vec2{ m_Position.x, m_Position.y };
}

float Camera::GetOrthoWidth() const
{
	return m_OrthoWidth;
}

float Camera::GetOrthoHeight() const
{
	return m_OrthoHeight;
}

void Camera::RecalculateViewMatrix()
{
	m_ViewMatrix = glm::mat4(1.0f);
	m_ViewMatrix = glm::translate(m_ViewMatrix, -m_Position);
}

const glm::mat4& Camera::GetViewMatrix() const
{
	return m_ViewMatrix;
}
