#pragma once

/* local includes */
#include "core/export.h"
#include "core/Structures.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <string>

namespace Osiris
{
	enum ShaderStage
	{
		Vertex,
		Fragment
	};

	class OSR_LIBRARY_API Shader
	{
	public:
		virtual bool Build(const std::string& vertexSrc, const std::string& fragment) = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetVPMatrix(const glm::mat4& mat) = 0;
		virtual void SetModelMatrix(const glm::mat4& mat) = 0;

		virtual void SetUniformFloat(const std::string& name, float val) = 0;
		virtual void SetUniformVec2(const std::string& name, glm::vec2& vec2) = 0;
		virtual void SetUniformVec3(const std::string& name, glm::vec3& vec3) = 0;
		virtual void SetUniformVec4(const std::string& name, glm::vec4& vec4) = 0;
		virtual void SetUniformColor(const std::string& name, const Color& color) = 0;

		virtual void SetMatrix(const std::string& name, const glm::mat4& mat) = 0;

		static Shader* Create();

		const std::string& GetSource(ShaderStage stage);

	protected:
		std::map<ShaderStage, std::string> _Source;
	};
}