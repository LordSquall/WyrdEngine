/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "Vector3PropertyView.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	void Vector3PropertyView::OnEditorRender(const std::shared_ptr<ScriptProperty>& prop, void* value)
	{
		ImGui::InputFloat3(prop->GetName().c_str(), (float*)value);
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(Vector3PropertyView);
}