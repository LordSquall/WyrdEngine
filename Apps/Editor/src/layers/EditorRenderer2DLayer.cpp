#pragma once
#include "osrpch.h"

#include "EditorRenderer2DLayer.h"
#include "services/ServiceManager.h"

namespace Osiris::Editor
{
	EditorRenderer2DLayer::EditorRenderer2DLayer(std::string name) : Renderer2DLayer(name)
	{
		/* Initialise the camera controller */
		_CameraController = std::make_shared<OrthographicCameraController>(Application::Get().GetWindow().GetAspectRatio(), 1.0f);

		/* register for scene loaded event */
		ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Subscribe(Events::EventType::SceneOpened, EVENT_FUNC(EditorRenderer2DLayer::OnSceneOpened));

		/* register context change events */
		ServiceManager::Get<EventService>(ServiceManager::Service::Events)->Subscribe(Events::EventType::SelectedGameObjectChanged, EVENT_FUNC(EditorRenderer2DLayer::OnSelectedGameObjectChanged));

		/* setup the iconset */
		ServiceManager::Get<ResourceService>(ServiceManager::Service::Resources)->GetIconLibrary().AddIconsFromFile(std::string("res/icons/gizmo-icons.json"));

		/* setup the gizmo shader */
		std::ifstream vertexStream("res/shaders/gizmo.vert");
		std::string vertexSrc((std::istreambuf_iterator<char>(vertexStream)), std::istreambuf_iterator<char>());

		std::ifstream fragmentStream("res/shaders/gizmo.frag");
		std::string fragmentSrc((std::istreambuf_iterator<char>(fragmentStream)), std::istreambuf_iterator<char>());

		_GizmoShader.reset(Shader::Create());

		if (_GizmoShader->Build(vertexSrc, fragmentSrc) == false)
		{
			OSR_ERROR("Unable to build shader.");
		}
		_GizmoShader->Bind();
		_GizmoShader->SetUniformVec3("blendColor", glm::vec3{ 0.8f, 0.2f, 0.2f });

		/* initialise each of the gizmos */
		_TranslationGizmo.reset(new TranslationGizmo(_GizmoShader, _CameraController));
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
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(OSR_BIND_EVENT_FN(EditorRenderer2DLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(OSR_BIND_EVENT_FN(EditorRenderer2DLayer::OnMouseButtonReleasedEvent));

		/* Only in the event of the renderer layer not processing the event, do we pass it on to the camera */
		if (event.Handled == false)
		{
			_CameraController->OnEvent(event);
		}

		/* Set camera settings */
		if (_Scene != nullptr)
		{
			_Scene->cameraZoom = _CameraController->GetZoomLevel();
			_Scene->cameraPosition = _CameraController->GetPosition();
		}
	}

	bool EditorRenderer2DLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		if (_Scene != nullptr)
		{
			/* Query each of the layer scene objects */
			for (auto& sl : _Scene->layers2D)
			{
				for (auto& go : sl->gameobjects)
				{
					glm::vec4 translatedInputArea = go->transform2d->matrix * glm::vec4(go->inputArea.x, go->inputArea.y, 0.0, 1.0);
					translatedInputArea.z = go->inputArea.z;
					translatedInputArea.w = go->inputArea.w;
					if (Rect::Contains(translatedInputArea, { (float)e.GetPositionX(), Application::Get().GetWindow().GetHeight() - (float)e.GetPositionY() }) == true)
					{
						OSR_TRACE("Click on GameObject: {0}", go->name);
					}
				}
			}
		}

		return true;
	}

	bool EditorRenderer2DLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		return true;
	}

	void EditorRenderer2DLayer::OnSceneOpened(Events::EventArgs& args)
	{
		Events::SceneOpenedArgs& evtArgs = static_cast<Events::SceneOpenedArgs&>(args);

		_Scene = evtArgs.scene;

		/* Set camera settings */
		_CameraController->SetZoomLevel(_Scene->cameraZoom);
		_CameraController->SetPosition(_Scene->cameraPosition);
	}

	void EditorRenderer2DLayer::OnSelectedGameObjectChanged(Events::EventArgs& args)
	{
		Events::SelectedGameObjectChangedArgs& evtArgs = static_cast<Events::SelectedGameObjectChangedArgs&>(args);

		_SelectedGameObject = evtArgs.gameObject;
	}
}