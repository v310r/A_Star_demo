#pragma once


enum class EShaderDataType
{
	None = 0,
	Int,
	VecInt2,
	VecInt3,
	VecInt4,
	Float,
	VecFloat2,
	VecFloat3,
	VecFloat4,
	Bool,
	Matrix3,
	Matrix4
};

struct Attribute
{
	Attribute(EShaderDataType type, const std::string& name, bool bDoNormalize = false);

	std::string Name;
	EShaderDataType ShaderDataType = EShaderDataType::None;
	uint32_t Size = 0;
	uint32_t Offset = 0;
	bool bNormalized = false;

	static uint32_t GetNumberOfElementsInAttribute(const Attribute& attribute);
};

class BufferLayout
{
public:
	BufferLayout(const std::initializer_list<Attribute>& attributes);

	BufferLayout() = default;

	uint32_t GetStride() const;

	uint32_t GetAttributesNum() const;

	uint32_t GetNumberOfElementsInAttribute(uint32_t attributeIndex) const;

	uint32_t GetAttributeOffset(uint32_t attributeIndex) const;

protected:
	std::vector<Attribute> m_Attributes;

	uint32_t m_Stride = 0;
};
