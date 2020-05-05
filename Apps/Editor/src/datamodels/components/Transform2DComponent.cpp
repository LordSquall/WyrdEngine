#pragma once
#include "osrpch.h"

#include <imgui.h>

#include "Transform2DComponent.h"


namespace Osiris::Editor
{
	Transform2DComponent::Transform2DComponent() : matrix(glm::identity<glm::mat4>())
	{
		position = vec2(0.0f, 0.0f);
		rotation = 0.0f;
		scale = vec2(1.0f, 1.0f);
	}

	Transform2DComponent::Transform2DComponent(const Transform2DComponent& obj) :  matrix(glm::identity<glm::mat4>())
	{
		position = obj.position;
		rotation = obj.rotation;
		scale = obj.scale;

		UpdateModelMatrix();
	}

	Transform2DComponent::~Transform2DComponent()
	{

	}

	void Transform2DComponent::SetPosition(const vec2& pos)
	{ 
		position = pos;
		UpdateModelMatrix();
	}

	void Transform2DComponent::SetRotation(const float rot)
	{ 
		rotation = rot;
		UpdateModelMatrix();
	}

	void Transform2DComponent::SetScale(const vec2& scl)
	{ 
		scale = scl;
		UpdateModelMatrix();
	}

	void Transform2DComponent::UpdateModelMatrix()
	{
		matrix = glm::translate(glm::identity<glm::mat4>(), glm::vec3(position.x, position.y, 0.0f));
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
				position.x = pos[0];
				position.y = pos[1];
				UpdateModelMatrix();
			}
			if (ImGui::InputFloat("Rotation", &rot) == true)
			{
				rotation = rot;
				UpdateModelMatrix();
			}
			if (ImGui::InputFloat2("Scale   ", scl) == true)
			{
				scale.x = scl[0];
				scale.x = scl[1];
				UpdateModelMatrix();
			}

			ImGui::TreePop();
		}
	}
}