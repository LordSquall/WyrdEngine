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
#include <core/renderer/Mesh.h>
#include <core/physics/PhysicsUtils.h>

/* local include */
#include "SceneViewer.h"
#include "gizmos/2D/Translate2DGizmo.h"
#include "gizmos/2D/Rotate2DGizmo.h"
#include "gizmos/2D/Scale2DGizmo.h"
#include "gizmos/3D/Grid3DGizmo.h"
#include "support/ImGuiUtils.h"
#include "datamodels/EditorComponents.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

namespace Wyrd::Editor
{
	static bool showEditorComponent = false;

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
		_EventService->Subscribe(Events::EventType::SceneOpened, WYRD_BIND_FN(SceneViewer::OnSceneOpened));
		_EventService->Subscribe(Events::EventType::SelectedEntityChanged, WYRD_BIND_FN(SceneViewer::OnSelectedEntityChanged));

		/* initialise camera */
		_CameraController = std::make_shared<CameraController>();

		/* initialise each of the transformation gizmos */
		_TransformationGizmos.push_back(std::make_unique<Translate2DGizmo>(this));
		_TransformationGizmos.push_back(std::make_unique<Rotate2DGizmo>(this));
		_TransformationGizmos.push_back(std::make_unique<Scale2DGizmo>(this));
		_CurrentTransformationGizmoIndex = 0;

		_Grid3DGizmo = std::make_unique<Grid3DGizmo>(this);

		/* initialise each icon */
		_pointSelectBtnIcon = _ResourceService->GetIconLibrary().GetIcon("common", "sim_play");
		_TransformationGizmoIcons.push_back(_ResourceService->GetIconLibrary().GetIcon("common", "sim_play"));
		_TransformationGizmoIcons.push_back(_ResourceService->GetIconLibrary().GetIcon("common", "sim_play"));
		_TransformationGizmoIcons.push_back(_ResourceService->GetIconLibrary().GetIcon("common", "sim_play"));

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
			for (Entity e : EntitySet<EditorComponent, Transform3DComponent>(*_Scene.get()))
			{
				EditorComponent* editorComponent = _Scene->Get<EditorComponent>(e);
				Transform3DComponent* transform3DComponent = _Scene->Get<Transform3DComponent>(e);

				glm::quat q = glm::quat(glm::vec3(glm::radians(transform3DComponent->rotation.x), glm::radians(transform3DComponent->rotation.y), glm::radians(transform3DComponent->rotation.z)));

				glm::mat4 translate = glm::translate(glm::mat4(1), glm::vec3(transform3DComponent->position.x, transform3DComponent->position.y, transform3DComponent->position.z));
				glm::mat4 rotate = glm::toMat4(q);
				glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(transform3DComponent->scale.x, transform3DComponent->scale.y, transform3DComponent->scale.z));

				transform3DComponent->modelMatrix = translate * rotate * scale;

				MeshRendererComponent* meshRenderer = _Scene->Get<MeshRendererComponent>(e);
				if (meshRenderer != nullptr)
				{
					const auto& mesh = Application::Get().GetResources().Meshs[meshRenderer->model].get();
					if (mesh != nullptr)
					{
						editorComponent->inputBoundingBox = mesh->boundingBox;

						glm::vec4 translatedMinExtents = rotate * scale * glm::vec4(editorComponent->inputBoundingBox._minExtent, 1.0f);
						glm::vec4 translatedMaxExtents = rotate * scale * glm::vec4(editorComponent->inputBoundingBox._maxExtent, 1.0f);
						editorComponent->inputBoundingBox._minExtent = glm::vec3(translatedMinExtents.x, translatedMinExtents.y, translatedMinExtents.z);
						editorComponent->inputBoundingBox._maxExtent = glm::vec3(translatedMaxExtents.x, translatedMaxExtents.y, translatedMaxExtents.z);
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
			
			if (_Scene != nullptr)
			{

				_Framebuffer->Bind();
				renderer.Clear(0.1f, 0.1f, 0.1f);

				for (Entity e : EntitySet<Transform3DComponent, MeshRendererComponent, MaterialComponent>(*_Scene.get()))
				{
					Transform3DComponent* transform = _Scene->Get<Transform3DComponent>(e);
					MeshRendererComponent* meshRenderer = _Scene->Get<MeshRendererComponent>(e);
					MaterialComponent* material = _Scene->Get<MaterialComponent>(e);

					Wyrd::DrawMeshCommand cmd{};
					cmd.modelMatrix = transform->modelMatrix;
					cmd.viewMatrix = _CameraController->GetCamera().GetViewMatrix();
					cmd.projectionMatrix = _CameraController->GetCamera().GetProjectionMatrix();
					cmd.material = Application::Get().GetResources().Materials[material->material].get();
					cmd.materialProps = &material->properties;
					cmd.mesh = Application::Get().GetResources().Meshs[meshRenderer->model].get();
					cmd.baseTexture = Application::Get().GetResources().Textures[RES_TEXTURE_DEFAULT].get();
					cmd.drawType = RendererDrawType::Triangles;
					
					renderer.Submit(cmd);
					renderer.Flush();
				}

				if (showEditorComponent)
				{
					for (Entity e : EntitySet<Transform3DComponent, EditorComponent>(*_Scene.get()))
					{
						Transform3DComponent* transform = _Scene->Get<Transform3DComponent>(e);
						EditorComponent* editorComponent = _Scene->Get<EditorComponent>(e);

						BasePropMapRef materialProps = std::make_shared<std::map<std::string, BasePropRef>>();
						(*materialProps)["BlendColor"] = PropFactory::CreateProp("Color", "BlendColor");
						(*materialProps)["BlendColor"]->Set<Color>(Color::MAGENTA);

						renderer.DrawDebugBoundingBox(editorComponent->inputBoundingBox, { transform->position.x, transform->position.y, transform->position.z }, Color::MAGENTA, _CameraController->GetCamera().GetProjectionMatrix(), _CameraController->GetCamera().GetViewMatrix());
					}
				}

				_Grid3DGizmo->Render(ts, renderer);

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

		_TransformationGizmos[_CurrentTransformationGizmoIndex]->OnEvent(event);

		/* Set camera settings */
		if (_Scene != nullptr)
		{
			//_Scene->cameraZoom = _CameraController->GetSize();
			_Scene->cameraPosition = _CameraController->GetPosition();
		}
	}

	void SceneViewer::OnEditorRender()
	{
		static bool showStats = false;
		static bool useOrtho = false;
		static bool showGizmoSettings = false;
		
		/* get the current cursor screen pos for y to determine the final size of the toolbars */
		float menuBarHeight = ImGui::GetCursorScreenPos().y;
		
		const ImVec2 size(16.0f, 16.0f);
		
		/* here we want to create a simple toolbox to show the different transformation tools */
		for (int i = 0; i < _TransformationGizmos.size(); i++)
		{
			ImGui::PushID(_TransformationGizmos[i].get());
			if (ImGui::IconButton(_TransformationGizmoIcons[i], 1, _CurrentTransformationGizmoIndex == i, size) == true)
			{
				_CurrentTransformationGizmoIndex = i;
			}
			ImGui::SameLine();
			ImGui::PopID();
		}
		
		if (ImGui::IconButton(_pointSelectBtnIcon, 1, false, size) == true)
		{
		
		}
		ImGui::SameLine();
		
		/* build the top toolbar */
		ImGui::Checkbox("show stats", &showStats);
		ImGui::SameLine();
		ImGui::Checkbox("show EC", &showEditorComponent);
		
		menuBarHeight = ImGui::GetCursorScreenPos().y - menuBarHeight;
		
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
		
		if (useOrtho == true)
		{
			_CameraController->SetMode(Camera::Mode::Orthographic);
		}
		else
		{
			_CameraController->SetMode(Camera::Mode::Perspective);
		}

		ImGui::Image((ImTextureID)(UINT_PTR)_Framebuffer->GetColorAttachmentID(), ImVec2(_Viewport._size.x, _Viewport._size.y), ImVec2(0, 1), ImVec2(1, 0));

		if (showStats == true)
		{
			ImGui::Checkbox("use ortho", &useOrtho);
			ImGui::SetCursorPosY(58.0f);
			ImGui::Text("Camera:");
		
			float camYaw = _CameraController->GetCamera().GetYaw();
		
			if (ImGui::InputFloat("Yaw", &camYaw))
			{
				_CameraController->GetCamera().SetYaw(camYaw);
			}
			ImGui::Text("\tPos		[%f, %f, %f]", _CameraController->GetPosition().x, _CameraController->GetPosition().y, _CameraController->GetPosition().z);
		
			ImGui::Text("\tUp		[%f, %f, %f]", _CameraController->GetCamera().GetUp().x, _CameraController->GetCamera().GetUp().y, _CameraController->GetCamera().GetUp().z);
			ImGui::Text("\tForward	[%f, %f, %f]", _CameraController->GetCamera().GetForward().x, _CameraController->GetCamera().GetForward().y, _CameraController->GetCamera().GetForward().z);
			ImGui::Text("\tPitch	[%f]", _CameraController->GetCamera().GetPitch());
			ImGui::Text("\tYaw		[%f]", _CameraController->GetCamera().GetYaw());
		
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
		/* resize the underlying framebuffer(s) */
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

	glm::vec3 SceneViewer::GetWorldSpaceFromPoint(const glm::vec3& point)
	{
		glm::vec3 viewportMouseCoords = point - glm::vec3(_ViewportBoundary._position.x, _ViewportBoundary._position.y, 0.0f);

		return _CameraController->GetCamera().GetWorldSpaceFromPoint(viewportMouseCoords, _ViewportBoundary);
	}

	glm::vec2 SceneViewer::GetScreenSpaceFromWorldPoint(const glm::vec2& point)
	{
		glm::vec2 viewportMouseCoords = point - _ViewportBoundary._position;

		return { 0.0f, 0.0f };// return _CameraController->GetCamera().GetNDCFromPoint(viewportMouseCoords, _ViewportBoundary);
	}

	bool SceneViewer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e, void* data)
	{
		_CameraController->OnEvent(e);
		_SimulationService->SetMouseButtonState(e.GetMouseButton(), true);

		/* build the initial mouse position vector */
		glm::vec2 mousePos = { e.GetPositionX(), e.GetPositionY() };

		/* viewport mouse position */
		glm::vec2 viewportPos = GetViewportSpaceFromPoint(mousePos);

		/* we only want to process mouse events within the viewport of the scene */
		if (_Viewport.ContainsPoint(viewportPos) == true)
		{
			for (Entity e : EntitySet<EditorComponent, MetaDataComponent, Transform3DComponent>(*_Scene.get()))
			{
				MetaDataComponent* metaDataComponent = _Scene->Get<MetaDataComponent>(e);
				Transform3DComponent* transform3DComponent = _Scene->Get<Transform3DComponent>(e);
				EditorComponent* editorComponent = _Scene->Get<EditorComponent>(e);

				Ray r;
				PhysicsUtils::ScreenPosToWorldRay(viewportPos, _ViewportBoundary._size, _CameraController->GetCamera().GetViewMatrix(), _CameraController->GetCamera().GetProjectionMatrix(), r);

				if (PhysicsUtils::IntersectRayBoundingBox(transform3DComponent->modelMatrix, r, editorComponent->inputBoundingBox))
				{
					_EventService->Publish(Editor::Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(e));
				}
			}
		}

		return true;
	}

	bool SceneViewer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e, void* data)
	{
		_CameraController->OnEvent(e);
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
		_CameraController->OnEvent(e);
		glm::vec2 viewportMouseCoords = glm::vec2(e.GetX(), e.GetY()) - _mouseOffset;

		if (_ViewportBoundary.ContainsPoint(glm::vec2(e.GetX(), e.GetY())))
		{
			/* track the world position of the mouse */
			//_MouseWorldPos = _CameraController->GetCamera().GetEyeSpaceFromPoint(viewportMouseCoords, _ViewportBoundary);

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

		if (e.GetKeyCode() == OSR_KEY_0)
		{
			{
				std::ifstream vertexStream(Utils::GetEditorResFolder() + "shaders/mesh.vs");
				std::string vertexSrc((std::istreambuf_iterator<char>(vertexStream)), std::istreambuf_iterator<char>());

				std::ifstream fragmentStream(Utils::GetEditorResFolder() + "shaders/mesh.fs");
				std::string fragmentSrc((std::istreambuf_iterator<char>(fragmentStream)), std::istreambuf_iterator<char>());

				std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(Shader::Create());
				Application::Get().GetResources().Shaders["Mesh"]->Build(vertexSrc, fragmentSrc);
			}

			{
				std::ifstream vertexStream(Utils::GetEditorResFolder() + "shaders/gizmo3DGrid.vs");
				std::string vertexSrc((std::istreambuf_iterator<char>(vertexStream)), std::istreambuf_iterator<char>());

				std::ifstream fragmentStream(Utils::GetEditorResFolder() + "shaders/gizmo3DGrid.fs");
				std::string fragmentSrc((std::istreambuf_iterator<char>(fragmentStream)), std::istreambuf_iterator<char>());

				std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(Shader::Create());
				Application::Get().GetResources().Shaders["Gizmo3DGrid"]->Build(vertexSrc, fragmentSrc);
			}
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
		//_CameraController->SetSize(_Scene->cameraZoom);
		//_CameraController->SetPosition(glm::vec3(_Scene->cameraPosition.x, _Scene->cameraPosition.y, _Scene->cameraPosition.z));
	}

	void SceneViewer::OnSelectedEntityChanged(Events::EventArgs& args)
	{
		Events::SelectedEntityChangedArgs& evtArgs = static_cast<Events::SelectedEntityChangedArgs&>(args);
		_SelectedEntity = evtArgs.entity;
	}
}