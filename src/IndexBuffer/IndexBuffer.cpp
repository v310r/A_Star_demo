#include "pch.h"

#include "IndexBuffer.h"


IndexBuffer::IndexBuffer(uint32_t* indices, uint64_t size, uint32_t usage /*= GL_STATIC_DRAW*/)
{
	m_Data = indices;
	m_DataSize = size;
	m_DataSizeInBytes = size * sizeof(uint32_t);

	glCreateBuffers(1, &m_Id);
	glNamedBufferData(m_Id, m_DataSizeInBytes, m_Data, usage);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_Id);
}

uint32_t IndexBuffer::GetId() const
{
	return m_Id;
}

uint32_t* IndexBuffer::GetData() const
{
	return m_Data;
}

uint64_t IndexBuffer::GetDataSize() const
{
	return m_DataSize;
}

uint64_t IndexBuffer::GetDataSizeInBytes() const
{
	return m_DataSizeInBytes;
}

