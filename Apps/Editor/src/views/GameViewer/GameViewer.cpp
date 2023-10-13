/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/ecs/ECS.h>
#include <core/ecs/EntitySet.h>
#include <core/Maths.h>
#include <core/pipeline/Camera.h>
#include <core/Layer.h>
#include <core/Input.h>
#include <core/KeyCodes.h>
#include <core/MouseCodes.h>

/* local include */
#include "GameViewer.h"
#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	GameViewer::GameViewer(EditorLayer* editorLayer) : EditorViewBase("Game Viewer", editorLayer), _CameraEntity(ENTITY_INVALID), _SizeConfigID(1)
	{
		/* retrieve services */
		_Workspace  = ServiceManager::Get<WorkspaceService>();
		_Events = ServiceManager::Get<EventService>();
		_Resources = ServiceManager::Get<ResourceService>();
		_Settings = ServiceManager::Get<SettingsService>();
		_Simulation = ServiceManager::Get<SimulationService>();

		/* setup event bindings */
		_Events->Subscribe(Events::EventType::SceneOpened, WYRD_BIND_FN(GameViewer::OnSceneOpened));
		_Events->Subscribe(Events::EventType::SetSceneCamera, [this](Events::EventArgs& args) {
			Events::SetSceneCameraArgs& a = (Events::SetSceneCameraArgs&)args;
			_CameraEntity = a.entity;
		});

		/* create a new framebuffer */
		_Framebuffer.reset(Wyrd::FrameBuffer::Create(FrameBufferConfig()));

		/* setup window config */
		config.windowPaddingX = 0.0f;
		config.windowPaddingY = 0.0f;

		_Camera = std::make_shared<Camera>();
	}

	GameViewer::~GameViewer()
	{
		WYRD_TRACE("SceneViewer::~");
	}

	void GameViewer::OnUpdate(Timestep ts)
	{
		if (_CameraEntity != ENTITY_INVALID)
		{
			Transform3DComponent* transform = _Scene->Get<Transform3DComponent>(_CameraEntity);
			CameraComponent* camera = _Scene->Get<CameraComponent>(_CameraEntity);
			RelationshipComponent* relationshipComponent = _Scene->Get<RelationshipComponent>(_CameraEntity);

			if (transform == nullptr || camera == nullptr)
			{
				_CameraEntity = ENTITY_INVALID;
			}
			else
			{
				glm::vec3 translation = { transform->position.x, transform->position.y, transform->position.z };
				glm::vec3 rotation;
				glm::vec3 scale;

				if (relationshipComponent != nullptr && relationshipComponent->parent != ENTITY_INVALID)
				{
					Transform3DComponent* parentTransform3DComponent = _Scene->Get<Transform3DComponent>(relationshipComponent->parent);

					if (parentTransform3DComponent != nullptr)
					{
						glm::vec3 parentTranslation;
						glm::vec3 parentRotation;
						glm::vec3 parentScale;
						Maths::DecomposeTransform(parentTransform3DComponent->modelMatrix, parentTranslation, rotation, scale);

						translation += parentTranslation;
					}
				}

				_Camera->SetPosition(translation);
				_Camera->SetYaw(-DEG_TO_RAD(transform->rotation.y));
				_Camera->SetPitch(DEG_TO_RAD(transform->rotation.x));
				_Camera->SetMode(camera->projection == 0 ? Camera::Mode::Perspective : Camera::Mode::Orthographic);
				_Camera->perspectiveSettings.nearPlane = camera->nearPlane;
				_Camera->perspectiveSettings.farPlane = camera->farPlane;
				_Camera->perspectiveSettings.aspect = camera->aspectRatio;

				_Camera->orthoSettings.top = camera->top;
				_Camera->orthoSettings.bottom = camera->bottom;
				_Camera->orthoSettings.left = camera->left;
				_Camera->orthoSettings.right = camera->right;
				_Camera->orthoSettings.nearPlane = camera->nearPlane;
				_Camera->orthoSettings.farPlane = camera->farPlane;

				_Camera->Update();
			}
		}
	}

	void GameViewer::OnRender(Timestep ts, Renderer& renderer)
	{
#ifdef WYRD_INCLUDE_DEBUG_TAGS
		renderer.StartNamedSection("GameViewer.Scene");
#endif

		if (_CameraEntity != ENTITY_INVALID)
		{
			if (_Framebuffer->GetConfig().height > 0 && _Framebuffer->GetConfig().width > 0)
			{
				_Framebuffer->Bind();

				renderer.Clear(0.1f, 0.1f, 0.1f);
				if (_Scene != nullptr)
				{
					for (Entity e : EntitySet<Transform3DComponent, MeshRendererComponent, MaterialComponent>(*_Scene.get()))
					{
						Transform3DComponent* transform = _Scene->Get<Transform3DComponent>(e);
						MeshRendererComponent* meshRenderer = _Scene->Get<MeshRendererComponent>(e);
						MaterialComponent* material = _Scene->Get<MaterialComponent>(e);

						Wyrd::DrawMeshCommand cmd{};
						cmd.modelMatrix = transform->modelMatrix;
						cmd.viewMatrix = _Camera->GetViewMatrix();
						cmd.projectionMatrix = _Camera->GetProjectionMatrix();
						cmd.material = Application::Get().GetResources().Materials[material->material].get();
						cmd.materialProps = &material->properties;
						cmd.mesh = Application::Get().GetResources().Meshs[meshRenderer->model].get();
						cmd.baseTexture = Application::Get().GetResources().Textures[RES_TEXTURE_DEFAULT].get();
						cmd.drawType = RendererDrawType::Triangles;

						renderer.Submit(cmd);
						renderer.Flush();
					}

					renderer.Flush();
				}

				_Framebuffer->Unbind();
			}

#ifdef WYRD_INCLUDE_DEBUG_TAGS
			renderer.EndNamedSection();
#endif
		}
	}

	void GameViewer::OnEvent(Event& event)
	{

		if (_Simulation->IsRunning())
		{
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<MouseButtonPressedEvent>(WYRD_BIND_EVENT_FN(GameViewer::OnMouseButtonPressedEvent), nullptr);
			dispatcher.Dispatch<MouseButtonReleasedEvent>(WYRD_BIND_EVENT_FN(GameViewer::OnMouseButtonReleasedEvent), nullptr);
			dispatcher.Dispatch<MouseScrolledEvent>(WYRD_BIND_EVENT_FN(GameViewer::OnMouseScrolledEvent), nullptr);
			dispatcher.Dispatch<MouseMovedEvent>(WYRD_BIND_EVENT_FN(GameViewer::OnMouseMovedEvent), nullptr);
			dispatcher.Dispatch<KeyPressedEvent>(WYRD_BIND_EVENT_FN(GameViewer::OnKeyPressedEvent), nullptr);
			dispatcher.Dispatch<KeyReleasedEvent>(WYRD_BIND_EVENT_FN(GameViewer::OnKeyReleasedEvent), nullptr);
		}
	}

	void GameViewer::OnEditorRender()
	{
		static bool showStats = false;
		static bool showGizmoSettings = false;

		/* calculate the viewport size */
		_Viewport._position.x = 0.0f;
		_Viewport._position.y = 0.0f;
		_Viewport._size.x = ImGui::GetContentRegionAvail().x;
		_Viewport._size.y = ImGui::GetContentRegionAvail().y - ImGui::GetCursorPos().y;


		const char* items[] = { "Export Size", "Window Size", "800x600" };
		if (ImGui::Combo("combo", &_SizeConfigID, items, IM_ARRAYSIZE(items)))
		{
			switch (_SizeConfigID)
			{
			case 0:
				_Framebuffer->Resize((uint32_t)_Workspace->GetCurrentProject()->GetExportSettings().width, (uint32_t)_Workspace->GetCurrentProject()->GetExportSettings().height);
				break;
			case 1:
				_Framebuffer->Resize((uint32_t)_Viewport._size.x, (uint32_t)_Viewport._size.y);
				break;
			case 2:
				_Framebuffer->Resize(800, 600);
				break;
			}
		}
		if (_CameraEntity != ENTITY_INVALID)
		{
			ImGui::Image((ImTextureID)(UINT_PTR)_Framebuffer->GetColorAttachmentID(), ImVec2(_Viewport._size.x, _Viewport._size.y), ImVec2(0, 1), ImVec2(1, 0));
		}
	}

	void GameViewer::OnResize()
	{
		if (_Viewport._size.x > 0.1f && _Viewport._size.y > 0.1f)
		{
			/* resize the underlying framebuffer */
			_Framebuffer->Resize((uint32_t)_Viewport._size.x, (uint32_t)_Viewport._size.y);

			//if (_CameraComponent != nullptr)
			//{
			//	/* Set the camera viewport */
			//	_Camera.SetSize(_CameraComponent->size);
			//	_Camera.SetViewportSize(_Viewport._size.x, _Viewport._size.y);
			//}
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
				//_CameraComponent = cameraComponent;
			}
			else
			{
				_CameraEntity = ENTITY_INVALID;
				//_CameraComponent = nullptr;
			}
		}
		else
		{
			_CameraEntity = ENTITY_INVALID;
			//_CameraComponent = nullptr;
		}
	}

	bool GameViewer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e, void* data)
	{
		return false;
	}

	bool GameViewer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e, void* data)
	{
		return false;
	}

	bool GameViewer::OnMouseScrolledEvent(MouseScrolledEvent& e, void* data)
	{
		return false;
	}

	bool GameViewer::OnMouseMovedEvent(MouseMovedEvent& e, void* data)
	{
		return false;
	}

	bool GameViewer::OnKeyPressedEvent(KeyPressedEvent& e, void* data)
	{
		if (e.GetRepeatCount() == 1)
		{
			_Simulation->SetInputState(e.GetKeyCode(), 0);
		}
		else
		{
			_Simulation->SetInputState(e.GetKeyCode(), 1);
		}
		return false;
	}

	bool GameViewer::OnKeyReleasedEvent(KeyReleasedEvent& e, void* data)
	{
		_Simulation->SetInputState(e.GetKeyCode(), 2);
		return false;
	}
}