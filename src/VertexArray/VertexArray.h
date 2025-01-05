#pragma once

#include "VertexBuffer/VertexBuffer.h"
#include "IndexBuffer/IndexBuffer.h"


struct AuxiliaryVertexBufferBoundInfo
{
	std::shared_ptr<VertexBuffer> BoundVertexBuffer;

	std::vector<uint8_t> TakenAttributes;

	uint8_t BindingIndex = -1;
};

class VertexArray
{
public:
	VertexArray();

	~VertexArray();

	static uint32_t GetMaxPossibleBindedVertexBuffers();

	static uint32_t GetMaxPossibleBindedVertexAttributes();

	void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer);
	void RemoveVertexBuffer(uint32_t id);

	bool IsVertexBufferPresent(uint32_t VertexBufferId) const;

	bool AreFreeAttributesAvailable(const std::shared_ptr<VertexBuffer>& vertexBuffer) const;

	std::unordered_map<uint32_t, AuxiliaryVertexBufferBoundInfo>::const_iterator GetBindedVertexBuffer(uint32_t VertexBufferId) const;

	void RemoveAllVertexBuffers();

	uint8_t PopFreeBindingIndex();

	uint8_t PopFreeAttributeIndex();

	void Bind();
	void Unbind();

	void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

	uint64_t GetIndexBufferSize() const;

protected:

	// VBO id to data mapping
	std::unordered_map<uint32_t, AuxiliaryVertexBufferBoundInfo> m_BoundVertexBufferMap;

	std::vector<uint8_t> m_FreeBindingIndices;

	std::vector<uint8_t> m_FreeAttributeIndices;

	std::shared_ptr<IndexBuffer> m_IndexBuffer;

	uint32_t m_Id = 0;
};
