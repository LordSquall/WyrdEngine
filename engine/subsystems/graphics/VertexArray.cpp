#include "VertexArray.h"

#include <malloc.h>
#include <memory.h>

#include "subsystems\interfaces\IRenderer.h"

using namespace OrisisEngine;

VertexArray::VertexArray()
{
	Handle = 0;
}

VertexArray::VertexArray(const VertexArray &obj)
{
}

VertexArray::~VertexArray()
{
}

void VertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer)
{
	_vertexBuffers.push_back(vertexBuffer);
}


void VertexBuffer::Generate(IRenderer* renderer)
{

}