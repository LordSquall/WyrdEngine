#pragma once

/* local includes */
#include "wyrdpch.h"
#include "Vertex2DBatch.h"
#include "core/renderer/Renderer.h"
#include "core/renderer/VertexArray.h"
#include "core/renderer/Buffer.h"
#include "core/renderer/Texture.h"
#include "core/renderer/Shader.h"
#include "core/pipeline/SpriteVertex.h"

namespace Wyrd
{
	bool Vertex2DBatch::Initialise(Renderer* renderer)
	{
		_Renderer = renderer;

		Vertex2D verts = { 0.0f, 0.0f };

		_VertexBuffer.reset(VertexBuffer::Create((float*)&verts, sizeof(Vertex2D), "vector2Dbatch"));

		_VertexArray.reset(VertexArray::Create());
		_VertexArray->SetAttribute(0, 0, 2, sizeof(Vertex2D));
		_VertexArray->SetAttribute(1, 2, 4, sizeof(Vertex2D));

		_Shader = nullptr;
		_DrawType = RendererDrawType::Uknown;

		return true;
	}


	void Vertex2DBatch::Submit(DrawRectCommand& cmd)
	{
		bool flushRequired = false;

		/* switching shaders requires a flush */
		if ((_Shader != nullptr) && _Shader != cmd.shader)
			flushRequired = true;

		/* switching primitive type requires a flush */
		if (_DrawType != RendererDrawType::Quads)
			flushRequired = true;

		/* flush if required */
		if (flushRequired == true)
			Flush();

		_DrawType = RendererDrawType::Quads;

		float t = cmd.thickness * 0.5f;

		// top
		_vertices.push_back({cmd.position.x,					cmd.position.y + cmd.size.y + t,	1.0f, 1.0f, 1.0f, 1.0f });	// top right
		_vertices.push_back({cmd.position.x,					cmd.position.y + cmd.size.y - t,	1.0f, 1.0f, 1.0f, 1.0f });	// bottom right
		_vertices.push_back({cmd.position.x + cmd.size.x,		cmd.position.y + cmd.size.y - t,	1.0f, 1.0f, 1.0f, 1.0f });	// bottom left
		_vertices.push_back({cmd.position.x + cmd.size.x,		cmd.position.y + cmd.size.y + t,	1.0f, 1.0f, 1.0f, 1.0f });	// top left 
		// bottom													 
		_vertices.push_back({cmd.position.x,					cmd.position.y + t,					1.0f, 1.0f, 1.0f, 1.0f });	// top right
		_vertices.push_back({cmd.position.x,					cmd.position.y - t,					1.0f, 1.0f, 1.0f, 1.0f });	// bottom right
		_vertices.push_back({cmd.position.x + cmd.size.x,		cmd.position.y - t,					1.0f, 1.0f, 1.0f, 1.0f });	// bottom left
		_vertices.push_back({cmd.position.x + cmd.size.x,		cmd.position.y + t,					1.0f, 1.0f, 1.0f, 1.0f });	// top left 
		// right													 
		_vertices.push_back({cmd.position.x - t,				cmd.position.y + cmd.size.y + t,	1.0f, 1.0f, 1.0f, 1.0f });	// top right
		_vertices.push_back({cmd.position.x - t,				cmd.position.y - t,					1.0f, 1.0f, 1.0f, 1.0f });	// bottom right
		_vertices.push_back({cmd.position.x + t,				cmd.position.y - t,					1.0f, 1.0f, 1.0f, 1.0f });	// bottom left
		_vertices.push_back({cmd.position.x + t,				cmd.position.y + cmd.size.y + t,	1.0f, 1.0f, 1.0f, 1.0f });	// top left 
		// left													 
		_vertices.push_back({cmd.position.x + cmd.size.x - t,	cmd.position.y + cmd.size.y + t,	1.0f, 1.0f, 1.0f, 1.0f });	// top right
		_vertices.push_back({cmd.position.x + cmd.size.x - t,	cmd.position.y - t,					1.0f, 1.0f, 1.0f, 1.0f });	// bottom right
		_vertices.push_back({cmd.position.x + cmd.size.x + t,	cmd.position.y - t,					1.0f, 1.0f, 1.0f, 1.0f });	// bottom left
		_vertices.push_back({cmd.position.x + cmd.size.x + t,	cmd.position.y + cmd.size.y + t,	1.0f, 1.0f, 1.0f, 1.0f });	// top left 

		Vector2 pivot = { cmd.position.x + cmd.rotationOrigin.x, cmd.position.y + cmd.rotationOrigin.y };

		for (int i = 0; i < 16; i++)
		{
			Vertex2D* v = &_vertices[i];
			Vector2 pt1 = Vector2::RotateAtPoint({ v->x, v->y }, -cmd.rotation, pivot);
			v->x = pt1.x;
			v->y = pt1.y;
		}

		/* bind the batch vertex array */
		_VertexArray->Bind();

		/* update both the vertex and index buffers */
		_VertexBuffer->Update((float*)&_vertices.at(0), sizeof(Vertex2D) * (uint32_t)_vertices.size(), 0);

		_VPMatrix = cmd.vpMatrix;
		_Shader = cmd.shader;
		_Color = cmd.color;
	}

	void Vertex2DBatch::Submit(DrawVertex2DCommand& cmd)
	{
		bool flushRequired = false;

		/* switching shaders requires a flush */
		if ((_Shader != nullptr) && _Shader != cmd.shader)
			flushRequired = true;

		/* switching primitive type requires a flush */
		if (_DrawType != cmd.drawType)
			flushRequired = true;

		/* flush if required */
		if (flushRequired == true)
			Flush();

		_DrawType = cmd.drawType;

		/* copy vertices in to batcher */
		for (int i = 0; i < cmd.vertices->size(); ++i)
		{
			Vertex2D v = cmd.vertices->at(i);
			v.x += cmd.position.x;
			v.y += cmd.position.y;
			_vertices.push_back(v);
		}
		
		/* bind the batch vertex array */
		_VertexArray->Bind();

		/* update both the vertex and index buffers */
		_VertexBuffer->Update((float*)&_vertices.at(0), sizeof(Vertex2D) * (uint32_t)_vertices.size(), 0);

		_VPMatrix = cmd.vpMatrix;
		_Shader = cmd.shader;
		_Color = cmd.color;
	}

	void Vertex2DBatch::Flush()
	{
		if (_vertices.size() == 0)
			return;

		if (_Shader == nullptr)
		{
			return;
		}

		_Shader->Bind();

		_Shader->SetVPMatrix(_VPMatrix);

		_Shader->SetModelMatrix(glm::mat4(1.0f));
		_Shader->SetUniformColor("blendColor", _Color);

		_VertexArray->Bind();
		_VertexBuffer->Bind();

#ifdef WYRD_INCLUDE_DEBUG_TAGS
		_Renderer->StartNamedSection("Vertex2D Batch Render");
#endif

		_Renderer->DrawArray(_DrawType, 0, _vertices.size());

#ifdef WYRD_INCLUDE_DEBUG_TAGS
		_Renderer->EndNamedSection();
#endif
		_vertices.clear();
	}
}