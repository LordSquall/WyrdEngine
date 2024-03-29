#pragma once

#include "core/renderer/Shader.h"

struct GLFWwindow;

namespace Wyrd
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(ShaderDesc desc);

		bool Build(const std::string& vertexSrc, const std::string& fragment, bool isRebuild = false) override;
		void Bind() override;
		void Unbind() override;
		void SetVPMatrix(const glm::mat4& mat) override;
		void SetModelMatrix(const glm::mat4& mat) override;
		
		void SetUniformFloat(const std::string& name, float val) override;
		void SetUniformVec2(const std::string& name, glm::vec2& vec2) override;
		void SetUniformVec3(const std::string& name, glm::vec3& vec3) override;
		void SetUniformVec4(const std::string& name, glm::vec4& vec4) override;
		void SetUniformColor(const std::string& name, const Color& color) override;

		void SetMatrix(const std::string& name, const glm::mat4& mat) override;

	private:
		uint32_t m_RendererHandle;
	};
}