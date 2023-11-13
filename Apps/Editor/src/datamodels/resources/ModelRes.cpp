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
	ModelRes::ModelRes(const std::string& name, const UID& uid) : Resource(name, uid)
	{

	}
	
	Resource::IOResult ModelRes::Load(const std::string& filepath)
	{
		std::vector<Vertex3D> vertices;
		BoundingBox boundingBox;
		ObjLoader::Load(_path.string().c_str(), &vertices, nullptr, nullptr, 1.0f, &boundingBox);

		MeshDesc meshDesc;
		meshDesc.resource.guid = _resourceID;
		meshDesc.resource.name = _path.stem().string();
		meshDesc.vertices = vertices;
		meshDesc.boundingBox = boundingBox;

		/* load the data from file */
		_mesh = std::shared_ptr<Mesh>(Mesh::Create(meshDesc));

		/* register the mesh with the core resource manager */
		Application::Get().GetResources().Meshs[_resourceID] = _mesh;

		_isLoaded = true;

		return Success;
	}

	Resource::IOResult ModelRes::Load(const jsonxx::Object& obj)
	{
		return NotImplemented;
	}

	Resource::IOResult ModelRes::Save(const std::string& filepath) 
	{
		return NotImplemented;
	}
}


