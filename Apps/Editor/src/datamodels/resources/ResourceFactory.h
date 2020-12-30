#pragma once

/* core osiris includes */
#include <osrpch.h>

/* local includes */
#include "datamodels/resources/Resource.h"
#include "datamodels/resources/TextureRes.h"
#include "datamodels/resources/SceneRes.h"

namespace Osiris::Editor
{
	class ResourceFactory
	{
	public:
		enum Type
		{
			NONE = 0,
			UNKNOWN = 1,
			TEXTURE = 2,
			SHADER = 3,
			MODEL = 4,
			SCENE = 5,
			SCRIPT = 6
		};

	public:
		static std::shared_ptr<Resource> Create(Type t, const std::string& resourceFilePath)
		{
			switch (t)
			{
			case TEXTURE:
				return std::make_shared<TextureRes>(resourceFilePath);
			case SCENE:
				return std::make_shared<SceneRes>(resourceFilePath);
			case UNKNOWN:
			case NONE:
			default:
				return nullptr;
			}
		}
	};
}