/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/ecs/Components.h>

/* local includes */
#include "CameraComponentView.h"
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	COMPONENT_VIEW_FACTORY_REGISTER(CameraComponentView, "Camera");

	void CameraComponentView::OnEditorRender(Entity e, void* data)
	{
		/* Cast to the correct component */
		CameraComponent* cameraComponent = (CameraComponent*)data;
		
		ImGui::InputFloat2("Size      ", (float*)&cameraComponent->size);
		ImGui::InputFloat ("Aspect    ", &cameraComponent->aspect);
		ImGui::InputFloat ("Near Plane", &cameraComponent->nearPlane);
		ImGui::InputFloat ("Far Plane ", &cameraComponent->farPlane);

		if (ImGui::Button("Set As Main Camera"))
		{
			ServiceManager::Get<EventService>()->Publish(Editor::Events::EventType::SetSceneCamera, std::make_unique<Events::SetSceneCameraArgs>(e, cameraComponent));
		}
	}
}