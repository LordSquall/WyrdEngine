#pragma once
#include "osrpch.h"

#include <imgui.h>

#include "Transform2DView.h"

namespace Osiris::Editor
{
	void Transform2DView::OnPropertyEditorDraw()
	{
		if (ImGui::TreeNodeEx("Transform (2D)", ImGuiTreeNodeFlags_DefaultOpen))
		{
			float pos[2];
			float rot;
			float scl[2];

			pos[0] = _BaseComponent->position.x;
			pos[1] = _BaseComponent->position.y;
			rot = _BaseComponent->rotation;
			scl[0] = _BaseComponent->scale.x;
			scl[1] = _BaseComponent->scale.y;

			if (ImGui::InputFloat2("Pos     ", pos) == true)
			{
				_BaseComponent->SetPosition({ pos[0], pos[1] });
			}
			if (ImGui::InputFloat("Rotation", &rot) == true)
			{
				_BaseComponent->SetRotation(rot);
			}
			if (ImGui::InputFloat2("Scale   ", scl) == true)
			{
				_BaseComponent->SetScale({ scl[0], scl[1] });
			}

			ImGui::TreePop();
		}
	}
}