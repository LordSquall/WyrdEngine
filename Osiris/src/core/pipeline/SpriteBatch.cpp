#pragma once

/* local includes */
#include "osrpch.h"
#include "SpriteBatch.h"
#include "core/renderer/VertexArray.h"
#include "core/renderer/Buffer.h"
#include "core/renderer/Texture.h"
#include "core/renderer/Shader.h"
#include "core/pipeline/SpriteVertex.h"
#include "core/scene/GameObject.h"
#include "core/scene/components/Transform2DComponent.h"
#include "core/scene/components/SpriteComponent.h"

namespace Osiris
{
	SpriteBatch::SpriteBatch()
	{
		SpriteVertex verts = { 0.0f, 0.0f, 0.0f, 0.0f};

		_VertexBuffer.reset(VertexBuffer::Create((float*)&verts, sizeof(SpriteVertex), "spritebatch"));

		_VertexArray.reset(VertexArray::Create());
		_VertexArray->SetAttribute(0, 0, 2);
		_VertexArray->SetAttribute(1, 2, 2);
	}

	void SpriteBatch::AddSprite(std::shared_ptr<SpriteComponent> sprite)
	{
		/* add vertices */
		_vertices.push_back({ (float)sprite->position.x, (float)sprite->position.y, 0.0f, 0.0f });
		_vertices.push_back({ (float)sprite->position.x, (float)sprite->position.y + (float)sprite->size.y, 0.0f, -1.0f });
		_vertices.push_back({ (float)sprite->position.x + (float)sprite->size.x, (float)sprite->position.y + (float)sprite->size.y, 1.0f, -1.0f });
		_vertices.push_back({ (float)sprite->position.x + (float)sprite->size.x, (float)sprite->position.y, 1.0f, 0.0f });

		/* bind the batch vertex array */
		_VertexArray->Bind();

		/* update both the vertex and index buffers */
		_VertexBuffer->Update((float*)&_vertices.at(0), sizeof(SpriteVertex) * (uint32_t)_vertices.size(), 0);

		/* create and add a new batch entry */
		_SpriteMap.push_back({ sprite, (uint32_t)(_SpriteMap.size() * 4), this });
	}

	void SpriteBatch::RemoveSprite(SpriteComponent* sprite)
	{
		/* retrieve the batch entry */
		auto spriteBatchEntry = std::find_if(_SpriteMap.begin(), _SpriteMap.end(), [&sprite](const SpriteBatchEntry& s) { return sprite == s.sprite.get(); });

		if (spriteBatchEntry != _SpriteMap.end())
		{
			_SpriteMap.erase(spriteBatchEntry);
		}
	}

	void SpriteBatch::SetShader(std::shared_ptr<Shader> shader)
	{
		_Shader = shader;
	}

	void SpriteBatch::Render(Renderer& renderer, const glm::mat4& viewProjectionMat)
	{
		_Shader->Bind();

		_Shader->SetVPMatrix(viewProjectionMat);

		_Shader->SetModelMatrix(glm::mat4(1.0f));

		_VertexArray->Bind();
		_VertexBuffer->Bind();

		for (auto&& sprite : _SpriteMap)
		{
			sprite.sprite->texture->Bind();

			_Shader->SetModelMatrix(sprite.sprite->Owner->transform2D->matrix);
			_Shader->SetUniformColor("blendColor", sprite.sprite->color);

			renderer.DrawArray(RendererDrawType::TriangleFan, sprite.offset, 4);
		}
	}
}