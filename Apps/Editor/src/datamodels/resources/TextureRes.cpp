#pragma once

#include "osrpch.h"

#include "TextureRes.h"

#include "loaders/TextureLoader.h"

namespace Osiris::Editor
{
	void TextureRes::Load()
	{
		/* load the data from file */
		TextureLoader::Load(_filePath, *this);

		/* create the core renderer resource and load */
		_texture = std::make_shared<Texture*>(Osiris::Texture::Create(_data, _width, _height, _channels, Utils::GetFilename(_filePath, true) + "_Texture"));
	}

	void TextureRes::Unload()
	{
		//TODO
	}
}
