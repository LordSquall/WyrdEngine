#pragma once
#include "osrpch.h"

#include <imgui.h>

#include "Transform2DComponentView.h"

namespace Osiris::Editor
{
	void Transform2DComponentView::OnPropertyEditorDraw()
	{
		_EnableControls = false;

		float pos[2];

		pos[0] = _BaseComponent->position.x;
		pos[1] = _BaseComponent->position.y;

		if (ImGui::InputFloat2("Pos     ", pos) == true)
		{
			_BaseComponent->SetPosition({ pos[0], pos[1] });
		}
	}
}