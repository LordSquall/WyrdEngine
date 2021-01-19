#pragma once
#include "osrpch.h"

#include <imgui.h>

#include "core/Log.h"
#include "PhysicsComponentView.h"
#include "support/ImGuiUtils.h"

#include <core/Application.h>
#include <core/renderer/Renderer.h>

namespace Osiris::Editor
{
	void PhysicsComponentView::OnPropertyEditorDraw()
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
	}

	void PhysicsComponentView::OnSceneViewerDraw(const glm::mat4& viewProjectMatrix)
	{
		//Osiris::Application::Get().GetRenderer().DrawCircle(_BaseComponent->GetAABB().position, { 10.0f, 20.0f }, { 0.4f, 0.2f, 0.2f, 1.0f }, viewProjectMatrix);

		/* Draw Markers */
		//Osiris::Application::Get().GetRenderer().DrawRect({ { 0.0f, 0.0f }, { 10.0f, 10.0f} }, { 1.0f, 1.0f, 1.0f, 1.0f }, viewProjectMatrix);
		//Osiris::Application::Get().GetRenderer().DrawRect({ { 100.0f, 0.0f }, { 10.0f, 10.0f} }, { 0.8f, 0.2f, 0.2f, 1.0f }, viewProjectMatrix);
		//Osiris::Application::Get().GetRenderer().DrawRect({ { -100.0f, 0.0f }, { 10.0f, 10.0f} }, { 0.4f, 0.2f, 0.2f, 1.0f }, viewProjectMatrix);
		//
		//Osiris::Application::Get().GetRenderer().DrawRect({ { 0.0f, 100.0f,}, { 10.0f, 10.0f} }, { 0.8f, 0.2f, 0.2f, 1.0f }, viewProjectMatrix);
		//Osiris::Application::Get().GetRenderer().DrawRect({ { 0.0f, -100.0f }, { 10.0f, 10.0f} }, { 0.4f, 0.2f, 0.2f, 1.0f }, viewProjectMatrix);
		//Osiris::Application::Get().GetRenderer().DrawRay({ {100.0f, 0.0f}, {0.0f, 1.0f} }, { 1.0f, 0.0f, 0.0f, 1.0f }, 100.0f, 5.0f, viewProjectMatrix);
		//Osiris::Application::Get().GetRenderer().DrawRay({ {100.0f, 0.0f}, {-1.0f, 0.0f} }, { 1.0f, 0.0f, 0.0f, 1.0f }, 100.0f, 5.0f, viewProjectMatrix);
		//Osiris::Application::Get().GetRenderer().DrawRay({ {100.0f, 0.0f}, {0.0f, -1.0f} }, { 1.0f, 0.0f, 0.0f, 1.0f }, 100.0f, 5.0f, viewProjectMatrix);


		//Osiris::Application::Get().GetRenderer().DrawRect(_BaseComponent->GetAABB(), { 0.8f, 0.2f, 0.2f, 1.0f }, viewProjectMatrix);

		//Ray ray{ _BaseComponent->GetAABB().position + (_BaseComponent->GetAABB().size * 0.5f), _BaseComponent->GetVelocity() };
		//Osiris::Application::Get().GetRenderer().DrawRay(ray, { 0.2f, 0.8f, 0.2f, 1.0f }, 100.0f, 5.0f, viewProjectMatrix);

		//OSR_TRACE("Velocity: {0},{1}", _BaseComponent->GetVelocity().x, _BaseComponent->GetVelocity().y);
		
	}
}