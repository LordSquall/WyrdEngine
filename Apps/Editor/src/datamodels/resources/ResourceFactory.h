#pragma once

/* core wyrd includes */
#include <wyrdpch.h>

/* local includes */
#include "datamodels/resources/ResourceTypes.h"
#include "datamodels/resources/Resource.h"
#include "datamodels/resources/TextureRes.h"
#include "datamodels/resources/SceneRes.h"
#include "datamodels/resources/ScriptRes.h"

namespace Wyrd::Editor
{
	class ResourceFactory
	{
	public:
		static std::shared_ptr<Resource> Create(ResourceType t, const std::string& resourceFilePath)
		{
			switch (t)
			{
			case TEXTURE:
				return std::make_shared<TextureRes>(resourceFilePath);
			case SCENE:
				return std::make_shared<SceneRes>(resourceFilePath);
			case SCRIPT:
				return std::make_shared<ScriptRes>(resourceFilePath);
			case UNKNOWN:
			case NONE:
			default:
				return nullptr;
			}
		}
	};
}