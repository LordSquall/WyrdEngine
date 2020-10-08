#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/pipeline/SpriteLayer.h>
#include <core/scene/Layer2D.h>
#include <core/scene/components/Transform2DComponent.h>

/* local include */
#include "SceneViewer.h"
#include "gizmos/TranslationGizmo.h"
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
		_SimulationService = ServiceManager::Get<SimulationService>(ServiceManager::Simulation);

		/* setup event bindings */
		_EventService->Subscribe(Events::EventType::SceneOpened, EVENT_FUNC(SceneViewer::OnSceneOpened));
		_EventService->Subscribe(Events::EventType::SelectedGameObjectChanged, EVENT_FUNC(SceneViewer::OnSelectedGameObjectChanged));

		/* initialise camera */
		_CameraController = std::make_shared<OrthographicCameraController>(1.0f, 1.0f);


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
		_GizmoShader->Unbind();

		/* initialise each of the gizmos */
		_TranslationGizmo.reset(new TranslationGizmo(_GizmoShader, _CameraController));

		/* create a new framebuffer */
		_Framebuffer.reset(Osiris::FrameBuffer::Create(FrameBufferConfig()));

		/* setup window config */
		config.windowPaddingX = 0.0f;
		config.windowPaddingY = 0.0f;

		/* Load any resources which are required to renderer (e.g. shaders) */
		std::ifstream vertexStreamGizmo("../../Osiris/res/shaders/sprite.vs");
		std::string vertexSrcGizmo((std::istreambuf_iterator<char>(vertexStreamGizmo)), std::istreambuf_iterator<char>());

		std::ifstream fragmentStreamGizmo("../../Osiris/res/shaders/sprite.fs");
		std::string fragmentSrcGizmo((std::istreambuf_iterator<char>(fragmentStreamGizmo)), std::istreambuf_iterator<char>());

		_Shader.reset(Shader::Create());
		_Shader->Build(vertexSrcGizmo, fragmentSrcGizmo);
		_Shader->Bind();
	}

	SceneViewer::~SceneViewer() {}

	void SceneViewer::OnUpdate(Timestep ts)
	{
		if (_Scene != nullptr)
		{
			/* Render Each sprite layer */
			for (auto sl : _Scene->layers2D)
			{
				for (auto go : sl->gameobjects)
				{
					if (go->transform2D->IsMatrixValid() == false)
					{
						go->transform2D->Recalculate();
					}
				}
			}
		}
	}

	void SceneViewer::OnRender(Timestep ts, Renderer& renderer)
	{
		_CameraController->OnUpdate(ts);

		_Shader->Bind();

		_Shader->SetVPMatrix(_CameraController->GetCamera().GetViewProjectionMatrix());

		_Framebuffer->Bind();
	
		renderer.Clear(0.1, 0.1, 0.1);

		if (_Scene != nullptr)
		{
			/* Render Each sprite layer */
			for (auto sl : _Scene->layers2D)
			{
				for (auto go : sl->gameobjects)
				{
					for (auto component : go->components)
					{
						component->Render(ts, renderer);
					}
				}
			}

			for (auto sl : _Scene->layers2D)
			{
				for (auto go : sl->gameobjects)
				{
					for (auto component : go->components)
					{
						if (component->debugOverlayFunction)
						{
							component->debugOverlayFunction(_CameraController->GetCamera().GetViewProjectionMatrix());
						}
					}
				}
			}

			if (_SelectedGameObject != NULL)
			{
				_TranslationGizmo->SetGameObject(_SelectedGameObject);
				_TranslationGizmo->Render(ts, renderer);
			}
		}

		_Framebuffer->Unbind();
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
			_Scene->cameraZoom = _CameraController->GetZoomLevel();
			_Scene->cameraPosition = _CameraController->GetPosition();
		}
	}

	void SceneViewer::OnEditorRender()
	{
		static bool showStats = false;

		/* build the top toolbar */
		ImGui::Checkbox("show stats", &showStats);

		/* calculate the mouse offset for events */
		_mouseOffset = { ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y };

		/* calculate the viewport size */
		_Viewport = { { 0.0f, 0.0f }, { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y - ImGui::GetCursorPos().y } };

		ImGui::Image((ImTextureID)_Framebuffer->GetColorAttachmentID(), ImVec2(_Viewport.size.x, _Viewport.size.y), ImVec2(0, 1), ImVec2(1, 0));

		if (showStats == true)
		{
			ImGui::SetCursorPosY(42.0f);
			ImGui::Text("Camera:");
			ImGui::Text("\tAspect Ratio [%f]", _CameraController->GetAspectRatio());
			ImGui::Text("\tZoom Level	[%f]", _CameraController->GetZoomLevel());
			ImGui::Text("\tPosition		[%f, %f, %f]", _CameraController->GetPosition().x, _CameraController->GetPosition().y, _CameraController->GetPosition().z);

			ImGui::Text("Window:");
			ImGui::Text("\tAspect Ratio [%f]", _Boundary.size.x / _Boundary.size.y);

			ImGui::Text("Viewport:");
			ImGui::Text("X [%f]", _Viewport.size.x);
			ImGui::Text("Y [%f]", _Viewport.size.y);
			ImGui::Text("W [%f]", _Viewport.position.x);
			ImGui::Text("H [%f]", _Viewport.position.y);

			ImGui::Text("FrameBuffer:");
			ImGui::Text("Width [%d]", _Framebuffer->GetConfig().width);
			ImGui::Text("Height [%d]", _Framebuffer->GetConfig().height);

			ImGui::Text("Cursor:");
			ImGui::Text("Viewport Offset Coords:   [%d, %d]", (int32_t)_mouseOffset.x, (int32_t)_mouseOffset.y);
			ImGui::Text("Evt Start Coords: [%d, %d]", (int32_t)_LastMousePos.x, (int32_t)_LastMousePos.y);
		}

		/* Context Menu */
		if (_OpenContextMenu == true)
		{
			ImGui::SetNextWindowPos({ ImVec2(_MenuPos.x, _MenuPos.y) });
			ImGui::Begin("Test Window", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

			if (ImGui::MenuItem("Copy", 0, false, false) == false) { /* TODO */ }
			if (ImGui::MenuItem("Paste", 0, false, false) == false) { /* TODO */ }
			if (ImGui::MenuItem("Cut", 0, false, false) == false) { /* TODO */ }
			ImGui::Separator();
			if (ImGui::MenuItem("Delete", 0, false, true) == false) { /* TODO */ }

			ImGui::End();
		}
	}

	void SceneViewer::OnResize()
	{
		_Framebuffer->Resize(_Viewport.size.x, _Viewport.size.y);
		_CameraController->UpdateProjection(_Viewport.size.x / _Viewport.size.y);
	}

	bool SceneViewer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		bool itemFound = false;
		glm::vec2 normalisedMouseCoords;
		std::shared_ptr<GameObject> selectedGameObject = nullptr;
		glm::vec2 mouseCoords = glm::vec2(e.GetPositionX(), e.GetPositionY()) - _mouseOffset;


		if (_Viewport.ContainsPoint(mouseCoords) == true)
		{
			normalisedMouseCoords.x = (2.0f * (mouseCoords.x) / (_Viewport.size.x)) - 1.0f;
			normalisedMouseCoords.y = -((2.0f * (mouseCoords.y) / (_Viewport.size.y)) - 1.0f);

			if (_Scene != nullptr)
			{
				/* Query each of the layer scene objects */
				for (auto& sl : _Scene->layers2D)
				{
					for (auto& go : sl->gameobjects)
					{
						glm::mat4 mvpInverse = glm::inverse(_CameraController->GetCamera().GetViewProjectionMatrix() * go->transform2D->matrix);

						glm::vec4 nearPoint = mvpInverse * glm::vec4(normalisedMouseCoords, 0.0f, 1.0f);

						Rect inputArea = { { go->inputArea.x, go->inputArea.y }, { 0.0, 1.0 } };
						Rect translatedInputArea = _CameraController->GetCamera().GetViewProjectionMatrix() * inputArea;
						translatedInputArea.size.x = go->inputArea.z;
						translatedInputArea.size.y = go->inputArea.w;

						if (translatedInputArea.ContainsPoint({ nearPoint.x, nearPoint.y }) == true)
						{
							selectedGameObject = go;
							itemFound = true;
						}
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
		}

		_LastMousePos = { mouseCoords.x, mouseCoords.y };

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
		/* calculate the mouse positions and deltas */
		glm::vec2 mouseCoords = glm::vec2(e.GetX(), e.GetY()) - _mouseOffset;

		if (_Viewport.ContainsPoint(mouseCoords) == true)
		{

			glm::vec2 mouseDelta = _LastMousePos - mouseCoords;
			glm::vec2 unitDelta = { mouseDelta.x * (1.0f / (_Boundary.size.x * 0.5f)),  -(mouseDelta.y * (1.0f / ((_Boundary.size.y * 0.5f) * _CameraController->GetAspectRatio()))) };

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
		}

		_LastMousePos = mouseCoords;

		return true;
	}

	bool SceneViewer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
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
		_SimulationService->SetInputState(e.GetKeyCode(), 2);

		return true;
	}

	void SceneViewer::OnSceneOpened(Events::EventArgs& args)
	{
		Events::SceneOpenedArgs& evtArgs = static_cast<Events::SceneOpenedArgs&>(args);

		_Scene = evtArgs.scene;

		/* Set camera settings */
		_CameraController->SetZoomLevel(_Scene->cameraZoom);
		_CameraController->SetPosition(_Scene->cameraPosition);
	}

	void SceneViewer::OnSelectedGameObjectChanged(Events::EventArgs& args)
	{
		Events::SelectedGameObjectChangedArgs& evtArgs = static_cast<Events::SelectedGameObjectChangedArgs&>(args);

		_SelectedGameObject = evtArgs.gameObject;
	}
}