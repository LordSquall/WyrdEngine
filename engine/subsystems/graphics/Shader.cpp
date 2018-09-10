#include "Shader.h"

#include <malloc.h>
#include <memory.h>

using namespace OrisisEngine;

Shader::Shader()
{
	Handle = 0;
	VertexHandle = 0;
	FragmentHandle = 0;
}

Shader::Shader(const Shader &obj)
{

}

Shader::~Shader()
{

}