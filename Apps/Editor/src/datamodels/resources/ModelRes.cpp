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
	ModelRes::ModelRes(const std::filesystem::path& filepath, const UID& uid) : Resource(filepath.stem().string(), filepath, uid)
	{

	}
	
	int ModelRes::Load()
	{
		/* load the data from file */
		_mesh = std::make_shared<Mesh>();

		ObjLoader::Load(_path.string().c_str(), &_mesh->Vertices, nullptr, nullptr, 1.0f, &_mesh->boundingBox);

		/* register the mesh with the core resource manager */
		Application::Get().GetResources().Meshs[_resourceID] = _mesh;

		_isLoaded = true;

		return 0;
	}
}
