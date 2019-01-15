#pragma once

#include "core/export.h"

namespace Osiris {

	class OSIRIS_API Shader
	{
	public:
		Shader();

		virtual ~Shader();


		static std::shared_ptr<Shader> CreateFromFiles(const std::string& vertexSrcFile, const std::string& fragmentSrcFile);

		std::string VertexSrc;
		std::string FragmentSrc;
	};
}