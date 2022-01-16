#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/Input.h>
#include <core/KeyCodes.h>
#include <core/MouseCodes.h>
#include <core/ecs/ECS.h>
#include <core/ecs/EntitySet.h>

/* local include */
#include "SceneViewer.h"
#include "gizmos/2D/Translation2DGizmo.h"
#include "gizmos/GridGizmo.h"
#include "support/ImGuiUtils.h"
#include "datamodels/EditorComponents.h"

/* external includes */
#include <glm/glm.hpp>
#include <imgui.h>

namespace Wyrd::Editor
{
	SceneViewer::SceneViewer(EditorLayer* editorLayer) : EditorViewBase("Scene Viewer", editorLayer), _SelectedEntity(ENTITY_INVALID)
	{
		/* retrieve services */
		_WorkspaceService = ServiceManager::Get<WorkspaceService>();
		_EventService = ServiceManager::Get<EventService>();
		_ResourceService = ServiceManager::Get<ResourceService>();
		_DialogService = ServiceManager::Get<DialogService>();
		_SettingsService = ServiceManager::Get<SettingsService>();
		_SimulationService = ServiceManager::Get<SimulationService>();
		_CoreSystemService = ServiceManager::Get<CoreSystemsService>();

		/* setup event bindings */
		_EventService->Subscribe(Events::EventType::SceneOpened, EVENT_FUNC(SceneViewer::OnSceneOpened));
		_EventService->Subscribe(Events::EventType::SelectedEntityChanged, EVENT_FUNC(SceneViewer::OnSelectedEntityChanged));

		/* initialise camera */
		_CameraController = std::make_shared<OrthographicCameraController>();

		/* initialise each of the gizmos */
		_GridGizmo = std::make_unique<GridGizmo>(this);
		_Translation2DGizmo = std::make_unique<Translation2DGizmo>(this);

		/* create a new framebuffer */
		_Framebuffer.reset(Wyrd::FrameBuffer::Create(FrameBufferConfig()));

		/* setup window config */
		config.windowPaddingX = 0.0f;
		config.windowPaddingY = 0.0f;
	}

	SceneViewer::~SceneViewer() 
	{
		WYRD_TRACE("SceneViewer::~");
	}

	void SceneViewer::OnUpdate(Timestep ts)
	{
		if (_Scene != nullptr)
		{
			/* we want to process all the entities */
			for (Entity e : EntitySet(*_Scene.get()))
			{
				EditorComponent* editorComponent = _Scene->Get<EditorComponent>(e);
				if (editorComponent)
				{

					Transform2DComponent* transform = _Scene->Get<Transform2DComponent>(e);
					SpriteComponent* sprite = _Scene->Get<SpriteComponent>(e);
					TextComponent* text = _Scene->Get<TextComponent>(e);

					Rect r;

					if (sprite)
					{
						// calculate aabb sprite component render rect

						Rect newRect;
						newRect._position = sprite->position + transform->position;
						newRect._size = sprite->size;
						r = newRect;
					}

					if (text)
					{
						// calculate aabb text component render rect
					}

					if (editorComponent)
					{
						editorComponent->inputArea._position = r._position;
						editorComponent->inputArea._size = r._size;
					}
				}
			}
		}
	}

	void SceneViewer::OnRender(Timestep ts, Renderer& renderer)
	{
#ifdef WYRD_INCLUDE_DEBUG_TAGS
		renderer.StartNamedSection("SceneViewer.Scene");
#endif

		if (_Framebuffer->GetConfig().height > 0 && _Framebuffer->GetConfig().width > 0)
		{
			_CameraController->OnUpdate(ts);

			_Framebuffer->Bind();

			renderer.Clear(0.1f, 0.1f, 0.1f);

			_GridGizmo->Render(ts, renderer);

			if (_Scene != nullptr)
			{
				for (Entity e : EntitySet<Transform2DComponent, SpriteComponent>(*_Scene.get()))
				{
					Transform2DComponent* transform = _Scene->Get<Transform2DComponent>(e);
					SpriteComponent* sprite = _Scene->Get<SpriteComponent>(e);

					Wyrd::DrawSpriteCommand cmd{};
					cmd.position = sprite->position + transform->position;
					cmd.size = sprite->size;
					cmd.rotation = transform->rotation;
					cmd.rotationOrigin = transform->rotationOrigin;
					cmd.tiling = sprite->tiling;
					cmd.vpMatrix = _CameraController->GetCamera().GetViewProjectionMatrix();
					cmd.shader = Application::Get().GetResources().Shaders["Sprite"].get();
					cmd.texture = Application::Get().GetResources().Textures[sprite->texture].get();
					cmd.color = sprite->color;

					renderer.Submit(cmd);
				}

				for (Entity e : EntitySet<Transform2DComponent, TextComponent>(*_Scene.get()))
				{
					Transform2DComponent* transform = _Scene->Get<Transform2DComponent>(e);
					TextComponent* text = _Scene->Get<TextComponent>(e);

					Wyrd::DrawTextCommand cmd{};
					cmd.position = transform->position;
					cmd.vpMatrix = _CameraController->GetCamera().GetViewProjectionMatrix();
					cmd.shader = Application::Get().GetResources().Shaders["Text"].get();
					cmd.content = text->content;
					cmd.scale = 1.0f;
					cmd.size = text->size;
					cmd.font = Application::Get().GetResources().FontTypes[text->font].get();
					cmd.color = text->color;

					renderer.Submit(cmd);
				}

			}

			/* Gizmos */
			if (_SelectedEntity != ENTITY_INVALID)
			{
				CameraComponent* cameraComp = _Scene->Get<CameraComponent>(_SelectedEntity);
				if (cameraComp != nullptr)
				{
					Transform2DComponent* transform = _Scene->Get<Transform2DComponent>(_SelectedEntity);

					Wyrd::DrawRectCommand cmd{};
					cmd.position = { transform->position.x + (-((cameraComp->size * cameraComp->aspectRatio) / 2.0f)), transform->position.y + (-(cameraComp->size) / 2.0f) };
					cmd.size = { cameraComp->size * cameraComp->aspectRatio, cameraComp->size };
					cmd.thickness = 2.0f;
					cmd.vpMatrix = _CameraController->GetCamera().GetViewProjectionMatrix();
					cmd.shader = Application::Get().GetResources().Shaders["Vertex2D"].get();
					cmd.color = { 1.0f, 0.0f, 0.0f, 1.0f };

					renderer.Submit(cmd);
				}

				_Translation2DGizmo->Render(ts, renderer);
			}

			renderer.Flush();

			_Framebuffer->Unbind();
		}

#ifdef WYRD_INCLUDE_DEBUG_TAGS
		renderer.EndNamedSection();
#endif
	}

	void SceneViewer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(WYRD_BIND_EVENT_FN(SceneViewer::OnMouseButtonPressedEvent), nullptr);
		dispatcher.Dispatch<MouseButtonReleasedEvent>(WYRD_BIND_EVENT_FN(SceneViewer::OnMouseButtonReleasedEvent), nullptr);
		dispatcher.Dispatch<MouseScrolledEvent>(WYRD_BIND_EVENT_FN(SceneViewer::OnMouseScrolledEvent), nullptr);
		dispatcher.Dispatch<MouseMovedEvent>(WYRD_BIND_EVENT_FN(SceneViewer::OnMouseMovedEvent), nullptr);
		dispatcher.Dispatch<KeyPressedEvent>(WYRD_BIND_EVENT_FN(SceneViewer::OnKeyPressedEvent), nullptr);
		dispatcher.Dispatch<KeyReleasedEvent>(WYRD_BIND_EVENT_FN(SceneViewer::OnKeyReleasedEvent), nullptr);

		_Translation2DGizmo->OnEvent(event);

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
		_ViewportBoundary._position.x = _Boundary._position.x - (_Boundary._size.x - _Viewport._size.x);
		_ViewportBoundary._position.y = _Boundary._position.y + (_Boundary._size.y - _Viewport._size.y);
		_ViewportBoundary._size.x = ImGui::GetWindowSize().x;
		_ViewportBoundary._size.y = ImGui::GetWindowSize().y - ImGui::GetCursorPos().y;

		/* calculate the viewport size */
		_Viewport._position.x = 0.0f;
		_Viewport._position.y = 0.0f;
		_Viewport._size.x = ImGui::GetWindowSize().x;
		_Viewport._size.y = ImGui::GetWindowSize().y - ImGui::GetCursorPos().y;

		ImGui::Image((ImTextureID)(UINT_PTR)_Framebuffer->GetColorAttachmentID(), ImVec2(_Viewport._size.x, _Viewport._size.y), ImVec2(0, 1), ImVec2(1, 0));

		if (showStats == true)
		{
			ImGui::SetCursorPosY(42.0f);
			ImGui::Text("Camera:");
			ImGui::Text("\tPosition		[%f, %f, %f]", _CameraController->GetPosition().x, _CameraController->GetPosition().y, _CameraController->GetPosition().z);

			ImGui::Text("Window:");
			ImGui::Text("\tAspect Ratio [%f]", _ViewportBoundary._size.x / _ViewportBoundary._size.y);

			ImGui::Text("FrameBuffer:");
			ImGui::Text("Width [%d]", _Framebuffer->GetConfig().width);
			ImGui::Text("Height [%d]", _Framebuffer->GetConfig().height);

			ImGui::Text("Cursor:");
			ImGui::Text("Screen Position: [%d, %d]", (int32_t)_mouseOffset.x, (int32_t)_mouseOffset.y);
			ImGui::Text("Viewport Offset Coords:   [%d, %d]", (int32_t)_mouseOffset.x, (int32_t)_mouseOffset.y);
			ImGui::Text("Evt Start Coords: [%d, %d]", (int32_t)_LastMousePos.x, (int32_t)_LastMousePos.y);
		}
	}

	void SceneViewer::OnResize()
	{
		/* resize the underlying framebuffer */
		_Framebuffer->Resize((uint32_t)_Viewport._size.x, (uint32_t)_Viewport._size.y);

		/* Set the camera viewport */
		_CameraController->SetViewportSize(_Viewport._size.x, _Viewport._size.y);
	}

	glm::vec2 SceneViewer::Convert2DToWorldSpace(const glm::vec2& point)
	{
		glm::vec2 ndcSpace = GetScreenSpaceFromWorldPoint(point);

		glm::vec4 clipSpace = { ndcSpace.x, ndcSpace.y, -1.0, 1.0f };

		glm::vec4 eyeSpace = glm::inverse(_CameraController->GetCamera().GetProjectionMatrix()) * clipSpace;

		glm::vec4 worldSpace = glm::inverse(_CameraController->GetCamera().GetViewMatrix()) * eyeSpace;

		return { worldSpace.x, worldSpace.y };
	}

	glm::vec2 SceneViewer::GetViewportSpaceFromPoint(const glm::vec2& point)
	{
		return point - _ViewportBoundary._position;
	}

	glm::vec2 SceneViewer::GetWorldSpaceFromPoint(const glm::vec2& point)
	{
		glm::vec2 viewportMouseCoords = point - _ViewportBoundary._position;

		return _CameraController->GetCamera().GetWorldSpaceFromPoint(viewportMouseCoords, _ViewportBoundary);
	}

	glm::vec2 SceneViewer::GetScreenSpaceFromWorldPoint(const glm::vec2& point)
	{
		glm::vec2 viewportMouseCoords = point - _ViewportBoundary._position;

		return _CameraController->GetCamera().GetNDCFromPoint(viewportMouseCoords, _ViewportBoundary);
	}

	bool SceneViewer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e, void* data)
	{
		_SimulationService->SetMouseButtonState(e.GetMouseButton(), true);

		/* build the initial mouse position vector */
		glm::vec2 mousePos = { e.GetPositionX(), e.GetPositionY() };

		/* viewport mouse position */
		glm::vec2 viewportPos = GetViewportSpaceFromPoint(mousePos);

		/* we only want to process mouse events within the viewport of the scene */
		if (_Viewport.ContainsPoint(viewportPos) == true)
		{
			glm::vec2 worldSpace = Convert2DToWorldSpace(mousePos);		
			if (_Scene != nullptr)
			{
				/* test each of the editor components to check input */
				for (Entity e : EntitySet<EditorComponent>(*_Scene.get()))
				{
					EditorComponent* editorComponent = _Scene->Get<EditorComponent>(e);

					if (editorComponent->inputArea.ContainsPoint(worldSpace) == true)
					{
						_EventService->Publish(Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(e));
					}
				}
			}
		}

		return true;
	}

	bool SceneViewer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e, void* data)
	{

		_SimulationService->SetMouseButtonState(e.GetMouseButton(), false);
		return true;
	}

	bool SceneViewer::OnMouseScrolledEvent(MouseScrolledEvent& e, void* data)
	{
		_CameraController->OnEvent(e);

		return true;
	}

	bool SceneViewer::OnMouseMovedEvent(MouseMovedEvent& e, void* data)
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

	bool SceneViewer::OnKeyPressedEvent(KeyPressedEvent& e, void* data)
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

	bool SceneViewer::OnKeyReleasedEvent(KeyReleasedEvent& e, void* data)
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
			
			break;
		}

		return true;
	}

	void SceneViewer::OnSceneOpened(Events::EventArgs& args)
	{
		Events::SceneOpenedArgs& evtArgs = static_cast<Events::SceneOpenedArgs&>(args);

		_Scene = evtArgs.scene;

		/* Set camera settings */
		_CameraController->SetSize(_Scene->cameraZoom);
		_CameraController->SetPosition(_Scene->cameraPosition);
	}

	void SceneViewer::OnSelectedEntityChanged(Events::EventArgs& args)
	{
		Events::SelectedEntityChangedArgs& evtArgs = static_cast<Events::SelectedEntityChangedArgs&>(args);
		_SelectedEntity = evtArgs.entity;
	}
}