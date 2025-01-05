#pragma once

#include "BufferLayout.h"

#include "glad/glad.h"


class VertexBuffer
{
public:
	VertexBuffer(float* vertices, uint64_t size, uint32_t usage = GL_STATIC_DRAW);

	~VertexBuffer();

	uint32_t GetId() const;

	void SetLayout(const BufferLayout& layout);

	uint32_t GetDataOffset() const;

	uint32_t GetVertexStride() const;

	uint32_t GetLayoutAttributesNum() const;

	uint32_t GetNumberOfElementsInAttribute(uint32_t attributeIndex) const;

	// relative offset (relative to other attributes)
	uint32_t GetAttributeOffset(uint32_t attributeIndex) const;

	float* GetData() const;
	uint64_t GetDataSize() const;
	uint64_t GetDataSizeInBytes() const;

protected:
	BufferLayout m_Layout;

	float* m_Data = nullptr;
	uint64_t m_DataSize = 0;
	uint64_t m_DataSizeInBytes = 0;


	uint32_t m_Id = 0;
};
