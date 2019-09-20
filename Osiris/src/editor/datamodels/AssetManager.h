#pragma once

#include <Osiris.h>

#include "TextureRes.h"

using namespace Osiris;


namespace Osiris::Editor {

	class AssetManager
	{
	public:
		AssetManager() {}
		~AssetManager() {}

	private:
		std::map<std::string, TextureRes> _textureResources;
	};
}