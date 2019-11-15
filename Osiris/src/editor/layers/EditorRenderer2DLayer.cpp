#pragma once
#include "osrpch.h"

#include "EditorRenderer2DLayer.h"
#include "editor/services/ServiceManager.h"

namespace Osiris::Editor
{
	EditorRenderer2DLayer::EditorRenderer2DLayer(std::string name) : Renderer2DLayer(name)
	{
		/* initialise editor services */
		ServiceManager::StartServices();

		/* Initialise the camera controller */
		_CameraController = std::make_shared<OrthographicCameraController>(Application::Get().GetWindow().GetAspectRatio());

		/* register for scene loaded event */
		ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Subscribe(Events::EventType::SceneOpened, EVENT_FUNC(EditorRenderer2DLayer::OnSceneOpened));
	}

	void EditorRenderer2DLayer::OnRender(Timestep ts, Renderer& renderer)
	{
		_CameraController->OnUpdate(ts);

		_Shader->Bind();

		_Shader->SetVPMatrix(_CameraController->GetCamera().GetViewProjectionMatrix());

		if (_Scene != nullptr)
		{
			/* Render Each sprite layer */
			for (auto sl : _Scene->layers2D)
			{
				sl->Render(renderer, *_Shader);
			}
		}
	}

	void EditorRenderer2DLayer::OnEvent(Event& event)
	{
		_CameraController->OnEvent(event);
		//OSR_CORE_INFO("event");
	}


	void EditorRenderer2DLayer::OnSceneOpened(Events::EventArgs& args)
	{
		Events::SceneOpenedArgs& evtArgs = static_cast<Events::SceneOpenedArgs&>(args);

		_Scene = evtArgs.scene;
	}
}