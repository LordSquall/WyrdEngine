#include "ShaderLoader.h"

#include <fstream>
#include <string>

using namespace OrisisEngine;

ShaderLoader::ShaderLoader()
{
}

ShaderLoader::ShaderLoader(const ShaderLoader &obj)
{
}

ShaderLoader::~ShaderLoader()
{
}

bool ShaderLoader::LoadFile(string vertexFile, string fragmentFile, ILogger* logger, Shader* shader)
{
	ifstream vertexFileStream(vertexFile);
	if (vertexFileStream.is_open())
	{
		shader->VertexFileName.assign(vertexFile);
		shader->VertexSource.assign((std::istreambuf_iterator<char>(vertexFileStream)),
			std::istreambuf_iterator<char>());

	}
	else
	{
		logger->LogError("Unable to located Shader file: " + vertexFile);
		return false;
	}

	ifstream fragmentFileStream(fragmentFile);

	if (fragmentFileStream.is_open())
	{
		shader->FragmentFileName.assign(fragmentFile);
		shader->FragmentSource.assign((std::istreambuf_iterator<char>(fragmentFileStream)),
			std::istreambuf_iterator<char>());
	}
	else
	{
		logger->LogError("Unable to located Shader file: " + fragmentFile);
		return false;
	}

	return true;

}