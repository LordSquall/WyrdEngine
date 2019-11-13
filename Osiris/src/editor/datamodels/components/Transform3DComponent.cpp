#pragma once
#include "osrpch.h"

#include <imgui.h>

#include "Transform3DComponent.h"

namespace Osiris::Editor
{
	Transform3DComponent::Transform3DComponent()
	{

	}

	Transform3DComponent::Transform3DComponent(const Transform3DComponent& obj)
	{
		position = obj.position;
		rotation = obj.rotation; 
		scale = obj.scale;
	}

	Transform3DComponent::~Transform3DComponent()
	{

	}

	void Transform3DComponent::OnPropertyEditorDraw()
	{
		if (ImGui::TreeNode("Transform (3D)"))
		{
			ImGui::Text("Position");
			ImGui::Text("Rotation");
			ImGui::Text("Scale");
			ImGui::TreePop();
		}
	}
}