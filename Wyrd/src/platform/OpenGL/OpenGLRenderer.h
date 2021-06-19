#pragma once

#include "core/renderer/Renderer.h"

namespace Wyrd
{
	class OpenGLRenderer : public Renderer
	{
	public:
		OpenGLRenderer();
		~OpenGLRenderer();

		virtual void SetViewport(float width, float height) override;
		virtual void Clear(float r, float g, float b) const override;
		virtual void DrawElements(RendererDrawType type, uint32_t count) const override;
		virtual void DrawArray(RendererDrawType type, uint32_t offset, uint32_t count) const override;
	};
}