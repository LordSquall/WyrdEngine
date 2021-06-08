#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Structures.h>

/* local includes */
#include "ColorPropertyView.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	void ColorPropertyView::OnEditorRender(const std::shared_ptr<ScriptProperty>& prop, void* value)
	{
		ImGui::ColorEdit4(prop->GetName().c_str(), (float*)value);
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(ColorPropertyView);
}