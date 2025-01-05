#include "pch.h"

#include "VertexArray.h"
#include "glad/glad.h"


VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_Id);

	const uint32_t maxPossibleBindedVertexBuffers = VertexArray::GetMaxPossibleBindedVertexBuffers();
	
	m_FreeBindingIndices.resize(maxPossibleBindedVertexBuffers);
	int vertexBindingValue = 0;

	std::generate(m_FreeBindingIndices.rbegin(), m_FreeBindingIndices.rend(), [&vertexBindingValue]()
	{
		return vertexBindingValue++;
	});

	const uint32_t maxPossibleBindedVertexAttributes = VertexArray::GetMaxPossibleBindedVertexAttributes();
	m_FreeAttributeIndices.resize(maxPossibleBindedVertexAttributes);

	int attributeValue = 0;
	std::generate(m_FreeAttributeIndices.rbegin(), m_FreeAttributeIndices.rend(), [&attributeValue]()
	{
		return attributeValue++;
	});
}

VertexArray::~VertexArray()
{
	m_FreeBindingIndices.clear();

	RemoveAllVertexBuffers();

	glDeleteVertexArrays(1, &m_Id);
}

uint32_t VertexArray::GetMaxPossibleBindedVertexBuffers()
{
	int32_t maxBindings;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIB_BINDINGS, &maxBindings);

	return maxBindings;
}

uint32_t VertexArray::GetMaxPossibleBindedVertexAttributes()
{
	int32_t maxAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);

	return maxAttributes;
}

void VertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	if (IsVertexBufferPresent(vertexBuffer->GetId()))
	{
		return;
	}

	if (!AreFreeAttributesAvailable(vertexBuffer))
	{
		return;
	}

	AuxiliaryVertexBufferBoundInfo auxiliaryVertexBufferInfo;

	const uint8_t freeBindingIndex = PopFreeBindingIndex();
	auxiliaryVertexBufferInfo.BindingIndex = freeBindingIndex;
	auxiliaryVertexBufferInfo.BoundVertexBuffer = vertexBuffer;

	std::shared_ptr<VertexBuffer>& cachedVertexBuffer = auxiliaryVertexBufferInfo.BoundVertexBuffer;

	glVertexArrayVertexBuffer(m_Id, freeBindingIndex, cachedVertexBuffer->GetId(), cachedVertexBuffer->GetDataOffset(), cachedVertexBuffer->GetVertexStride());

	for (uint32_t index = 0; index < cachedVertexBuffer->GetLayoutAttributesNum(); ++index)
	{
		const uint8_t freeAttributeIndex = PopFreeAttributeIndex();
		auxiliaryVertexBufferInfo.TakenAttributes.emplace_back(freeAttributeIndex);

		glVertexArrayAttribBinding(m_Id, freeAttributeIndex, freeBindingIndex);
		glVertexArrayAttribFormat(m_Id, freeAttributeIndex, cachedVertexBuffer->GetNumberOfElementsInAttribute(index), GL_FLOAT, GL_FALSE, cachedVertexBuffer->GetAttributeOffset(index));

		glEnableVertexArrayAttrib(m_Id, freeAttributeIndex);
	}

	m_BoundVertexBufferMap.emplace(cachedVertexBuffer->GetId(), std::move(auxiliaryVertexBufferInfo));
}

void VertexArray::RemoveVertexBuffer(uint32_t id)
{
	auto iter = GetBindedVertexBuffer(id);
	if (iter != m_BoundVertexBufferMap.end())
	{
		return;
	}

	const auto& [vertexBufferId, auxiliaryVertexBufferBoundInfo] = *iter;

	uint32_t emptyVBO = 0;
	glVertexArrayVertexBuffer(m_Id, auxiliaryVertexBufferBoundInfo.BindingIndex, emptyVBO, 0, 0);

	for (uint8_t takenAttributeIndex : auxiliaryVertexBufferBoundInfo.TakenAttributes)
	{
		m_FreeAttributeIndices.emplace_back(takenAttributeIndex);
	}

	m_FreeBindingIndices.emplace_back(auxiliaryVertexBufferBoundInfo.BindingIndex);

	m_BoundVertexBufferMap.erase(iter);
}

bool VertexArray::IsVertexBufferPresent(uint32_t VertexBufferId) const
{
	auto it = GetBindedVertexBuffer(VertexBufferId);
	const bool bIsVertexBufferPresent = (it != m_BoundVertexBufferMap.end());

	return bIsVertexBufferPresent;
}

bool VertexArray::AreFreeAttributesAvailable(const std::shared_ptr<VertexBuffer>& vertexBuffer) const
{
	const uint32_t requiredIncomingAmountOfAttributes = vertexBuffer->GetLayoutAttributesNum();
	return m_FreeAttributeIndices.size() >= requiredIncomingAmountOfAttributes;
}

std::unordered_map<uint32_t, AuxiliaryVertexBufferBoundInfo>::const_iterator VertexArray::GetBindedVertexBuffer(uint32_t VertexBufferId) const
{
	auto iter = m_BoundVertexBufferMap.find(VertexBufferId);
	if (iter == m_BoundVertexBufferMap.end())
	{
		return m_BoundVertexBufferMap.end();
	}

	return iter;
}

void VertexArray::RemoveAllVertexBuffers()
{
	for (auto& [vertexBufferId, auxiliaryVertexBufferBoundInfo] : m_BoundVertexBufferMap)
	{
		RemoveVertexBuffer(vertexBufferId);
	}
}

uint8_t VertexArray::PopFreeBindingIndex()
{
	const uint8_t freeBindingIndex = m_FreeBindingIndices.back();
	m_FreeBindingIndices.pop_back();
	return freeBindingIndex;
}

uint8_t VertexArray::PopFreeAttributeIndex()
{
	const uint8_t freeAttributeIndex = m_FreeAttributeIndices.back();
	m_FreeAttributeIndices.pop_back();
	return freeAttributeIndex;
}

void VertexArray::Bind()
{
	glBindVertexArray(m_Id);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	m_IndexBuffer = indexBuffer;
	glVertexArrayElementBuffer(m_Id, m_IndexBuffer->GetId());
}

uint64_t VertexArray::GetIndexBufferSize() const
{
	return m_IndexBuffer->GetDataSize();
}
