#pragma once

#include "osrpch.h"
#include "Shader.h"

namespace Osiris {

	Shader::Shader()
	{
		
	}

	Shader::~Shader()
	{

	}

	std::unique_ptr<Shader> Shader::CreateFromFiles(const std::string& vertexSrcFile, const std::string& fragmentSrcFile)
	{
		std::unique_ptr<Shader> shader = std::make_unique<Shader>();
		
		std::ifstream vsFile(vertexSrcFile);
		if (vsFile)
		{
			shader->VertexSrc = std::string(std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>());
		}
		else
		{
			OSR_CORE_ERROR("Unable to find file " + std::filesystem::current_path().string() + vertexSrcFile);
		}
		
		std::ifstream fsFile(fragmentSrcFile);
		if (fsFile)
		{
			shader->FragmentSrc = std::string(std::istreambuf_iterator<char>(fsFile), std::istreambuf_iterator<char>());
		}
		else
		{
			OSR_CORE_ERROR("Unable to find file " + std::filesystem::current_path().string() + fragmentSrcFile);
		}
		
		return shader;
	}
}
