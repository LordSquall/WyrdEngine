/* local includes */
#include "wyrdpch.h"
#include "MeshBatch.h"
#include "core/renderer/Renderer.h"
#include "core/renderer/VertexArray.h"
#include "core/renderer/Buffer.h"
#include "core/renderer/Texture.h"
#include "core/renderer/Mesh.h"
#include "core/pipeline/SpriteVertex.h"

namespace Wyrd
{
	bool MeshBatch::Initialise(Renderer* renderer)
	{
		_Renderer = renderer;

		Vertex3D verts = { 0.0f, 0.0f, 0.0f };

		_VertexBuffer.reset(VertexBuffer::Create((float*)&verts, sizeof(Vertex3D), "meshbatch"));

		_VertexArray.reset(VertexArray::Create());
		_VertexArray->SetAttribute(0, 0, 3, sizeof(Vertex3D));
		_VertexArray->SetAttribute(1, 3, 2, sizeof(Vertex3D));

		_Material = nullptr;
		_DrawType = RendererDrawType::Uknown;

		return true;
	}

	void MeshBatch::Submit(DrawMeshCommand& cmd)
	{
		bool flushRequired = false;

		/* switching primitive type requires a flush */
		if (_DrawType != cmd.drawType)
			flushRequired = true;

		/* flush if required */
		if (flushRequired == true)
			Flush();

		_DrawType = cmd.drawType;

		/* copy vertices in to batcher */
		for (int i = 0; i < cmd.mesh->Vertices.size(); ++i)
		{
			Vertex3D v = cmd.mesh->Vertices.at(i);
			v.x += cmd.position.x;
			v.y += cmd.position.y;
			v.z += cmd.position.z;
			_vertices.push_back(v);
		}
		
		/* bind the batch vertex array */
		_VertexArray->Bind();

		/* update both the vertex and index buffers */
		_VertexBuffer->Update((float*)&_vertices.at(0), sizeof(Vertex3D) * (uint32_t)_vertices.size(), 0);

		_ViewMatrix = cmd.viewMatrix;
		_ProjectionMatrix = cmd.projectionMatrix;
		_Material = cmd.material;
		_Color = cmd.color;
		_BaseTexture = cmd.baseTexture;
		_MaterialProps = cmd.materialProps;
	}

	void MeshBatch::Flush()
	{
		if (_vertices.size() == 0)
			return;

		_Material->BindShader();
		_Material->Bind(_MaterialProps);
		_Material->BindViewMatrix(_ViewMatrix);
		_Material->BindProjectionMatrix(_ProjectionMatrix);

		_VertexArray->Bind();
		_VertexBuffer->Bind();

#ifdef WYRD_INCLUDE_DEBUG_TAGS
		_Renderer->StartNamedSection("Vertex2D Batch Render");
#endif

		_Renderer->DrawArray(_DrawType, 0, (uint32_t)_vertices.size());

#ifdef WYRD_INCLUDE_DEBUG_TAGS
		_Renderer->EndNamedSection();
#endif
		_vertices.clear();
	}
}