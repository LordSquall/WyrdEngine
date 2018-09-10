#include "VertexBuffer.h"

#include <malloc.h>
#include <memory.h>

#include "subsystems\interfaces\IRenderer.h"

using namespace OrisisEngine;

VertexBuffer::VertexBuffer()
{
	Handle = 0;
}

VertexBuffer::VertexBuffer(const VertexBuffer &obj)
{
}

VertexBuffer::~VertexBuffer()
{
}

void VertexBuffer::AddVertex(Vertex vertex)
{
	_vertices.push_back(vertex);
}

unsigned int VertexBuffer::GetCount()
{
	return _vertices.size();
}

vector<Vertex> VertexBuffer::GetArray()
{
	return _vertices;
}