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
		void SetVPMatrix(const glm::mat4& mat) override;
		void SetModelMatrix(const glm::mat4& mat) override;

		void SetUniformVec2(const std::string& name, glm::vec2& vec2) override;
		void SetUniformVec3(const std::string& name, glm::vec3& vec3) override;
		void SetUniformVec4(const std::string& name, glm::vec4& vec4) override;

	private:
		uint32_t m_RendererHandle;
	};
}