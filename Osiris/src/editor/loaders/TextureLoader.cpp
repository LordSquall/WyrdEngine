#pragma once
#include "osrpch.h"

#include "TextureLoader.h"

#include "SOIL.h"

namespace Osiris::Editor
{
	TextureLoader::Result TextureLoader::Load(std::string path, TextureRes& texture)
	{
		int width, height, channels;
		unsigned char* data;

		data = SOIL_load_image(path.c_str(), &width, &height, &channels, 4);

		texture.SetFilePath(path);
		texture.SetWidth(width);
		texture.SetHeight(height);
		texture.SetChannels(channels);
		texture.SetData(data);

		return Result::Success;
	}
}