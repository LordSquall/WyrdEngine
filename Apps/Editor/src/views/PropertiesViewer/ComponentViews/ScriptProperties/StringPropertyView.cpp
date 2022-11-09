/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/ecs/Components.h>

/* local includes */
#include "StringPropertyView.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	void StringPropertyView::OnEditorRender(const std::shared_ptr<ScriptProperty>& prop, void* value)
	{
		ImGui::InputText(prop->GetName().c_str(), (std::string*)value);
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(StringPropertyView);
}