/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "FloatPropertyView.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	void FloatPropertyView::OnEditorRender(const std::shared_ptr<ScriptProperty>& prop, void* value)
	{
		if (value != nullptr)
		{
			ImGui::DragFloat(prop->GetName().c_str(), (float*)value);
		}
		else
		{
			float val = 0.0f;
			ImGui::DragFloat(prop->GetName().c_str(), &val);
		}
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(FloatPropertyView);
}