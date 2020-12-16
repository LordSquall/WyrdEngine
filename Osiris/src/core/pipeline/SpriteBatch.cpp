#pragma once
#include "osrpch.h"

#include "SpriteBatch.h"
#include "core/renderer/VertexArray.h"
#include "core/renderer/Buffer.h"
#include "core/renderer/Texture.h"
#include "core/pipeline/SpriteVertex.h"
#include "core/pipeline/Sprite.h"

namespace Osiris
{

	void SpriteBatchEntry::Update()
	{
		batch->_vertices[offset + 0].x = (float)sprite->GetX();
		batch->_vertices[offset + 0].y = (float)sprite->GetY();
		batch->_vertices[offset + 1].x = (float)sprite->GetX();
		batch->_vertices[offset + 1].y = (float)sprite->GetY() + (float)sprite->GetHeight();
		batch->_vertices[offset + 2].x = (float)sprite->GetX() + (float)sprite->GetWidth();
		batch->_vertices[offset + 2].y = (float)sprite->GetY() + (float)sprite->GetHeight();
		batch->_vertices[offset + 3].x = (float)sprite->GetX() + (float)sprite->GetWidth();
		batch->_vertices[offset + 3].y = (float)sprite->GetY();

		batch->_VertexArray->Bind();
		batch->_VertexBuffer->Update((float*)&batch->_vertices.at(0), sizeof(SpriteVertex) * (uint32_t)batch->_vertices.size(), offset);
	}

	SpriteBatch::SpriteBatch()
	{
		SpriteVertex verts = { 0.0f, 0.0f, 0.0f, 0.0f};

		_VertexBuffer.reset(VertexBuffer::Create((float*)&verts, sizeof(SpriteVertex), "spritebatch"));

		_VertexArray.reset(VertexArray::Create());
		_VertexArray->SetAttribute(0, 0, 2);
		_VertexArray->SetAttribute(1, 2, 2);

		uint32_t index = 0;

		_IndexBuffer.reset(IndexBuffer::Create(&index, sizeof(index) / sizeof(uint32_t)));
	}

	void SpriteBatch::AddSprite(std::shared_ptr<Sprite> sprite)
	{
		unsigned int x = (((unsigned int)_indicies.size() / 6) * 4);

		/* add vertices */
		_vertices.push_back({ (float)sprite->GetX(), (float)sprite->GetY(), 0.0f, 0.0f });
		_vertices.push_back({ (float)sprite->GetX(), (float)sprite->GetY() + (float)sprite->GetHeight(), 0.0f, -1.0f });
		_vertices.push_back({ (float)sprite->GetX() + (float)sprite->GetWidth(), (float)sprite->GetY() + (float)sprite->GetHeight(), 1.0f, -1.0f });
		_vertices.push_back({ (float)sprite->GetX() + (float)sprite->GetWidth(), (float)sprite->GetY(), 1.0f, 0.0f });

		/* add indices */
		_indicies.push_back(x + 0);
		_indicies.push_back(x + 1);
		_indicies.push_back(x + 2);
		_indicies.push_back(x + 2);
		_indicies.push_back(x + 3);
		_indicies.push_back(x + 0);

		/* bind the batch vertex array */
		_VertexArray->Bind();

		/* update both the vertex and index buffers */
		_VertexBuffer->Update((float*)&_vertices.at(0), sizeof(SpriteVertex) * (uint32_t)_vertices.size(), 0);
		_IndexBuffer->Update(&_indicies.at(0), (uint32_t)_indicies.size());

		/* create and add a new batch entry */
		SpriteBatchEntry batchEntry = { sprite, (uint32_t)(_SpriteMap.size() * 4), this };
		_SpriteMap.insert(std::pair<uint32_t, SpriteBatchEntry>(sprite->GetID(), batchEntry));

		/* get the sprite the entry reference */
		//sprite->SetBatchEntry(&_SpriteMap[sprite->GetID()]);
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

		if(_Texture != nullptr)
			_Texture->Bind();

		renderer.DrawElements(RendererDrawType::Triangles, _IndexBuffer->GetCount());
	}
}