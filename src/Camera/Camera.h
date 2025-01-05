#pragma once


class Camera
{
public:
	Camera(float widthView, float heightView);

	void Update(float deltaTime);

	void ImGuiFrameUpdate();

	const glm::mat4& GetViewMatrix() const;

	const glm::mat4& GetOrthographicProjectionMatrix() const;

	void AddPositionOffset(glm::vec3 offset, float deltaTime);

	void SetMovementSpeed(float speed);

	glm::vec2 GetPositionVec2() const;

	float GetOrthoWidth() const;
	float GetOrthoHeight() const;

protected:
	void RecalculateViewMatrix();

	glm::vec3 m_Position = glm::vec3{ 0.0f, 0.0f, 0.0f };

	glm::mat4 m_ViewMatrix = glm::mat4(1.0f);

	glm::mat4 m_OrthographicProjectionMatrix = glm::mat4(1.0f);

	float m_OrthoWidth = 0, m_OrthoHeight = 0;

	float m_Speed = 500.0f;
};