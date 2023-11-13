#pragma once

/* local includes */
#include "core/export.h"
#include "core/Structures.h"
#include "core/renderer/ResourceBase.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <string>

namespace Wyrd
{
	struct ShaderDesc
	{
		ResourceDesc resource;
		std::string vertexSrc;
		std::string fragmentSrc;
		ShaderDesc() : resource() { }
	};

	enum ShaderStage
	{
		Vertex,
		Fragment
	};

	class WYRD_LIBRARY_API Shader : public ResourceBase
	{
	public:
		virtual bool Build(const std::string& vertexSrc, const std::string& fragment, bool isRebuild = false) = 0;
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

		static Shader* Create(ShaderDesc desc);

		const std::string& GetSource(ShaderStage stage);

	protected:
		std::map<ShaderStage, std::string> _Source;
	};
}