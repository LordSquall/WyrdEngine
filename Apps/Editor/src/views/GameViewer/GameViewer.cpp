#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/Input.h>
#include <core/KeyCodes.h>
#include <core/MouseCodes.h>
#include <core/scene/Layer2D.h>
#include <core/scene/SceneLayer.h>
#include <core/scene/components/Transform2DComponent.h>
#include <core/scene/components/CameraComponent.h>

/* local include */
#include "GameViewer.h"
#include "gizmos/TranslationGizmo.h"
#include "gizmos/GridGizmo.h"
#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	GameViewer::GameViewer(EditorLayer* editorLayer) : EditorViewBase("Game Viewer", editorLayer), _CameraComponent(nullptr)
	{
		/* retrieve services */
		_WorkspaceService = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace);
		_EventService = ServiceManager::Get<EventService>(ServiceManager::Events);
		_ResourceService = ServiceManager::Get<ResourceService>(ServiceManager::Resources);
		_SettingsService = ServiceManager::Get<SettingsService>(ServiceManager::Settings);
		_SimulationService = ServiceManager::Get<SimulationService>(ServiceManager::Simulation);

		/* setup event bindings */
		_EventService->Subscribe(Events::EventType::SceneOpened, EVENT_FUNC(GameViewer::OnSceneOpened));
		_EventService->Subscribe(Events::EventType::SetSceneCamera, [this](Events::EventArgs& args) {
			Events::SetSceneCameraArgs& a = (Events::SetSceneCameraArgs&)args;
			_CameraComponent = a.cameraComponent;
		});

		/* create a new framebuffer */
		_Framebuffer.reset(Wyrd::FrameBuffer::Create(FrameBufferConfig()));

		/* setup window config */
		config.windowPaddingX = 0.0f;
		config.windowPaddingY = 0.0f;
	}

	GameViewer::~GameViewer()
	{
		WYRD_TRACE("SceneViewer::~");
	}

	void GameViewer::OnUpdate(Timestep ts)
	{
		if (_CameraComponent != nullptr)
		{
			/* at the moment only functional for 2D transforms */
			Transform2DComponent* transform2D = dynamic_cast<Transform2DComponent*>(_CameraComponent->Owner->transform.get());
			if (transform2D != nullptr)
			{
				_CameraComponent->GetCamera().SetSize(_CameraComponent->GetSize());
				_CameraComponent->GetCamera().SetPosition(vec3(transform2D->position, 0.0f));

				/* Set the camera viewport */
				_CameraComponent->GetCamera().SetViewportSize(800.0f, 600.0f);
			}
		}
	}

	void GameViewer::OnRender(Timestep ts, Renderer& renderer)
	{
		if (_Framebuffer->GetConfig().height > 0 && _Framebuffer->GetConfig().width > 0)
		{
			if (_CameraComponent != nullptr)
			{
				_CameraComponent->GetCamera().RecalulateViewProjection();

				_Framebuffer->Bind();

				renderer.Clear(0.1f, 0.1f, 0.1f);

				if (_Scene != nullptr)
				{
					/* Render each of the sprite layers */
					for (auto& sl : _Scene->GetLayers())
					{
						if (sl->IsVisible())
							sl->Render(renderer, _CameraComponent->GetCamera().GetViewProjectionMatrix());
					}
				}

				_Framebuffer->Unbind();
			}
		}
	}

	void GameViewer::OnEditorRender()
	{
		static bool showStats = false;
		static bool showGizmoSettings = false;

		/* calculate the mouse offset for events */
		_mouseOffset = { ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y };

		/* calculate the viewport boundary */
		_ViewportBoundary = { {_Boundary.position.x - (_Boundary.size.x - _Viewport.size.x), _Boundary.position.y + (_Boundary.size.y - _Viewport.size.y) }, { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y - ImGui::GetCursorPos().y } };

		/* calculate the viewport size */
		_Viewport = { { 0.0f, 0.0f }, { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y - ImGui::GetCursorPos().y } };

		if (_CameraComponent != nullptr)
		{

			ImGui::Image((ImTextureID)(UINT_PTR)_Framebuffer->GetColorAttachmentID(), ImVec2(_Viewport.size.x, _Viewport.size.y), ImVec2(0, 1), ImVec2(1, 0));
		}
		else
		{
			ImGui::Text("No Scene Primary Camera Selected");
		}
	}

	void GameViewer::OnResize()
	{
		/* resize the underlying framebuffer */
		_Framebuffer->Resize((uint32_t)_Viewport.size.x, (uint32_t)_Viewport.size.y);

		if (_CameraComponent != nullptr)
		{
			/* Set the camera viewport */
			_CameraComponent->GetCamera().SetViewportSize(_Viewport.size.x, _Viewport.size.y);
		}
	}

	void GameViewer::RenderGameObject(std::unique_ptr<GameObject>& gameObject, Timestep ts, Renderer& renderer)
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

	void GameViewer::OnSceneOpened(Events::EventArgs& args)
	{
		Events::SceneOpenedArgs& evtArgs = static_cast<Events::SceneOpenedArgs&>(args);

		_Scene = evtArgs.scene;

		/* Get the current active camera */
		_CameraComponent = _Scene->GetPrimaryCamera();

		if (_CameraComponent != nullptr)
		{
			/* at the moment only functional for 2D transforms */
			Transform2DComponent* transform2D = dynamic_cast<Transform2DComponent*>(_CameraComponent->Owner->transform.get());
			if (transform2D != nullptr)
			{
				_CameraComponent->GetCamera().SetSize(_CameraComponent->GetSize());
				_CameraComponent->GetCamera().SetPosition(vec3(transform2D->position, 0.0f));

				/* Set the camera viewport */
				_CameraComponent->GetCamera().SetViewportSize(_Viewport.size.x, _Viewport.size.y);
			}
		}
	}
}