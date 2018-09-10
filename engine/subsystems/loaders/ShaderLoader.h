#ifndef _SHADER_LOADER_H_
#define _SHADER_LOADER_H_

#include "export.h"
#include "subsystems\interfaces\ILogger.h"
#include "subsystems\graphics\Shader.h"

namespace OrisisEngine
{
	class ShaderLoader {
		public:
			ShaderLoader();
			ShaderLoader(const ShaderLoader &obj);
			~ShaderLoader();

			static bool LoadFile(string vertexFile, string fragmentFile, ILogger* logger, Shader* shader);
	};
}

#endif