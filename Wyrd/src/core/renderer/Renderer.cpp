#include "wyrdpch.h"
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
		_vertex2DBatch.Initialise(this);
		_spriteBatch.Initialise(this);
		_textBatch.Initialise(this);
	}

	void Renderer::Submit(DrawRectCommand& cmd)
	{
		_vertex2DBatch.Submit(cmd);
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

	void Renderer::Flush()
	{
		_spriteBatch.Flush();
		_vertex2DBatch.Flush();
	}

#ifdef WYRD_INCLUDE_DEBUG_TAGS
	void Renderer::StartNamedSection(const char* name) const {}
	void Renderer::EndNamedSection() const {}
#endif
}