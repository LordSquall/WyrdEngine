#pragma once
#include "wyrdpch.h"

#include <imgui.h>

#include "CameraComponentView.h"
#include "services/ServiceManager.h"

#include <core/scene/components/Transform2DComponent.h>

namespace Wyrd::Editor
{
	void CameraComponentView::Initialise()
	{
		_BaseComponent->debugOverlayFunction = ONSCENERENDER_BIND(CameraComponentView::OnSceneViewerRender);
	}

	void CameraComponentView::OnPropertyEditorDraw()
	{
		float size;

		size = _BaseComponent->GetSize();

		if (ImGui::InputFloat("Size     ", &size) == true)
		{
			_BaseComponent->SetSize(size);
		}
		
		if (ImGui::Button("Set as Primary Camera"))
		{
			ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Publish(Events::EventType::SetSceneCamera, std::make_unique<Events::SetSceneCameraArgs>(_BaseComponent));
		}
	}

	void CameraComponentView::OnSceneViewerRender(Renderer& renderer, Timestep timestep, const glm::mat4& matrix)
	{
		Transform2DComponent* transform = dynamic_cast<Transform2DComponent*>(_BaseComponent->Owner->transform.get());
		
		float sizeHalf = _BaseComponent->GetSize() / 2.0f;

		const Rect viewport = _BaseComponent->GetCamera().GetViewport();

		renderer.DrawRect({ { viewport.position + transform->position }, viewport.size }, 2.0f, { 0.1f ,0.1f, 0.9f, 1.0f }, matrix);
		
		renderer.DrawCircle(transform->position, { 5.0f, 5.0f }, { 1.0f ,0.0f, 0.0f, 1.0f }, matrix);

	}
}