#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/Input.h>
#include <core/KeyCodes.h>
#include <core/MouseCodes.h>
#include <core/scene/Layer2D.h>
#include <core/scene/SceneLayer.h>
#include <core/scene/components/Transform2DComponent.h>

/* local include */
#include "SceneViewer.h"
#include "gizmos/TranslationGizmo.h"
#include "gizmos/GridGizmo.h"
#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Osiris::Editor
{
	SceneViewer::SceneViewer(EditorLayer* editorLayer) : EditorViewBase("Scene Viewer", editorLayer), _SelectedGameObject(nullptr)
	{
		/* retrieve services */
		_WorkspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Events);
		_ResourceService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);
		_DialogService = ServiceManager::Get<DialogService>(ServiceManager::Dialog);
		_SettingsService = ServiceManager::Get<SettingsService>(ServiceManager::Settings);
		_SimulationService = ServiceManager::Get<SimulationService>(ServiceManager::Simulation);

		/* setup event bindings */
		_EventService->Subscribe(Events::EventType::SceneOpened, EVENT_FUNC(SceneViewer::OnSceneOpened));
		_EventService->Subscribe(Events::EventType::SelectedGameObjectChanged, EVENT_FUNC(SceneViewer::OnSelectedGameObjectChanged));

		/* initialise camera */
		_CameraController = std::make_shared<OrthographicCameraController>();

		/* setup the gizmo shader */
		std::ifstream vertexStream("res/shaders/gizmoGrid.vert");
		std::string vertexSrc((std::istreambuf_iterator<char>(vertexStream)), std::istreambuf_iterator<char>());

		std::ifstream fragmentStream("res/shaders/gizmoGrid.frag");
		std::string fragmentSrc((std::istreambuf_iterator<char>(fragmentStream)), std::istreambuf_iterator<char>());

		_GizmoShader.reset(Shader::Create());

		/* Add shader to core */
		Application::Get().GetResources().Shaders["Gizmo"] = _GizmoShader;

		if (_GizmoShader->Build(vertexSrc, fragmentSrc) == false)
		{
			OSR_ERROR("Unable to build shader.");
		}
		_GizmoShader->Bind();
		_GizmoShader->SetUniformVec4("blendColor", glm::vec4{ 0.8f, 0.2f, 0.2f, 1.0f });
		_GizmoShader->Unbind();

		/* initialise each of the gizmos */
		_GridGizmo = std::make_unique<GridGizmo>(_CameraController);
		_TranslationGizmo = std::make_unique<TranslationGizmo>(this);

		/* create a new framebuffer */
		_Framebuffer.reset(Osiris::FrameBuffer::Create(FrameBufferConfig()));

		/* setup window config */
		config.windowPaddingX = 0.0f;
		config.windowPaddingY = 0.0f;
	}

	SceneViewer::~SceneViewer() 
	{
		OSR_TRACE("SceneViewer::~");
	}

	void SceneViewer::OnUpdate(Timestep ts)
	{
		if (_Scene != nullptr)
		{
			/* Render Each sprite layer */
			for (auto& sl : _Scene->GetLayers())
			{
				for (auto& go : sl->GetGameObjects())
				{
					UpdateGameObject(go, ts, false);
				}
			}
		}
	}

	void SceneViewer::OnRender(Timestep ts, Renderer& renderer)
	{
		if (_Framebuffer->GetConfig().height > 0 && _Framebuffer->GetConfig().width > 0)
		{
			_CameraController->OnUpdate(ts);

			_Framebuffer->Bind();

			renderer.Clear(0.1f, 0.1f, 0.1f);

			if (_GridGizmo->IsEnabled())
				_GridGizmo->Render(ts, renderer);

			if (_Scene != nullptr)
			{
				/* Render each of the sprite layers */
				for (auto& sl : _Scene->GetLayers())
				{
					if (sl->IsVisible())
						sl->Render(renderer, _CameraController->GetCamera().GetViewProjectionMatrix());
				}

				if (_SelectedGameObject)
				{
					for (auto& component : _SelectedGameObject->components)
					{
						if (component->debugOverlayFunction != nullptr)
						{
							component->debugOverlayFunction(renderer, ts, _CameraController->GetCamera().GetViewProjectionMatrix());
						}
					}

					_TranslationGizmo->Render(ts, renderer);
				}
			}

			_Framebuffer->Unbind();
		}
	}

	void SceneViewer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(OSR_BIND_EVENT_FN(SceneViewer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(OSR_BIND_EVENT_FN(SceneViewer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(OSR_BIND_EVENT_FN(SceneViewer::OnMouseScrolledEvent));
		dispatcher.Dispatch<MouseMovedEvent>(OSR_BIND_EVENT_FN(SceneViewer::OnMouseMovedEvent));
		dispatcher.Dispatch<KeyPressedEvent>(OSR_BIND_EVENT_FN(SceneViewer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(OSR_BIND_EVENT_FN(SceneViewer::OnKeyReleasedEvent));


		_TranslationGizmo->OnEvent(event);

		/* Set camera settings */
		if (_Scene != nullptr)
		{
			_Scene->cameraZoom = _CameraController->GetSize();
			_Scene->cameraPosition = _CameraController->GetPosition();
		}
	}

	void SceneViewer::OnEditorRender()
	{
		static bool showStats = false;
		static bool showGizmoSettings = false;

		/* build the top toolbar */
		ImGui::Checkbox("show stats", &showStats);

		/* calculate the mouse offset for events */
		_mouseOffset = { ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y };

		/* calculate the viewport boundary */
		_ViewportBoundary = { {_Boundary.position.x - (_Boundary.size.x - _Viewport.size.x), _Boundary.position.y + (_Boundary.size.y - _Viewport.size.y) }, { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y - ImGui::GetCursorPos().y } };

		/* calculate the viewport size */
		_Viewport = { { 0.0f, 0.0f }, { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y - ImGui::GetCursorPos().y } };

		ImGui::Image((ImTextureID)(UINT_PTR)_Framebuffer->GetColorAttachmentID(), ImVec2(_Viewport.size.x, _Viewport.size.y), ImVec2(0, 1), ImVec2(1, 0));

		if (showStats == true)
		{
			ImGui::SetCursorPosY(42.0f);
			ImGui::Text("Camera:");
			ImGui::Text("\tPosition		[%f, %f, %f]", _CameraController->GetPosition().x, _CameraController->GetPosition().y, _CameraController->GetPosition().z);

			ImGui::Text("Window:");
			ImGui::Text("\tAspect Ratio [%f]", _ViewportBoundary.size.x / _ViewportBoundary.size.y);

			ImGui::Text("FrameBuffer:");
			ImGui::Text("Width [%d]", _Framebuffer->GetConfig().width);
			ImGui::Text("Height [%d]", _Framebuffer->GetConfig().height);

			ImGui::Text("Cursor:");
			ImGui::Text("Screen Position: [%d, %d]", (int32_t)_mouseOffset.x, (int32_t)_mouseOffset.y);
			ImGui::Text("Viewport Offset Coords:   [%d, %d]", (int32_t)_mouseOffset.x, (int32_t)_mouseOffset.y);
			ImGui::Text("Evt Start Coords: [%d, %d]", (int32_t)_LastMousePos.x, (int32_t)_LastMousePos.y);
		}


		if (_SelectedGameObject != nullptr)
		{
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Delete"))
				{
					_EventService->Publish(Events::EventType::SelectedGameObjectChanged, std::make_unique<Events::SelectedGameObjectChangedArgs>(nullptr));
				}
				ImGui::EndPopup();
			}
		}
	}

	void SceneViewer::OnResize()
	{
		/* resize the underlying framebuffer */
		_Framebuffer->Resize((uint32_t)_Viewport.size.x, (uint32_t)_Viewport.size.y);

		/* Set the camera viewport */
		_CameraController->SetViewportSize(_Viewport.size.x, _Viewport.size.y);
	}

	glm::vec2 SceneViewer::Convert2DToWorldSpace(const glm::vec2& point)
	{
		vec2 ndcSpace = GetScreenSpaceFromWorldPoint(point);

		vec4 clipSpace = { ndcSpace.x, ndcSpace.y, -1.0, 1.0f };

		vec4 eyeSpace = glm::inverse(_CameraController->GetCamera().GetProjectionMatrix()) * clipSpace;

		vec4 worldSpace = glm::inverse(_CameraController->GetCamera().GetViewMatrix()) * eyeSpace;

		return { worldSpace.x, worldSpace.y };
	}

	glm::vec2 SceneViewer::GetViewportSpaceFromPoint(const glm::vec2& point)
	{
		return point - _ViewportBoundary.position;
	}

	glm::vec2 SceneViewer::GetWorldSpaceFromPoint(const glm::vec2& point)
	{
		glm::vec2 viewportMouseCoords = point - _ViewportBoundary.position;

		return _CameraController->GetCamera().GetWorldSpaceFromPoint(viewportMouseCoords, _ViewportBoundary);
	}

	glm::vec2 SceneViewer::GetScreenSpaceFromWorldPoint(const glm::vec2& point)
	{
		glm::vec2 viewportMouseCoords = point - _ViewportBoundary.position;

		return _CameraController->GetCamera().GetNDCFromPoint(viewportMouseCoords, _ViewportBoundary);
	}

	bool SceneViewer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		_SimulationService->SetMouseButtonState(e.GetMouseButton(), true);
		//bool itemFound = false;
		//glm::vec2 normalisedMouseCoords;
		//glm::vec2 mouseCoords = glm::vec2(e.GetPositionX(), e.GetPositionY()) - _mouseOffset;


		//if (_Viewport.ContainsPoint(mouseCoords) == true)
		//{
		//	normalisedMouseCoords.x = (2.0f * (mouseCoords.x) / (_Viewport.size.x)) - 1.0f;
		//	normalisedMouseCoords.y = -((2.0f * (mouseCoords.y) / (_Viewport.size.y)) - 1.0f);

		//	if (_Scene != nullptr)
		//	{
		//		/* this point we need to run a check on the scene tree to find which child has been clicked */
		//		std::shared_ptr<GameObject> selectedGameObject = FindGameObjectInScene(normalisedMouseCoords);

		//		switch (e.GetMouseButton())
		//		{
		//		case OSR_MOUSE_BUTTON_LEFT:
		//			_EventService->Publish(Events::EventType::SelectedGameObjectChanged, std::make_shared<Events::SelectedGameObjectChangedArgs>(selectedGameObject));
		//			_OpenContextMenu = false;
		//			break;
		//		case OSR_MOUSE_BUTTON_RIGHT:
		//			_EventService->Publish(Events::EventType::SelectedGameObjectChanged, std::make_shared<Events::SelectedGameObjectChangedArgs>(selectedGameObject));
		//			_MenuPos.x = (float)e.GetPositionX();
		//			_MenuPos.y = (float)e.GetPositionY();
		//			_OpenContextMenu = true;
		//			break;
		//		}
		//	}
		//}

		//_LastMousePos = { mouseCoords.x, mouseCoords.y };

		return true;
	}

	bool SceneViewer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{

		_SimulationService->SetMouseButtonState(e.GetMouseButton(), false);
		return true;
	}

	bool SceneViewer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		_CameraController->OnEvent(e);

		return true;
	}

	bool SceneViewer::OnMouseMovedEvent(MouseMovedEvent& e)
	{ 
		glm::vec2 viewportMouseCoords = glm::vec2(e.GetX(), e.GetY()) - _mouseOffset;

		if (_ViewportBoundary.ContainsPoint(glm::vec2(e.GetX(), e.GetY())))
		{
			/* track the world position of the mouse */
			_MouseWorldPos = _CameraController->GetCamera().GetEyeSpaceFromPoint(viewportMouseCoords, _ViewportBoundary);

			glm::vec2 worldSpaceDelta = _PrevMouseWorldPos - _MouseWorldPos;

			if (Input::IsMouseButtonPressed(OSR_MOUSE_BUTTON_MIDDLE) == true)
			{
				_CameraController->Translate({ worldSpaceDelta.x, worldSpaceDelta.y });
			}
			_PrevMouseWorldPos = _MouseWorldPos;
		}

		return true;
	}

	bool SceneViewer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		_CameraController->OnEvent(e);

		if (e.GetRepeatCount() == 1)
		{
			_SimulationService->SetInputState(e.GetKeyCode(), 0);
		}
		else
		{
			_SimulationService->SetInputState(e.GetKeyCode(), 1);
		}

		return true;
	}

	bool SceneViewer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		_CameraController->OnEvent(e);

		if (_SimulationService->IsRunning())
		{
			_SimulationService->SetInputState(e.GetKeyCode(), 2);
			return true;
		}

		switch (e.GetKeyCode())
		{
		case OSR_KEY_DELETE:
			if (_SelectedGameObject != nullptr)
			{
				_EventService->Publish(Events::EventType::SelectedGameObjectChanged, std::make_unique<Events::SelectedGameObjectChangedArgs>(nullptr));
			}
			break;
		}

		return true;
	}

	void SceneViewer::RenderGameObject(std::unique_ptr<GameObject>& gameObject, Timestep ts, Renderer& renderer)
	{
		for (auto& component : gameObject->components)
		{
			component->Render(ts, renderer);
		}

		for (auto& child : gameObject->GetGameObjects())
		{
			RenderGameObject(child, ts, renderer);
		}
	}

	void SceneViewer::UpdateGameObject(std::unique_ptr<GameObject>& gameObject, Timestep ts, bool updateTransform)
	{
		// check if the game has an invalid matrix as this will need to be propogated

		gameObject->transform->Recalculate();
		for (auto& component : gameObject->components)
		{
			component->Recalculate();
		}

		for (auto& child : gameObject->GetGameObjects())
		{
			//UpdateGameObject(child, ts, transformPropogation);
		}
	}

	GameObject* SceneViewer::FindGameObjectInScene(glm::vec2 inputPosition)
	{
		/* Query each of the layer scene objects in reverse */
		for (std::vector<std::unique_ptr<SceneLayer>>::reverse_iterator layer = _Scene->GetLayers().rbegin(); layer != _Scene->GetLayers().rend(); ++layer)
		{
			for (std::vector<std::unique_ptr<GameObject>>::reverse_iterator gameObject = (*layer)->GetGameObjects().rbegin(); gameObject != (*layer)->GetGameObjects().rend(); ++gameObject)
			{
				GameObject* foundGameObject = FindGameObjectInGameObject((*gameObject).get(), inputPosition);
				if (foundGameObject != nullptr)
				{
					return foundGameObject;
				}
			}
		}

		return nullptr;
	}

	GameObject* SceneViewer::FindGameObjectInGameObject(GameObject* gameObject, glm::vec2 inputPosition)
	{
		for (std::vector<std::unique_ptr<GameObject>>::reverse_iterator child = gameObject->GetGameObjects().rbegin(); child != gameObject->GetGameObjects().rend(); ++child)
		{
			if (FindGameObjectInGameObject((*child).get(), inputPosition) != nullptr)
			{
				return (*child).get();
			}
		}

		/*glm::mat4 mvpInverse = glm::inverse(_CameraController->GetCamera().GetViewProjectionMatrix() * gameObject->transform2D->matrix);
		glm::vec4 nearPoint = mvpInverse * glm::vec4(inputPosition, 0.0f, 1.0f);
		Rect inputArea = { { gameObject->inputArea.x, gameObject->inputArea.y }, { 0.0, 1.0 } };
		Rect translatedInputArea = _CameraController->GetCamera().GetViewProjectionMatrix() * inputArea;
		translatedInputArea.size.x = gameObject->inputArea.z;
		translatedInputArea.size.y = gameObject->inputArea.w;
		if (translatedInputArea.ContainsPoint({ nearPoint.x, nearPoint.y }) == true)
		{
			return gameObject;
		}*/


		return nullptr;
	}

	void SceneViewer::OnSceneOpened(Events::EventArgs& args)
	{
		Events::SceneOpenedArgs& evtArgs = static_cast<Events::SceneOpenedArgs&>(args);

		_Scene = evtArgs.scene;

		/* Set camera settings */
		_CameraController->SetSize(_Scene->cameraZoom);
		_CameraController->SetPosition(_Scene->cameraPosition);
	}

	void SceneViewer::OnSelectedGameObjectChanged(Events::EventArgs& args)
	{
		Events::SelectedGameObjectChangedArgs& evtArgs = static_cast<Events::SelectedGameObjectChangedArgs&>(args);

		_SelectedGameObject = evtArgs.gameObject;
	}
}