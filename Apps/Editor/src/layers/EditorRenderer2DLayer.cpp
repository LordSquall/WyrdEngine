#pragma once
#include "osrpch.h"

#include "EditorRenderer2DLayer.h"
#include "services/ServiceManager.h"

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

		/* register context change events */
		ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Subscribe(Events::EventType::SelectedGameObjectChanged, EVENT_FUNC(EditorRenderer2DLayer::OnSelectedGameObjectChanged));

		_TranslationGizmo.reset(new TranslationGizmo());
		_TranslationGizmo->SetCameraController(_CameraController);
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
				for (auto go : sl->gameobjects)
				{
					std::shared_ptr<Sprite> sprite = go->spriteRender->Sprite;

					sprite->GetVertexArray()->Bind();
					sprite->GetVertexBuffer()->Bind();
					sprite->GetIndexBuffer()->Bind();
					(*sprite->GetTexture())->Bind();

					_Shader->SetModelMatrix(go->transform2d->matrix);

					_Shader->SetUniformVec3("blendColor", go->spriteRender->Color);

					renderer.DrawElements(RendererDrawType::Triangles, 6);
				}
			}
		}

		if (_SelectedGameObject != NULL)
		{
			_TranslationGizmo->SetGameObject(_SelectedGameObject);
			_TranslationGizmo->Render(ts, renderer);
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

	void EditorRenderer2DLayer::OnSelectedGameObjectChanged(Events::EventArgs& args)
	{
		Events::SelectedGameObjectChangedArgs& evtArgs = static_cast<Events::SelectedGameObjectChangedArgs&>(args);

		_SelectedGameObject = evtArgs.gameObject;
	}
}