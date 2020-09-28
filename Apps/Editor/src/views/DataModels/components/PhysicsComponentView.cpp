#pragma once
#include "osrpch.h"

#include <imgui.h>

#include "core/Log.h"
#include "PhysicsComponentView.h"
#include "support/ImGuiUtils.h"

namespace Osiris::Editor
{
	void PhysicsComponentView::OnPropertyEditorDraw()
	{
		if (ImGui::TreeNodeEx("Physics", ImGuiTreeNodeFlags_DefaultOpen))
		{
			bool isTrigger = _BaseComponent->IsTrigger();
			bool isStatic = _BaseComponent->IsStatic();
			bool useSpriteBoundary = _BaseComponent->UseSpriteBoundary();

			if (ImGui::Checkbox("Is Trigger", &isTrigger) == true)
				_BaseComponent->SetIsTrigger(isTrigger);

			if (ImGui::Checkbox("Is Static", &isStatic) == true)
				_BaseComponent->SetIsStatic(isStatic);

			if (ImGui::Checkbox("Use Sprite Boundary", &useSpriteBoundary) == true)
				_BaseComponent->SetUseSpriteBoundary(useSpriteBoundary);

			ImGui::LabelVec2("Velocity", _BaseComponent->GetVelocity());

			ImGui::TreePop();
		}
	}
}