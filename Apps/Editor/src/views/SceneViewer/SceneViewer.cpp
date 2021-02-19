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
	SceneViewer::SceneViewer(EditorLayer* editorLayer) : EditorViewBase("Scene Viewer", editorLayer)
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
		Application::Get().GetResources().Shaders["GridGizmo"] = _GizmoShader;

		if (_GizmoShader->Build(vertexSrc, fragmentSrc) == false)
		{
			OSR_ERROR("Unable to build shader.");
		}
		_GizmoShader->Bind();
		_GizmoShader->SetUniformVec4("blendColor", glm::vec4{ 0.8f, 0.2f, 0.2f, 1.0f });
		_GizmoShader->Unbind();

		/* initialise each of the gizmos */
		_GridGizmo = std::make_unique<GridGizmo>(_CameraController);

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
			for (auto sl : _Scene->layers2D)
			{
				for (auto go : sl->children)
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
				for (auto sl : _Scene->layers2D)
				{
					sl->Render(renderer, _CameraController->GetCamera().GetViewProjectionMatrix());
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

		ImGui::SameLine();

		if (ImGui::Button("Gizmos"))
		{
			showGizmoSettings = true;
		}

		if (showGizmoSettings == true)
		{
			ImGui::SetNextWindowPos({ _Boundary.position.x + ImGui::GetCursorPosX(), _Boundary.position.y + ImGui::GetCursorPosY() });
			ImGui::Begin("gizmo_controls", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
			if (ImGui::CollapsingHeader("Grid", ImGuiTreeNodeFlags_DefaultOpen))
			{
				static bool enableGrid = _GridGizmo->IsEnabled();
				static Color gridColor = _GridGizmo->GetColor();
				if (ImGui::Checkbox("Enable Grid", &enableGrid)) _GridGizmo->ToggleEnabled();
				if (ImGui::ColorEdit4("Color", (float*)&gridColor)) _GridGizmo->SetColor(gridColor);
			}


			/* build the top toolbar */
			ImGui::Checkbox("show stats", &showStats);				ImGui::End();
		}

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
					_SelectedGameObject->parent->RemoveChild(_SelectedGameObject->uid);
					_EventService->Publish(Events::EventType::SelectedGameObjectChanged, std::make_shared<Events::SelectedGameObjectChangedArgs>(nullptr));
				}
				ImGui::EndPopup();
			}
		}
	}

	void SceneViewer::OnResize()
	{
		_Framebuffer->Resize((uint32_t)_Viewport.size.x, (uint32_t)_Viewport.size.y);
		_CameraController->SetViewportSize(_Viewport.size.x, _Viewport.size.y);
	}


	glm::vec2 SceneViewer::GetWorldSpaceFromPoint(const glm::vec2& point)
	{
		glm::vec2 viewportMouseCoords = point - _ViewportBoundary.position;

		return _CameraController->GetCamera().GetWorldSpaceFromPoint(viewportMouseCoords, _ViewportBoundary);
	}

	bool SceneViewer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
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
			glm::vec2 worldSpaceCoords = _CameraController->GetCamera().GetEyeSpaceFromPoint(viewportMouseCoords, _ViewportBoundary);

			glm::vec2 worldSpaceDelta = _LastMouseWorldPos - worldSpaceCoords;

			if (Input::IsMouseButtonPressed(OSR_MOUSE_BUTTON_MIDDLE) == true)
			{
				_CameraController->Translate({ worldSpaceDelta.x, worldSpaceDelta.y });
			}
			_LastMouseWorldPos = worldSpaceCoords;
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
				_SelectedGameObject->parent->RemoveChild(_SelectedGameObject->uid);
				_EventService->Publish(Events::EventType::SelectedGameObjectChanged, std::make_shared<Events::SelectedGameObjectChangedArgs>(nullptr));
			}
			break;
		}

		return true;
	}

	void SceneViewer::RenderGameObject(std::shared_ptr<GameObject> gameObject, Timestep ts, Renderer& renderer)
	{
		for (auto component : gameObject->components)
		{
			component->Render(ts, renderer);
		}

		for (auto child : gameObject->children)
		{
			RenderGameObject(child, ts, renderer);
		}
	}

	void SceneViewer::UpdateGameObject(std::shared_ptr<GameObject> gameObject, Timestep ts, bool updateTransform)
	{
		// check if the game has an invalid matrix as this will need to be propogated
		bool transformPropogation = gameObject->transform2D->IsMatrixValid();

		if (updateTransform == true)
			gameObject->transform2D->SetMatrixValid(false);

		gameObject->transform2D->Recalculate();
		for (auto& component : gameObject->components)
		{
			component->Recalculate();
		}

		for (auto& child : gameObject->children)
		{
			UpdateGameObject(child, ts, transformPropogation);
		}
	}

	std::shared_ptr<GameObject> SceneViewer::FindGameObjectInScene(glm::vec2 inputPosition)
	{
		/* Query each of the layer scene objects in reverse */
		for (std::vector<std::shared_ptr<Layer2D>>::reverse_iterator layer = _Scene->layers2D.rbegin(); layer != _Scene->layers2D.rend(); ++layer)
		{
			for (std::vector<std::shared_ptr<GameObject>>::reverse_iterator gameObject = (*layer)->children.rbegin(); gameObject != (*layer)->children.rend(); ++gameObject)
			{
				std::shared_ptr<GameObject> foundGameObject = FindGameObjectInGameObject(*gameObject, inputPosition);
				if (foundGameObject != nullptr)
				{
					return foundGameObject;
				}
			}
		}

		return nullptr;
	}

	std::shared_ptr<GameObject> SceneViewer::FindGameObjectInGameObject(std::shared_ptr<GameObject> gameObject, glm::vec2 inputPosition)
	{
		for (std::vector<std::shared_ptr<GameObject>>::reverse_iterator child = gameObject->children.rbegin(); child != gameObject->children.rend(); ++child)
		{
			if (FindGameObjectInGameObject(*child, inputPosition) != nullptr)
			{
				return *child;
			}
		}

		glm::mat4 mvpInverse = glm::inverse(_CameraController->GetCamera().GetViewProjectionMatrix() * gameObject->transform2D->matrix);
		glm::vec4 nearPoint = mvpInverse * glm::vec4(inputPosition, 0.0f, 1.0f);
		Rect inputArea = { { gameObject->inputArea.x, gameObject->inputArea.y }, { 0.0, 1.0 } };
		Rect translatedInputArea = _CameraController->GetCamera().GetViewProjectionMatrix() * inputArea;
		translatedInputArea.size.x = gameObject->inputArea.z;
		translatedInputArea.size.y = gameObject->inputArea.w;
		if (translatedInputArea.ContainsPoint({ nearPoint.x, nearPoint.y }) == true)
		{
			return gameObject;
		}


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