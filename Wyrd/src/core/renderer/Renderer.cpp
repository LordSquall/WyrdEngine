#include "wyrdpch.h"
#include "core/Application.h"
#include "core/renderer/Renderer.h"
#include "platform/OpenGL/OpenGLRenderer.h"

namespace Wyrd
{
	RendererAPI Renderer::s_RendererAPI = RendererAPI::OpenGL;

	Renderer* Renderer::Create()
	{
		Renderer* newRenderer = nullptr;
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		OSR_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::OpenGL:	newRenderer = new OpenGLRenderer();
		}

		if (newRenderer == nullptr)
		{
			OSR_CORE_ASSERT(false, "Unknown Renderer API");
			return newRenderer;
		}

		/* Initialise the renderer subsystems */
		newRenderer->Initialise();

		return newRenderer;
	}

	void Renderer::Initialise() 
	{
		_MeshBatch.Initialise(this);
		_vertex3DBatch.Initialise(this);
		_vertex2DBatch.Initialise(this);
		_spriteBatch.Initialise(this);
		_textBatch.Initialise(this);

		Vertex3D verts = { 0.0f, 0.0f, 0.0f };

		_DebugVertexBuffer.reset(VertexBuffer::Create((float*)&verts, sizeof(Vertex3D), "debug3d"));
		_DebugVertexArray.reset(VertexArray::Create());
		_DebugVertexArray->SetAttribute(0, 0, 3, sizeof(Vertex3D));
		_DebugVertexArray->SetAttribute(1, 3, 2, sizeof(Vertex3D));
	}

	void Renderer::Submit(DrawRectCommand& cmd)
	{
		_vertex2DBatch.Submit(cmd);
	}


	void Renderer::Submit(DrawMeshCommand& cmd)
	{
		_MeshBatch.Submit(cmd);
	}

	void Renderer::Submit(DrawSpriteCommand& cmd)
	{
		_spriteBatch.Submit(cmd);
	}

	void Renderer::Submit(DrawTextCommand& cmd)
	{
		_textBatch.Submit(cmd);
	}


	void Renderer::Submit(DrawVertex2DCommand& cmd)
	{
		_vertex2DBatch.Submit(cmd);
	}

	void Renderer::Submit(DrawVertex3DCommand& cmd)
	{
		_vertex3DBatch.Submit(cmd);
	}

	void Renderer::Submit(DrawMeshInputColorPickerCommand& cmd)
	{

	}

	void Renderer::Flush()
	{
		_vertex3DBatch.Flush();
		_MeshBatch.Flush();
		//_spriteBatch.Flush();
		//_vertex2DBatch.Flush();
	}

	void Renderer::SetFallbackMaterial(std::shared_ptr<Material> fallbackMaterial)
	{
		/* Setup fallback resources */
		_MeshBatch.SetFallbackMaterial(fallbackMaterial);
	}

#ifdef WYRD_INCLUDE_DEBUG_TAGS
	void Renderer::StartNamedSection(const char* name) const {}
	void Renderer::EndNamedSection() const {}
#endif
}