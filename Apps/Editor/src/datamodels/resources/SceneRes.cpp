#include "wyrdpch.h"

#include "SceneRes.h"

#include "services/ServiceManager.h"

namespace Wyrd::Editor
{
	SceneRes::SceneRes(const std::string& name, const UID& uid) : Resource(name, uid)
	{
	}

	Resource::IOResult SceneRes::Load(const std::string& filepath)
	{
		return NotImplemented;
	}

	Resource::IOResult SceneRes::Load(const jsonxx::Object& obj)
	{
		return NotImplemented;
	}

	Resource::IOResult SceneRes::Save(const std::string& filepath) 
	{
		return NotImplemented;
	}

	const Icon& SceneRes::GetThumbnail()
	{
		return ServiceManager::Get<ResourceService>()->RetrieveIcon("common", "assets_scene");
	}
}
