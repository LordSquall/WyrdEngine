#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

namespace Osiris
{
	class Shader
	{
	public:
		virtual bool Build(const std::string& vertexSrc, const std::string& fragment) = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetVPMatrix(glm::mat4& mat) = 0;

		static Shader* Create();

	};
}