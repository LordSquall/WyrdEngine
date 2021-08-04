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

		_Shader = nullptr;

		return true;
	}


	void Vertex2DBatch::Submit(DrawRectCommand& cmd)
	{
		bool flushRequired = false;

		/* switching shaders requires a flush */
		if ((_Shader != nullptr) && _Shader != cmd.shader)
			flushRequired = true;

		/* flush if required */
		if (flushRequired == true)
			Flush();

		float t = cmd.thickness * 0.5f;

		// top
		_vertices.push_back({cmd.position.x,					cmd.position.y + cmd.size.y + t});	// top right
		_vertices.push_back({cmd.position.x,					cmd.position.y + cmd.size.y - t});	// bottom right
		_vertices.push_back({cmd.position.x + cmd.size.x,		cmd.position.y + cmd.size.y - t});	// bottom left
		_vertices.push_back({cmd.position.x + cmd.size.x,		cmd.position.y + cmd.size.y + t});	// top left 
		// bottom													 
		_vertices.push_back({cmd.position.x,					cmd.position.y + t,			});		// top right
		_vertices.push_back({cmd.position.x,					cmd.position.y - t,			});		// bottom right
		_vertices.push_back({cmd.position.x + cmd.size.x,		cmd.position.y - t,			});		// bottom left
		_vertices.push_back({cmd.position.x + cmd.size.x,		cmd.position.y + t,			});		// top left 
		// right													 
		_vertices.push_back({cmd.position.x - t,				cmd.position.y + cmd.size.y + t});	// top right
		_vertices.push_back({cmd.position.x - t,				cmd.position.y - t,			});		// bottom right
		_vertices.push_back({cmd.position.x + t,				cmd.position.y - t,			});		// bottom left
		_vertices.push_back({cmd.position.x + t,				cmd.position.y + cmd.size.y + t});	// top left 
		// left													 
		_vertices.push_back({cmd.position.x + cmd.size.x - t,	cmd.position.y + cmd.size.y + t});	// top right
		_vertices.push_back({cmd.position.x + cmd.size.x - t,	cmd.position.y - t,			});		// bottom right
		_vertices.push_back({cmd.position.x + cmd.size.x + t,	cmd.position.y - t,			});		// bottom left
		_vertices.push_back({cmd.position.x + cmd.size.x + t,	cmd.position.y + cmd.size.y + t});	// top left 


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

		_Renderer->DrawArray(RendererDrawType::Quads, 0, _vertices.size());

		_vertices.clear();
	}
}