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
#include "datamodels/resources/ShaderStageRes.h"

namespace Wyrd::Editor
{
	class ResourceFactory
	{
	public:
		static std::shared_ptr<Resource> Create(ResourceType t, const std::filesystem::path& resourceFilePath, const UID& uid)
		{
			std::string filename = resourceFilePath.stem().string();
			switch (t)
			{
			case TEXTURE:
				return std::make_shared<TextureRes>(filename, uid);
			case SCENE:
				return std::make_shared<SceneRes>(filename, uid);
			case SCRIPT:
				return std::make_shared<ScriptRes>(filename, uid);
			case FONT:
				return std::make_shared<FontTypeRes>(filename, uid);
			case MATERIAL:
				return std::make_shared<MaterialRes>(filename, uid);
			case MODEL:
				return std::make_shared<ModelRes>(filename, uid);
			case SHADER:
				return std::make_shared<ShaderRes>(filename, uid);
			case SHADERSTAGE:
				return std::make_shared<ShaderStageRes>(filename, uid);
			case UNKNOWN:
			case NONE:
			default:
				return nullptr;
			}
		}
	};
}