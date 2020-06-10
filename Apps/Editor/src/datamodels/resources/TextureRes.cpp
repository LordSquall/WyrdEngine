#pragma once

#include "osrpch.h"

#include "TextureRes.h"

#include "loaders/TextureLoader.h"

#include <core/renderer/Texture.h>

namespace Osiris::Editor
{
	void TextureRes::Load()
	{
		/* load the data from file */
		TextureLoader::Load(_filePath, *this);

		/* create the core renderer resource and load */
		_texture = std::make_shared<Texture*>(Osiris::Texture::Create(_data, _width, _height, _channels, Utils::GetFilename(_filePath, true) + "_Texture"));
	}

	void TextureRes::Reload()
	{
		/* load the data from file */
		TextureLoader::Load(_filePath, *this);

		(*_texture)->Update(_data, 0, 0, _width, _height);
	}

	void TextureRes::Unload()
	{
		(*_texture)->Delete();
		//TODO
	}
}
