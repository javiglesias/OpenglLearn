#pragma once
#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	void Bind() const;
	void Unbind() const;
	void AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);
private:
	unsigned int m_RendererID;
};
