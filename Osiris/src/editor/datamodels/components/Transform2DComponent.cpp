#pragma once
#include "osrpch.h"

#include <imgui.h>

#include "Transform2DComponent.h"

namespace Osiris::Editor
{
	Transform2DComponent::Transform2DComponent()
	{
		position = vec2(0.0f, 0.0f);
		rotation = 0.0f;
		scale = vec2(1.0f, 1.0f);
	}

	Transform2DComponent::Transform2DComponent(const Transform2DComponent& obj)
	{
		position = obj.position;
		rotation = obj.rotation;
		scale = obj.scale;
	}

	Transform2DComponent::~Transform2DComponent()
	{

	}

	void Transform2DComponent::OnPropertyEditorDraw()
	{
		if (ImGui::TreeNode("Transform (2D)"))
		{
			char buffer[32];

			ImGui::Text("Position");
			ImGui::Text("x");
			ImGui::PushID("pos.x");
			ImGui::SameLine();
			ImGui::InputFloat("##hidelabel", &position.x);
			ImGui::PopID();
			ImGui::Text("y");
			ImGui::PushID("pos.y");
			ImGui::SameLine();
			ImGui::InputFloat("##hidelabel", &position.y);
			ImGui::PopID();


			ImGui::Text("Rotation");
			ImGui::Text("r");
			ImGui::PushID("rot");
			ImGui::SameLine();
			ImGui::InputFloat("##hidelabel", &rotation);
			ImGui::PopID();


			ImGui::Text("Scale");
			ImGui::Text("x");
			ImGui::PushID("scale.x");
			ImGui::SameLine();
			ImGui::InputFloat("##hidelabel", &scale.x);
			ImGui::PopID();
			ImGui::Text("y");
			ImGui::PushID("scale.y");
			ImGui::SameLine();
			ImGui::InputFloat("##hidelabel", &scale.y);
			ImGui::PopID();

			ImGui::TreePop();
		}
	}
}