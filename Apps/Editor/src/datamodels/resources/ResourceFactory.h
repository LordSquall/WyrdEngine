/* core wyrd includes */
#include <wyrdpch.h>

/* local includes */
#include "datamodels/resources/ResourceTypes.h"
#include "datamodels/resources/Resource.h"
#include "datamodels/resources/TextureRes.h"
#include "datamodels/resources/SceneRes.h"
#include "datamodels/resources/ScriptRes.h"
#include "datamodels/resources/FontTypeRes.h"
#include "datamodels/resources/MaterialRes.h"
#include "datamodels/resources/ModelRes.h"
#include "datamodels/resources/ShaderRes.h"

namespace Wyrd::Editor
{
	class ResourceFactory
	{
	public:
		static std::shared_ptr<Resource> Create(ResourceType t, const std::filesystem::path& resourceFilePath, const UID& uid)
		{
			switch (t)
			{
			case TEXTURE:
				return std::make_shared<TextureRes>(resourceFilePath, uid);
			case SCENE:
				return std::make_shared<SceneRes>(resourceFilePath, uid);
			case SCRIPT:
				return std::make_shared<ScriptRes>(resourceFilePath, uid);
			case FONT:
				return std::make_shared<FontTypeRes>(resourceFilePath, uid);
			case MATERIAL:
				return std::make_shared<MaterialRes>(resourceFilePath, uid);
			case MODEL:
				return std::make_shared<ModelRes>(resourceFilePath, uid);
			case SHADER:
				return std::make_shared<ShaderRes>(resourceFilePath, uid);
			case UNKNOWN:
			case NONE:
			default:
				return nullptr;
			}
		}
	};
}