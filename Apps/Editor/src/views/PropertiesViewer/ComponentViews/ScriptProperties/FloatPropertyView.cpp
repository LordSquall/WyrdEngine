#pragma once

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
		char buf[255];
		sprintf(buf, "%s -> %p", prop->GetName().c_str(), value);
		ImGui::DragFloat(buf, (float*)value);
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(FloatPropertyView);
}