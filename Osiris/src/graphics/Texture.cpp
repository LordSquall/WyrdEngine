#pragma once

#include "osrpch.h"
#include "Texture.h"

#include <SOIL.h>

namespace Osiris {

	Texture::Texture()
	{
		
	}

	Texture::~Texture()
	{
		if (m_data != nullptr)
		{
			std::free(m_data);
		}
	}

	std::shared_ptr<Texture> Texture::CreateFromFile(const std::string& filepath)
	{
		int width;
		int height;
		int channels;

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();

		std::string path_delimiter = "/";
		std::string file_delimiter = ".";
		std::string token = filepath.substr(filepath.find_last_of(path_delimiter) + 1); // token is "scott"
		//std::string filename = token.substr(0, token.find(file_delimiter));

		texture->SetName(token);
		texture->SetSourceFileName(filepath);
		texture->SetData(SOIL_load_image(filepath.c_str(), &width, &height, &channels, false));
		texture->SetWidth(width);
		texture->SetHeight(height);
		texture->SetChannels(channels);

		return texture;
	}
}
