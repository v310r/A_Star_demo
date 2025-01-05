#pragma once


class VertexArray;

class BaseTile
{
public:
	BaseTile(float width, float height);

	virtual ~BaseTile();

	void SetWidth(float inWidth);
	float GetWidth() const;

	void SetHeight(float inHeight);
	float GetHeight() const;

	void SetColor(glm::vec4 color);
	const glm::vec4& GetColor() const;

	glm::vec3 GetColorVec3() const;

	void SetPosition(glm::vec3 position);
	const glm::vec3& GetPosition() const;
	glm::vec2 GetPositionVec2() const;

	glm::vec2 GetMinBounds() const;
	glm::vec2 GetMaxBounds() const;

	void SetScale(glm::vec3 scale);
	const glm::vec3& GetScale() const;

	void SetVAO(const std::shared_ptr<VertexArray>& inVAO);
	const std::shared_ptr<VertexArray>& GetVAO() const;

	glm::mat4 GetComputedTransform() const;

protected:
	glm::vec3 m_Position;

	glm::vec3 m_Scale;

	glm::vec4 m_Color;

	float m_Width = 0;
	float m_Height = 0;

	std::shared_ptr<VertexArray> m_VAO;
};