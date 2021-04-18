#pragma once

#include "core/renderer/Renderer.h"

namespace Osiris
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
		virtual	void DrawRect(const Rect& rect, float thickness, const glm::vec4& color, const glm::mat4& vpMatrix) const override;
		virtual	void DrawFilledRect(const Rect& rect, const glm::vec4& color, const glm::mat4& vpMatrix) const override;
		virtual	void DrawRay(const Ray& ray, const glm::vec4& color, float length, float thickness, const glm::mat4& vpMatrix) const override;
		virtual	void DrawCircle(const glm::vec2& position, const glm::vec2& radius, const glm::vec4& color, const glm::mat4& vpMatrix) const override;

	private:
		int _debugShader;
		unsigned int _debugVAO, _debugVBO, _debugEBO;
		unsigned int _debugVPMatLoc, _debugModelMatLoc, _debugColorLoc;
	};
}