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
	bool Vertex3DBatch::Initialise(Renderer* renderer)
	{
		_Renderer = renderer;

		Vertex3D verts = { 0.0f, 0.0f, 0.0f };

		_VertexBuffer.reset(VertexBuffer::Create((float*)&verts, sizeof(Vertex3D), "vector3Dbatch"));

		_VertexArray.reset(VertexArray::Create());
		_VertexArray->SetAttribute(0, 0, 3, sizeof(Vertex3D));

		_Shader = nullptr;
		_DrawType = RendererDrawType::Uknown;

		return true;
	}

	void Vertex3DBatch::Submit(DrawVertex3DCommand& cmd)
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
			Vertex3D v = cmd.vertices->at(i);
			_vertices.push_back(v);
		}
		
		/* bind the batch vertex array */
		_VertexArray->Bind();

		/* update both the vertex and index buffers */
		_VertexBuffer->Update((float*)&_vertices.at(0), sizeof(Vertex3D) * (uint32_t)_vertices.size(), 0);

		_ViewMatrix = cmd.viewMatrix;
		_ProjectionMatrix = cmd.projectionMatrix;
		_Shader = cmd.shader;
		_Color = cmd.color;
	}

	void Vertex3DBatch::Flush()
	{
		if (_vertices.size() == 0)
			return;

		if (_Shader == nullptr)
		{
			return;
		}

		_Shader->Bind();

		_Shader->SetMatrix("u_view", _ViewMatrix);
		_Shader->SetMatrix("u_projection", _ProjectionMatrix);

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