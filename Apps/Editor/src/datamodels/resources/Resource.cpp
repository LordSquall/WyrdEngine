#include "wyrdpch.h"
#include "Resource.h"

#include <core/Log.h>

#include <imgui.h>

namespace Wyrd::Editor
{

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
