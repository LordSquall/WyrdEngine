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

	std::shared_ptr<Shader> Shader::CreateFromFiles(const std::string& vertexSrcFile, const std::string& fragmentSrcFile)
	{
		std::shared_ptr<Shader> shader = std::make_shared<Shader>();
		
		std::ifstream vsFile(vertexSrcFile);
		if (vsFile)
		{
			shader->SetVertexSource(std::string(std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>()));
		}
		else
		{
			OSR_CORE_ERROR("Unable to find file " + std::filesystem::current_path().string() + vertexSrcFile);
		}
		
		std::ifstream fsFile(fragmentSrcFile);
		if (fsFile)
		{
			shader->SetFragmentSource(std::string(std::istreambuf_iterator<char>(fsFile), std::istreambuf_iterator<char>()));
		}
		else
		{
			OSR_CORE_ERROR("Unable to find file " + std::filesystem::current_path().string() + fragmentSrcFile);
		}
		
		std::string path_delimiter = "/";
		std::string file_delimiter = ".";
		std::string token = vertexSrcFile.substr(vertexSrcFile.find_last_of(path_delimiter) + 1);

		shader->SetName(token);

		return shader;
	}
}
