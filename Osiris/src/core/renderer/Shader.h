#pragma once

#include <string>

namespace Osiris
{
	class Shader
	{
	public:
		virtual bool Build(const std::string& vertexSrc, const std::string& fragment) = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static Shader* Create();

	};
}