#pragma once
#include "osrpch.h"

#include <imgui.h>

#include "CameraComponentView.h"

namespace Osiris::Editor
{
	void CameraComponentView::OnPropertyEditorDraw()
	{
		float pos[2];

		pos[0] = _BaseComponent->GetPosition().x;
		pos[1] = _BaseComponent->GetPosition().y;

		if (ImGui::InputFloat2("Pos     ", pos) == true)
		{
			_BaseComponent->SetPosition({ pos[0], pos[1] });
		}
	}
}