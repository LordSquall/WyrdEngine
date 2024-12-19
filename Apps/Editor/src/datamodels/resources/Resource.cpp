#include "wyrdpch.h"
#include "Resource.h"

#include "services/ServiceManager.h"

#include <core/Log.h>

#include <imgui.h>

namespace Wyrd::Editor
{
	const Icon& Resource::GetThumbnail()
	{
		return ServiceManager::Get<ResourceService>()->RetrieveIcon("common", "assets_3dmodel");
	}

	Resource::IOResult Resource::Reload()
	{
		WYRD_CORE_TRACE("Reload Resource: {0}", _name);
		Load(_path.string());
		for (const auto& r : _listeningResources)
		{
			r.second->Reload();
		}

		return Success;
	}

	void Resource::DrawProperties()
	{
		ImGui::Text("%s", _name.c_str());
	}
}
