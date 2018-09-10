#ifndef _SHADER_H_
#define _SHADER_H_

#include "export.h"

#include <string>

using namespace std;

namespace OrisisEngine
{
	class Shader {
	public:
		Shader();
		Shader(const Shader &obj);
		~Shader();

	public:
		unsigned int Handle;
		unsigned int VertexHandle;
		unsigned int FragmentHandle;

		string VertexFileName;
		string VertexSource;

		string FragmentFileName;
		string FragmentSource;
	};
}

#endif