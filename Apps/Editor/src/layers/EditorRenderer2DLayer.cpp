#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Input.h>
#include <core/MouseCodes.h>
#include <core/scene/Layer2D.h>

/* local includes */
#include "EditorRenderer2DLayer.h"

/* external includes */
#include <imgui.h>

namespace Osiris::Editor
{
	EditorRenderer2DLayer::EditorRenderer2DLayer(std::string name) : Layer(name)
	{
		/* Initialise the camera controller */
		_CameraController = std::make_shared<OrthographicCameraController>(Application::Get().GetWindow().GetAspectRatio(), 1.0f);

		/* Cache services */
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Service::Events);

		/* register events */
		_EventService->Subscribe(Events::EventType::SceneOpened, EVENT_FUNC(EditorRenderer2DLayer::OnSceneOpened));
		_EventService->Subscribe(Events::EventType::SelectedGameObjectChanged, EVENT_FUNC(EditorRenderer2DLayer::OnSelectedGameObjectChanged));

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

		//_Shader->Bind();

		//_Shader->SetVPMatrix(_CameraController->GetCamera().GetViewProjectionMatrix());

		//if (_Scene != nullptr)
		//{
		//	/* Render Each sprite layer */
		//	for (auto sl : _Scene->layers2D)
		//	{
		//		for (auto go : sl->children)
		//		{
		//			for (auto component : go->components)
		//			{
		//				component->Render(ts, renderer);
		//			}
		//		}
		//	}
		//}

		//if (_SelectedGameObject != NULL)
		//{
		//	_TranslationGizmo->SetGameObject(_SelectedGameObject);
		//	_TranslationGizmo->Render(ts, renderer);
		//}

	}

	void EditorRenderer2DLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(OSR_BIND_EVENT_FN(EditorRenderer2DLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(OSR_BIND_EVENT_FN(EditorRenderer2DLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(OSR_BIND_EVENT_FN(EditorRenderer2DLayer::OnMouseMovedEvent));

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


	void EditorRenderer2DLayer::OnGUI()
	{
		if (_OpenContextMenu == true)
		{
			ImGui::SetNextWindowPos({ ImVec2(_MenuPos.x, _MenuPos.y) });
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, 8.0f);
			ImGui::Begin("_ContextMenu", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
			if (ImGui::MenuItem("Copy", 0, false, false) == false) { /* TODO */ }
			if (ImGui::MenuItem("Paste", 0, false, false) == false) { /* TODO */ }
			if (ImGui::MenuItem("Cut", 0, false, false) == false) { /* TODO */ }
			ImGui::Separator();
			if (ImGui::MenuItem("Delete", 0, false, true) == false) { /* TODO */ }
			ImGui::End();
			ImGui::PopStyleVar();
		}
	}

	bool EditorRenderer2DLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		bool itemFound = false;
		glm::vec2 normalisedMouseCoords;
		std::shared_ptr<GameObject> selectedGameObject = nullptr;
		Osiris::Window& window = Application::Get().GetWindow();

		normalisedMouseCoords.x = (2.0f * ((float)e.GetPositionX()) / ((float)window.GetWidth())) - 1.0f;
		normalisedMouseCoords.y = -((2.0f * ((float)e.GetPositionY()) / ((float)window.GetHeight())) - 1.0f);

		if (_Scene != nullptr)
		{
			/* Query each of the layer scene objects */
			for (auto& sl : _Scene->layers2D)
			{
				for (auto& go : sl->children)
				{
					/*glm::mat4 mvpInverse = glm::inverse(_CameraController->GetCamera().GetViewProjectionMatrix() * go->transform2d.matrix);

					glm::vec4 nearPoint = mvpInverse * glm::vec4(normalisedMouseCoords, 0.0f, 1.0f);

					glm::vec4 translatedInputArea = _CameraController->GetCamera().GetViewProjectionMatrix() * (glm::vec4(go->inputArea.x, go->inputArea.y, 0.0, 1.0));
					translatedInputArea.z = go->inputArea.z;
					translatedInputArea.w = go->inputArea.w;

					if (Rect::Contains(translatedInputArea, { nearPoint.x, nearPoint.y }) == true)
					{
						selectedGameObject = go;
						itemFound = true;
					}*/
				}
			}

			switch (e.GetMouseButton())
			{
			case OSR_MOUSE_BUTTON_LEFT:
				_EventService->Publish(Events::EventType::SelectedGameObjectChanged, std::make_shared<Events::SelectedGameObjectChangedArgs>(selectedGameObject));
				_OpenContextMenu = false;
				break;
			case OSR_MOUSE_BUTTON_RIGHT:
				_EventService->Publish(Events::EventType::SelectedGameObjectChanged, std::make_shared<Events::SelectedGameObjectChangedArgs>(selectedGameObject));
				_MenuPos.x = (float)e.GetPositionX();
				_MenuPos.y = (float)e.GetPositionY();
				_OpenContextMenu = true;
				break;
			}
		}

		return true;
	}

	bool EditorRenderer2DLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		return true;
	}

	bool EditorRenderer2DLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		/* calculate the mouse positions and deltas */
		glm::vec2 mouseCoords = { e.GetX(), Application::Get().GetWindow().GetHeight() - e.GetY() };
		glm::vec2 mouseDelta = _LastMousePos - mouseCoords;
		glm::vec2 unitDelta = { mouseDelta.x * (1.0f / (Application::Get().GetWindow().GetWidth() * 0.5f)),  mouseDelta.y * (1.0f / ((Application::Get().GetWindow().GetHeight() * 0.5f) * _CameraController->GetAspectRatio())) };

		unitDelta = unitDelta * (_CameraController->GetAspectRatio() * _CameraController->GetZoomLevel());

		/* on middle mouse click we want to drag the camera around */
		if (Input::IsMouseButtonPressed(OSR_MOUSE_BUTTON_MIDDLE) == true)
		{
			_CameraController->Translate(unitDelta);
		}

		/* on left click with selected game object we want to feed the mouse events to the currently selected gizmo */
		if (Input::IsMouseButtonPressed(OSR_MOUSE_BUTTON_LEFT) && _SelectedGameObject != NULL)
		{
			_TranslationGizmo->OnDrag(-unitDelta);
		}

		_LastMousePos = mouseCoords;
		
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