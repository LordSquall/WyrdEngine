#include "BatchTable.h"

#include <malloc.h>
#include <memory.h>

#include "subsystems\common\Structures.h"

using namespace OrisisEngine;

BatchTable::BatchTable()
{
}

BatchTable::BatchTable(const BatchTable &obj)
{
}

BatchTable::~BatchTable()
{
}

void BatchTable::Initialise(IRenderer* renderer)
{
	_renderer = renderer;

	_vertexArray.AddVertexBuffer(&_vertexBuffer);

	renderer->CreateVertexArray(&_vertexArray);
	renderer->CreateVertexBuffer(&_vertexBuffer);
}

void BatchTable::AddSpriteEntry(Sprite* sprite, int x, int y)
{	
	float fx = (float)x;
	float fy = (float)y;

	_vertexBuffer.AddVertex(Vertex{ fx, fy + (float)sprite->Height, sprite->TextureU[1],sprite->TextureV[1] });
	_vertexBuffer.AddVertex(Vertex{ fx, fy, sprite->TextureU[0],sprite->TextureV[0] });
	_vertexBuffer.AddVertex(Vertex{ fx + (float)sprite->Width, fy + (float)sprite->Height, sprite->TextureU[2],sprite->TextureV[2] });

	_vertexBuffer.AddVertex(Vertex{ fx + (float)sprite->Width, fy + (float)sprite->Height, sprite->TextureU[2],sprite->TextureV[2] });
	_vertexBuffer.AddVertex(Vertex{ fx, fy, sprite->TextureU[0],sprite->TextureV[0] });
	_vertexBuffer.AddVertex(Vertex{ fx + (float)sprite->Width, fy, sprite->TextureU[3],sprite->TextureV[3] });

	_entries.push_back(BatchTable_Entry(_entries.size() * 6));
}


void BatchTable::PushAllEntries()
{
	if(_vertexBuffer.GetCount() > 0)
		_renderer->UpdateVertexBuffer(&_vertexBuffer, 0, _vertexBuffer.GetCount());
}

void BatchTable::Render()
{
	_renderer->SetTexture(TextureAtlas);
	_renderer->DrawVertexBuffer(&_vertexBuffer);
}

vector<BatchTable_Entry> BatchTable::GetAllEntry()
{
	return _entries;
}