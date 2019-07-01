#pragma once
#include "osrpch.h"

#include "SpriteBatch.h"

namespace Osiris
{
	SpriteBatch::SpriteBatch()
	{

		float vertices[3 * 2];

		vertices[0] = -0.2f; vertices[1] = 0.0f;
		vertices[2] = 0.0f; vertices[3] = 0.4f;
		vertices[4] = 0.2f; vertices[5] = 0.0f;

		_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		_VertexArray.reset(VertexArray::Create());
		_VertexArray->SetAttribute(0, 0, 2);
		_VertexArray->SetAttribute(1, 2, 2);

		uint32_t indices[3] = { 0, 1, 2 };

		_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
	}

	void SpriteBatch::AddSprite(Sprite* sprite)
	{
		size_t x = ((_indicies.size() / 6) * 4);

		_vertices.push_back(sprite->GetX());
		_vertices.push_back(sprite->GetY());
		
		_vertices.push_back(0.0f);
		_vertices.push_back(0.0f);

		_vertices.push_back(sprite->GetX());
		_vertices.push_back(sprite->GetY() + sprite->GetHeight());

		_vertices.push_back(0.0f);
		_vertices.push_back(-1.0f);

		_vertices.push_back(sprite->GetX() + sprite->GetWidth());
		_vertices.push_back(sprite->GetX() + sprite->GetHeight());

		_vertices.push_back(1.0f);
		_vertices.push_back(-1.0f);

		_vertices.push_back(sprite->GetX() + sprite->GetWidth());
		_vertices.push_back(sprite->GetX());

		_vertices.push_back(1.0f);
		_vertices.push_back(0.0f);

		_indicies.push_back(x + 0);
		_indicies.push_back(x + 1);
		_indicies.push_back(x + 2);
		_indicies.push_back(x + 2);
		_indicies.push_back(x + 3);
		_indicies.push_back(x + 0);

		_VertexArray->Bind();

		_VertexBuffer->Update(&_vertices.at(0), sizeof(float) * _vertices.size());
		_IndexBuffer->Update(&_indicies.at(0), _indicies.size());

	}

	void SpriteBatch::SetTexture(Texture* texture)
	{
		_Texture.reset(texture);
	}

	void SpriteBatch::Render(Renderer& renderer)
	{
		_VertexArray->Bind();
		_IndexBuffer->Bind();
		_VertexBuffer->Bind();

		_Texture->Bind();

		renderer.DrawElements(RendererDrawType::Triangles, _IndexBuffer->GetCount());
	}
}