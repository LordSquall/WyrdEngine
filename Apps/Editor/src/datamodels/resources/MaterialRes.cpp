/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Application.h>

/* local includes */
#include "MaterialRes.h"
#include "services/ServiceManager.h"
#include "loaders/MaterialLoader.h"

/* external includes */

namespace Wyrd::Editor
{
	MaterialRes::MaterialRes(const std::filesystem::path& filepath, const UID& uid) : Resource(filepath.stem().string(), filepath, uid)
	{

	}
	
	int MaterialRes::Load()
	{
		/* create the core renderer resource and load */
		_material = std::make_shared<Material>();

		/* load the data from file */
		std::string name;
		MaterialLoader::Result loadResult = MaterialLoader::Load(_path, name, _material.get());
		if (loadResult == MaterialLoader::Result::Success)
		{
			/* set the base resource uid and name */
			_material->SetUID(_resourceID);
			_material->SetName(name);

			/* register the material with the core resource manager */
			Application::Get().GetResources().Materials[_resourceID] = _material;

			_isLoaded = true;
		}

		return loadResult;
	}
}
