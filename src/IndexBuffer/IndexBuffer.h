#pragma once

#include "glad/glad.h"


class IndexBuffer
{
public:
	IndexBuffer(uint32_t* indices, uint64_t size, uint32_t usage = GL_STATIC_DRAW);

	~IndexBuffer();

	uint32_t GetId() const;

	uint32_t* GetData() const;
	uint64_t GetDataSize() const;
	uint64_t GetDataSizeInBytes() const;

protected:
	uint32_t m_Id = 0;

	uint32_t* m_Data = nullptr;

	uint64_t m_DataSize = 0;
	uint64_t m_DataSizeInBytes = 0;
};