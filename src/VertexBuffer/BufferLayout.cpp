#include "pch.h"

#include "BufferLayout.h"


static uint32_t ConvertShaderDataTypeToBytes(EShaderDataType shaderDataType)
{
	switch (shaderDataType)
	{
		case EShaderDataType::None:			return 0;
		case EShaderDataType::Int:			return sizeof(int);
		case EShaderDataType::VecInt2:		return sizeof(int) * 2;
		case EShaderDataType::VecInt3:		return sizeof(int) * 3;
		case EShaderDataType::VecInt4:		return sizeof(int) * 4;
		case EShaderDataType::Float:		return sizeof(float);
		case EShaderDataType::VecFloat2:	return sizeof(float) * 2;
		case EShaderDataType::VecFloat3:	return sizeof(float) * 3;
		case EShaderDataType::VecFloat4:	return sizeof(float) * 4;
		case EShaderDataType::Bool:			return sizeof(bool);
		case EShaderDataType::Matrix3:		return sizeof(float) * 9;
		case EShaderDataType::Matrix4:		return sizeof(float) * 16;
	}

	assert(false);
	return -1;
}

static uint32_t ConvertShaderDataTypeToNumberOfElements(EShaderDataType shaderDataType)
{
	switch (shaderDataType)
	{
		case EShaderDataType::None:			return 0;
		case EShaderDataType::Int:			return 1;
		case EShaderDataType::VecInt2:		return 2;
		case EShaderDataType::VecInt3:		return 3;
		case EShaderDataType::VecInt4:		return 4;
		case EShaderDataType::Float:		return 1;
		case EShaderDataType::VecFloat2:	return 2;
		case EShaderDataType::VecFloat3:	return 3;
		case EShaderDataType::VecFloat4:	return 4;
		case EShaderDataType::Bool:			return 1;
		case EShaderDataType::Matrix3:		return 9;
		case EShaderDataType::Matrix4:		return 16;
	}

	assert(false);
	return -1;
}

Attribute::Attribute(EShaderDataType type, const std::string& name, bool bDoNormalize /*= false*/)
	: Name(name), ShaderDataType(type), Size(ConvertShaderDataTypeToBytes(ShaderDataType)), bNormalized(bDoNormalize)
{

}

uint32_t Attribute::GetNumberOfElementsInAttribute(const Attribute& attribute)
{
	const uint32_t numOfElem = ConvertShaderDataTypeToNumberOfElements(attribute.ShaderDataType);
	return numOfElem;
}

BufferLayout::BufferLayout(const std::initializer_list<Attribute>& attributes)
	: m_Attributes(attributes)
{
	uint32_t offset = 0;
	for (Attribute& attribute : m_Attributes)
	{
		m_Stride += attribute.Size;

		attribute.Offset = offset;
		offset += attribute.Size;
	}
}

uint32_t BufferLayout::GetStride() const
{
	return m_Stride;
}

uint32_t BufferLayout::GetAttributesNum() const
{
	return m_Attributes.size();
}

uint32_t BufferLayout::GetNumberOfElementsInAttribute(uint32_t attributeIndex) const
{
	assert(attributeIndex >= 0 && attributeIndex < m_Attributes.size());

	const uint32_t numOfElem = Attribute::GetNumberOfElementsInAttribute(m_Attributes[attributeIndex]);
	return numOfElem;
}

uint32_t BufferLayout::GetAttributeOffset(uint32_t attributeIndex) const
{
	assert(attributeIndex >= 0 && attributeIndex < m_Attributes.size());

	const uint32_t attributeOffset = m_Attributes[attributeIndex].Offset;
	return attributeOffset;
}
