#pragma once
#include "osrpch.h"

#include "Sprite.h"

#include "SpriteBatch.h"

namespace Osiris
{
	uint32_t Sprite::_nextID = 0;

	Sprite::Sprite(std::string name, int x, int y, int width, int height) : _ID(_nextID), _Name(name), _X(x), _Y(y), _Width(width), _Height(width)
	{
		/* Initialie the vertex buffer */
		_vertices.push_back({ (float)_X, (float)_Y, 0.0f, 0.0f });
		_vertices.push_back({ (float)_X, (float)_Y + (float)_Height, 0.0f, -1.0f });
		_vertices.push_back({ (float)_X + (float)_Width, (float)_Y + (float)_Height, 1.0f, -1.0f });
		_vertices.push_back({ (float)_X + (float)_Width, (float)_Y, 1.0f, 0.0f });

		_VertexBuffer.reset(VertexBuffer::Create((float*)&_vertices[0], sizeof(SpriteVertex), name + "_VertexBuffer"));

		/* Intialise the vertex array */
		_VertexArray.reset(VertexArray::Create());
		_VertexArray->SetAttribute(0, 0, 2);
		_VertexArray->SetAttribute(1, 2, 2);

		/* Intialise the index buffers */
		_indicies.push_back(0);
		_indicies.push_back(1);
		_indicies.push_back(2);
		_indicies.push_back(2);
		_indicies.push_back(3);
		_indicies.push_back(0);
		_IndexBuffer.reset(IndexBuffer::Create(&_indicies[0], sizeof(uint32_t) / sizeof(uint32_t)));

		/* bind the batch vertex array */
		_VertexArray->Bind();

		/* update both the vertex and index buffers */
		_VertexBuffer->Update((float*)&_vertices.at(0), sizeof(SpriteVertex) * (uint32_t)_vertices.size(), 0);
		_IndexBuffer->Update(&_indicies.at(0), (uint32_t)_indicies.size());

		_nextID = _nextID + 1;
	}

	void Sprite::SetX(const int x)
	{
		_X = x;
		UpdateBuffers();
	}

	void Sprite::SetY(const int y)
	{
		_Y = y;
		UpdateBuffers();
	}

	void Sprite::SetWidth(const int width)
	{
		_Width = width;
		UpdateBuffers();
	}

	void Sprite::SetHeight(const int height)
	{
		_Height = height;
		UpdateBuffers();
	}

	void Sprite::UpdateBuffers()
	{

		_vertices[0].x = (float)_X;
		_vertices[0].y = (float)_Y;
		_vertices[1].x = (float)_X;
		_vertices[1].y = (float)_Y + (float)_Height;
		_vertices[2].x = (float)_X + (float)_Width;
		_vertices[2].y = (float)_Y + (float)_Height;
		_vertices[3].x = (float)_X + (float)_Width;
		_vertices[3].y = (float)_Y;

		_VertexArray->Bind();
		_VertexBuffer->Update((float*)&_vertices[0], sizeof(SpriteVertex) * (uint32_t)_vertices.size(), 0);
	}
}