#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/ecs/Components.h>

/* local includes */
#include "CameraComponentView.h"

#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	COMPONENT_VIEW_FACTORY_REGISTER(CameraComponentView, "Camera");

	void CameraComponentView::OnEditorRender(void* data)
	{
		/* Cast to the correct component */
		CameraComponent* cameraComponent = (CameraComponent*)data;
		
		ImGui::InputFloat("Size     ", &cameraComponent->size);
	}
}