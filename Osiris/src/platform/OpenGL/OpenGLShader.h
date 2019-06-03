#pragma once

#include "core/renderer/Shader.h"

struct GLFWwindow;

namespace Osiris
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader();

		bool Build(const std::string& vertexSrc, const std::string& fragment) override;
		void Bind() override;
		void Unbind() override;

	private:
		uint32_t m_RendererHandle;
	};
}