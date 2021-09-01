#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/ecs/ECS.h>
#include <core/ecs/EntitySet.h>
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
	GameViewer::GameViewer(EditorLayer* editorLayer) : EditorViewBase("Game Viewer", editorLayer), _CameraEntity(ENTITY_INVALID), _CameraComponent(nullptr), _SizeConfigID(1)
	{
		/* retrieve services */
		_WorkspaceService = ServiceManager::Get<WorkspaceService>();
		_EventService = ServiceManager::Get<EventService>();
		_ResourceService = ServiceManager::Get<ResourceService>();
		_SettingsService = ServiceManager::Get<SettingsService>();
		_SimulationService = ServiceManager::Get<SimulationService>();

		/* setup event bindings */
		_EventService->Subscribe(Events::EventType::SceneOpened, EVENT_FUNC(GameViewer::OnSceneOpened));
		_EventService->Subscribe(Events::EventType::SetSceneCamera, [this](Events::EventArgs& args) {
			Events::SetSceneCameraArgs& a = (Events::SetSceneCameraArgs&)args;
			_CameraEntity = a.entity;
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
		if (_CameraEntity != ENTITY_INVALID)
		{
			Transform2DComponent* transform = _Scene->Get<Transform2DComponent>(_CameraEntity);

			_Camera.SetPosition({ transform->position, 0.0f });
			_Camera.SetSize(_CameraComponent->size);

			switch (_SizeConfigID)
			{
			case 0:
				_Camera.SetViewportSize(_WorkspaceService->GetCurrentProject()->GetExportSettings().width, _WorkspaceService->GetCurrentProject()->GetExportSettings().height);
				break;
			case 1:
				_Camera.SetViewportSize(_Viewport.size.x, _Viewport.size.y);
				break;
			case 2:
				_Camera.SetViewportSize(800.0f, 600.0f);
				break;

			}

			_Camera.RecalulateViewProjection();
		}
	}

	void GameViewer::OnRender(Timestep ts, Renderer& renderer)
	{
		if (_Framebuffer->GetConfig().height > 0 && _Framebuffer->GetConfig().width > 0)
		{
			_Framebuffer->Bind();

			renderer.Clear(0.1f, 0.1f, 0.1f);
			if (_Scene != nullptr)
			{
				for (Entity e : EntitySet<Transform2DComponent, SpriteComponent>(*_Scene.get()))
				{
					Transform2DComponent* transform = _Scene->Get<Transform2DComponent>(e);
					SpriteComponent* sprite = _Scene->Get<SpriteComponent>(e);
				
					Wyrd::DrawSpriteCommand cmd{};
					cmd.type = 1;
					cmd.position = sprite->position + transform->position;
					cmd.size = sprite->size;
					cmd.tiling = sprite->tiling;
					cmd.vpMatrix = _Camera.GetViewProjectionMatrix();
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
					cmd.type = 1;
					cmd.position = transform->position;
					cmd.vpMatrix = _Camera.GetViewProjectionMatrix();
					cmd.shader = Application::Get().GetResources().Shaders["Text"].get();
					cmd.content = text->content;
					cmd.scale = 1.0f;
					cmd.size = text->size;
					cmd.font = Application::Get().GetResources().FontTypes[text->font].get();
					cmd.color = text->color;

					renderer.Submit(cmd);
				}

				renderer.Flush();
			}

			_Framebuffer->Unbind();
		}
	}

	void GameViewer::OnEditorRender()
	{
		static bool showStats = false;
		static bool showGizmoSettings = false;

		/* calculate the viewport size */
		_Viewport.position.x = 0.0f;
		_Viewport.position.y = 0.0f;
		_Viewport.size.x = ImGui::GetContentRegionAvail().x;
		_Viewport.size.y = ImGui::GetContentRegionAvail().y - ImGui::GetCursorPos().y;


		const char* items[] = { "Export Size", "Window Size", "800x600" };
		if (ImGui::Combo("combo", &_SizeConfigID, items, IM_ARRAYSIZE(items)))
		{
			switch (_SizeConfigID)
			{
			case 0:
				_Framebuffer->Resize((uint32_t)_WorkspaceService->GetCurrentProject()->GetExportSettings().width, (uint32_t)_WorkspaceService->GetCurrentProject()->GetExportSettings().height);
				break;
			case 1:
				_Framebuffer->Resize((uint32_t)_Viewport.size.x, (uint32_t)_Viewport.size.y);
				break;
			case 2:
				_Framebuffer->Resize(800, 600);
				break;
			}
		}

		if (_CameraComponent != nullptr)
		{
			_CameraComponent->viewport = _Viewport;
			_CameraComponent->aspectRatio = _Camera.GetAspectRatio();

			auto pos = ImGui::GetCursorPos();
			ImGui::Image((ImTextureID)(UINT_PTR)_Framebuffer->GetColorAttachmentID(), ImVec2(_Viewport.size.x, _Viewport.size.y), ImVec2(0, 1), ImVec2(1, 0));

			ImGui::SetCursorPos(pos);
			ImGui::Text("X : %f", _Viewport.position.x);
			ImGui::Text("Y : %f", _Viewport.position.y);
			ImGui::Text("W : %f", _Viewport.size.x);
			ImGui::Text("H : %f", _Viewport.size.y);
			ImGui::Text("Aspect Ratio : %f", _CameraComponent->aspectRatio);
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
			_Camera.SetSize(_CameraComponent->size);
			_Camera.SetViewportSize(_Viewport.size.x, _Viewport.size.y);
		}
	}

	void GameViewer::OnSceneOpened(Events::EventArgs& args)
	{
		Events::SceneOpenedArgs& evtArgs = static_cast<Events::SceneOpenedArgs&>(args);
		_Scene = evtArgs.scene;

		/* check to see if the scene already has a camera component on the camera entity */
		if (_Scene->GetPrimaryCameraEntity() != ENTITY_INVALID)
		{
			CameraComponent* cameraComponent = _Scene->Get<CameraComponent>(_Scene->GetPrimaryCameraEntity());
			if (cameraComponent != nullptr)
			{
				_CameraEntity = _Scene->GetPrimaryCameraEntity();
				_CameraComponent = cameraComponent;
			}
			else
			{
				_CameraEntity = ENTITY_INVALID;
				_CameraComponent = nullptr;
			}
		}
		else
		{
			_CameraEntity = ENTITY_INVALID;
			_CameraComponent = nullptr;
		}
	}
}