#include "pch.h"

#include "BaseTile.h"


#include "VertexArray/VertexArray.h"
#include "Utilities/ColorUtility.h"


BaseTile::BaseTile(float width, float height)
{
	SetWidth(width);
	SetHeight(height);

	SetColor(glm::vec4{ColorUtility::WHITE, 1.0f});
}

BaseTile::~BaseTile()
{

}

void BaseTile::SetWidth(float inWidth)
{
	m_Width = inWidth;
}

float BaseTile::GetWidth() const
{
	return m_Width;
}

void BaseTile::SetHeight(float inHeight)
{
	m_Height = inHeight;
}

float BaseTile::GetHeight() const
{
	return m_Height;
}

void BaseTile::SetColor(glm::vec4 color)
{
	m_Color = color;
}

const glm::vec4& BaseTile::GetColor() const
{
	return m_Color;
}

glm::vec3 BaseTile::GetColorVec3() const
{
	return glm::vec3(m_Color.x, m_Color.y, m_Color.z);
}

void BaseTile::SetPosition(glm::vec3 position)
{
	m_Position = position;
}

const glm::vec3& BaseTile::GetPosition() const
{
	return m_Position;
}

glm::vec2 BaseTile::GetPositionVec2() const
{
	return glm::vec2{ m_Position.x, m_Position.y };
}

glm::vec2 BaseTile::GetMinBounds() const
{
	return glm::vec2(m_Position.x - (m_Width / 2.0f), m_Position.y - (m_Height / 2.0f));
}

glm::vec2 BaseTile::GetMaxBounds() const
{
	return glm::vec2(m_Position.x + (m_Width / 2.0f), m_Position.y + (m_Height / 2.0f));
}

void BaseTile::SetScale(glm::vec3 scale)
{
	m_Scale = scale;
}

const glm::vec3& BaseTile::GetScale() const
{
	return m_Scale;
}

void BaseTile::SetVAO(const std::shared_ptr<VertexArray>& inVAO)
{
	m_VAO = inVAO;
}

const std::shared_ptr<VertexArray>& BaseTile::GetVAO() const
{
	return m_VAO;
}

glm::mat4 BaseTile::GetComputedTransform() const
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, GetPosition());
	model = glm::scale(model, GetScale());
	
	return model;
}
