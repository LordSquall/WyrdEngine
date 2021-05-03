#pragma once
#include "wyrdpch.h"

#include "TextureLoader.h"

#include "SOIL.h"

namespace Wyrd::Editor
{
	TextureLoader::Result TextureLoader::Load(std::string path, TextureRes& texture)
	{
		int width, height, channels;
		unsigned char* data;

		/* check the file exsits */
		if (!Utils::FileExists(path))
		{
			texture.SetLoaded(false);
			return Result::FileNotFound;
		}

		/* perform the actual load operation */
		data = SOIL_load_image(path.c_str(), &width, &height, &channels, 4);

		if (data == nullptr)
		{
			printf("SOIL loading error: '%s'\n", SOIL_last_result());
			texture.SetLoaded(false);
			return Result::FileMalformed;
		}

		/* set texture details */
		texture.SetWidth(width);
		texture.SetHeight(height);
		texture.SetChannels(channels);
		texture.SetData(data);
		texture.SetLoaded(true);

		return Result::Success;
	}
}