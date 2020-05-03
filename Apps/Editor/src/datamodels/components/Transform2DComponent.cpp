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

			float pos[2];
			float rot;
			float scl[2];

			pos[0] = position.x;
			pos[1] = position.y;
			rot = rotation;
			scl[0] = scale.x;
			scl[1] = scale.y;

			if (ImGui::InputFloat2("Pos     ", pos) == true)
			{
				//Sprite->SetPosition(position[0], position[1]);
			}
			if (ImGui::InputFloat("Rotation", &rot) == true)
			{
				//Sprite->SetPosition(position[0], position[1]);
			}
			if (ImGui::InputFloat2("Scale   ", scl) == true)
			{
				//Sprite->SetPosition(position[0], position[1]);
			}

			ImGui::TreePop();
		}
	}
}