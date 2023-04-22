/* core wyrd includes */
#include <wyrdpch.h>
#include <core/renderer/Mesh.h>

/* local includes */
#include "ModelRes.h"
#include "services/ServiceManager.h"
#include "loaders/ObjLoader.h"

/* external includes */
#include <core/Application.h>

namespace Wyrd::Editor
{
	ModelRes::ModelRes(const std::filesystem::path& filepath) : Resource(filepath.stem().string(), filepath), _loaded(false)
	{

	}
	
	bool ModelRes::Load()
	{
		/* load the data from file */
		_mesh = std::make_shared<Mesh>();

		ObjLoader::Load(_path.string().c_str(), &_mesh->Vertices);

		/* register the mesh with the core resource manager */
		Application::Get().GetResources().Meshs[_resourceID] = _mesh;

		_loaded = true;

		return _loaded;
	}
}
