#pragma once

/* local includes */
#include "wyrdpch.h"
#include "TextBatch.h"
#include "core/renderer/Renderer.h"
#include "core/renderer/VertexArray.h"
#include "core/renderer/Buffer.h"
#include "core/renderer/Texture.h"
#include "core/renderer/Shader.h"
#include "core/renderer/FontType.h"
#include "core/pipeline/SpriteVertex.h"

namespace Wyrd
{
	bool TextBatch::Initialise(Renderer* renderer)
	{
		_Renderer = renderer;

		TextVertex2D verts = { 0.0f, 0.0f, 0.0f, 0.0f };

		_VertexBuffer.reset(VertexBuffer::Create((float*)&verts, sizeof(TextVertex2D), "textbatch"));

		_VertexArray.reset(VertexArray::Create());
		_VertexArray->SetAttribute(0, 0, 2, sizeof(TextVertex2D));
		_VertexArray->SetAttribute(1, 2, 2, sizeof(TextVertex2D));

		_Shader = nullptr;

		return true;
	}

	void TextBatch::Submit(DrawTextCommand& cmd)
	{
		bool flushRequired = false;

		/* switching shaders requires a flush */
		if ((_Shader != nullptr) && _Shader != cmd.shader)
			flushRequired = true;

		/* flush if required */
		if (flushRequired == true)
			Flush();

		float x = cmd.position.x;
		float y = cmd.position.y;

		std::string::const_iterator c;
		for (c = cmd.content.begin(); c != cmd.content.end(); c++)
		{
			FontType::Character ch = cmd.font->GetCharacters()[(int)*c];

			float xpos = x + ch.Bearing.x * cmd.scale;
			float ypos = y - (ch.Size.y - ch.Bearing.y) * cmd.scale;

			float w = ch.Size.x * cmd.scale;
			float h = ch.Size.y * cmd.scale;

			_vertices.push_back({ xpos, ypos + h,		0.0f, 0.0f });
			_vertices.push_back({ xpos, ypos,			0.0f, 1.0f });
			_vertices.push_back({ xpos + w, ypos,		1.0f, 1.0f });

			_vertices.push_back({ xpos, ypos + h,		0.0f, 0.0f });
			_vertices.push_back({ xpos + w, ypos,		1.0f, 1.0f });
			_vertices.push_back({ xpos + w, ypos + h,	1.0f, 0.0f });
			
			x += (ch.Advance >> 6) * cmd.scale;

			/* update both the vertex and index buffers */
			_VertexBuffer->Update((float*)&_vertices.at(0), sizeof(TextVertex2D) * (uint32_t)_vertices.size(), 0);


			_VPMatrix = cmd.vpMatrix;
			_Shader = cmd.shader;
			_Color = cmd.color;

			ch.Texture->Bind();
			
			Flush();
		}

		//FontType::Character ch = cmd.font->GetCharacters()[0];

		//float xpos = x + ch.Bearing.x * cmd.scale;
		//float ypos = y - (ch.Size.y - ch.Bearing.y) * cmd.scale;

		//float w = ch.Size.x * cmd.scale;
		//float h = ch.Size.y * cmd.scale;

		//_vertices.push_back({ 0.0f, 100.0f,		0.0f, 0.0f });
		//_vertices.push_back({ 0.0f, 0.0f,		0.0f, 1.0f });
		//_vertices.push_back({ 100.0f, 0.0f,		1.0f, 1.0f });

		//_vertices.push_back({ 0.0f, 100.0f,		0.0f, 0.0f });
		//_vertices.push_back({ 100.0f, 0.0f,		1.0f, 1.0f });
		//_vertices.push_back({ 100.0f, 100.0f,	1.0f, 0.0f });
	}


	void TextBatch::Flush()
	{
		if (_Shader == nullptr)
		{
			return;
		}

		_Shader->Bind();

		_Shader->SetVPMatrix(_VPMatrix);

		//_Shader->SetModelMatrix(glm::mat4(1.0f));
		_Shader->SetUniformColor("blendColor", _Color);

		_VertexArray->Bind();
		_VertexBuffer->Bind();

		_Renderer->DrawArray(RendererDrawType::Triangles, 0, 6);

		_vertices.clear();
	}
}