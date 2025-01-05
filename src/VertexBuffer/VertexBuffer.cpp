#include "pch.h"

#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(float* vertices, uint64_t size, uint32_t usage /* = GL_STATIC_DRAW */)
{
	m_Data = vertices;
	m_DataSize = size;
	m_DataSizeInBytes = size * sizeof(float);

	glCreateBuffers(1, &m_Id);
	glNamedBufferData(m_Id, m_DataSizeInBytes, m_Data, usage);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_Id);
}

uint32_t VertexBuffer::GetId() const
{
	return m_Id;
}

void VertexBuffer::SetLayout(const BufferLayout& layout)
{
	m_Layout = layout;
}

uint32_t VertexBuffer::GetDataOffset() const
{
	return 0;
}

uint32_t VertexBuffer::GetVertexStride() const
{
	const uint32_t vertexStride = m_Layout.GetStride();
	return vertexStride;
}

uint32_t VertexBuffer::GetLayoutAttributesNum() const
{
	return m_Layout.GetAttributesNum();
}

uint32_t VertexBuffer::GetNumberOfElementsInAttribute(uint32_t attributeIndex) const
{
	const uint32_t numOfElem = m_Layout.GetNumberOfElementsInAttribute(attributeIndex);
	return numOfElem;
}

uint32_t VertexBuffer::GetAttributeOffset(uint32_t attributeIndex) const
{
	const uint32_t attributeOffset = m_Layout.GetAttributeOffset(attributeIndex);
	return attributeOffset;
}

float* VertexBuffer::GetData() const
{
	return m_Data;
}

uint64_t VertexBuffer::GetDataSize() const
{
	return m_DataSize;
}

uint64_t VertexBuffer::GetDataSizeInBytes() const
{
	return m_DataSizeInBytes;
}
