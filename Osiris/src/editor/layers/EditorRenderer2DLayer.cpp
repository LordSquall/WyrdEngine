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

		/* register for scene loaded event */
		ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Subscribe(Events::EventType::SceneOpened, EVENT_FUNC(EditorRenderer2DLayer::OnSceneOpened));
	}

	void EditorRenderer2DLayer::OnRender(Renderer& renderer)
	{
		_Shader->Bind();

		if (_Scene != nullptr)
		{
			/* Render Each sprite layer */
			for (auto sl : _Scene->layers2D)
			{
				sl->Render(renderer);
			}
		}
	}


	void EditorRenderer2DLayer::OnSceneOpened(Events::EventArgs& args)
	{
		Events::SceneOpenedArgs& evtArgs = static_cast<Events::SceneOpenedArgs&>(args);

		_Scene = evtArgs.scene;
	}
}