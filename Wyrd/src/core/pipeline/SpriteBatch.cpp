#pragma once

/* local includes */
#include "wyrdpch.h"
#include "SpriteBatch.h"
#include "core/renderer/Renderer.h"
#include "core/renderer/VertexArray.h"
#include "core/renderer/Buffer.h"
#include "core/renderer/Texture.h"
#include "core/renderer/Shader.h"
#include "core/pipeline/SpriteVertex.h"

namespace Wyrd
{
	bool SpriteBatch::Initialise(Renderer* renderer)
	{
		_Renderer = renderer;
		_SpriteCount = 0;

		SpriteVertex verts = { 0.0f, 0.0f, 0.0f, 0.0f };

		_VertexBuffer.reset(VertexBuffer::Create((float*)&verts, sizeof(SpriteVertex), "spritebatch"));

		_VertexArray.reset(VertexArray::Create());
		_VertexArray->SetAttribute(0, 0, 2, sizeof(SpriteVertex));
		_VertexArray->SetAttribute(1, 2, 2, sizeof(SpriteVertex));

		_Shader = nullptr;
		_Texture = nullptr;

		return true;
	}


	void SpriteBatch::Submit(DrawSpriteCommand& cmd)
	{
		bool flushRequired = false;

		/* switching texture requires a flush */
		if ((_Texture != nullptr) && _Texture != cmd.texture)
			flushRequired = true;

		/* flush if required */
		if (flushRequired == true)
			Flush();

		/* record initial offset */
		size_t offset = sizeof(SpriteVertex) * (uint32_t)_vertices.size();

		/* add vertices */
		_vertices.push_back({ (float)cmd.position.x, (float)cmd.position.y, 0.0, 0.0f });
		_vertices.push_back({ (float)cmd.position.x, (float)cmd.position.y + (float)cmd.size.y, 0.0f, -(cmd.tiling.y) });
		_vertices.push_back({ (float)cmd.position.x + (float)cmd.size.x, (float)cmd.position.y + (float)cmd.size.y, (cmd.tiling.x), -(cmd.tiling.y) });
		_vertices.push_back({ (float)cmd.position.x + (float)cmd.size.x, (float)cmd.position.y + (float)cmd.size.y, (cmd.tiling.x), -(cmd.tiling.y) });
		_vertices.push_back({ (float)cmd.position.x + (float)cmd.size.x, (float)cmd.position.y, (cmd.tiling.x), 0.0f });
		_vertices.push_back({ (float)cmd.position.x, (float)cmd.position.y, 0.0f, 0.0f });

		/* bind the batch vertex array */
		_VertexArray->Bind();

		/* update both the vertex and index buffers */
		_VertexBuffer->Update((float*)&_vertices.at(0), sizeof(SpriteVertex) * _vertices.size(), offset);

		_VPMatrix = cmd.vpMatrix;
		_Shader = cmd.shader;
		_Texture = cmd.texture;
		_Color = cmd.color;

		_SpriteCount++;
	}


	void SpriteBatch::Flush()
	{
		if (_Shader == nullptr)
		{
			return;
		}

		_Shader->Bind();

		_Shader->SetVPMatrix(_VPMatrix);

		_Shader->SetModelMatrix(glm::mat4(1.0f));
		_Shader->SetUniformColor("blendColor", _Color);

		if (_Texture != nullptr)
			_Texture->Bind();

		_VertexArray->Bind();
		_VertexBuffer->Bind();

		_Renderer->DrawArray(RendererDrawType::Triangles, 0, _SpriteCount * 6);

		_vertices.clear();
		_SpriteCount = 0;
	}
}