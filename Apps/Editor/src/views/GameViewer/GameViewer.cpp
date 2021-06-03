#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Layer.h>
#include <core/Input.h>
#include <core/KeyCodes.h>
#include <core/MouseCodes.h>

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

	}

	void GameViewer::OnRender(Timestep ts, Renderer& renderer)
	{
		
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
			//_CameraComponent->GetCamera().SetViewportSize(_Viewport.size.x, _Viewport.size.y);
		}
	}

	void GameViewer::OnSceneOpened(Events::EventArgs& args)
	{
		Events::SceneOpenedArgs& evtArgs = static_cast<Events::SceneOpenedArgs&>(args);

		_Scene = evtArgs.scene;
	}
}