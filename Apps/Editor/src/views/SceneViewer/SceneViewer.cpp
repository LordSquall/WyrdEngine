/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Maths.h>
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
#include "gizmos/3D/Grid3DGizmo.h"
#include "support/ImGuiUtils.h"
#include "datamodels/EditorComponents.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <ImGuizmo.h>

namespace Wyrd::Editor
{
	static bool showEditorComponent = true;

	SceneViewer::SceneViewer(EditorLayer* editorLayer) : EditorViewBase("Scene Viewer", editorLayer), _SelectedEntity(ENTITY_INVALID)
	{
		/* retrieve services */
		_Workspace = ServiceManager::Get<WorkspaceService>();
		_Events = ServiceManager::Get<EventService>();
		_Resources = ServiceManager::Get<ResourceService>();
		_Dialogs = ServiceManager::Get<DialogService>();
		_Settings = ServiceManager::Get<SettingsService>();
		_Simulation = ServiceManager::Get<SimulationService>();
		_CoreSystem = ServiceManager::Get<CoreSystemsService>();

		/* setup event bindings */
		_Events->Subscribe(Events::EventType::SceneOpened, WYRD_BIND_FN(SceneViewer::OnSceneOpened));
		_Events->Subscribe(Events::EventType::SelectedEntityChanged, WYRD_BIND_FN(SceneViewer::OnSelectedEntityChanged));

		/* initialise camera */
		_CameraController = std::make_shared<CameraController>();

		/* initialise the grid */
		_Grid3DGizmo = std::make_unique<Grid3DGizmo>(this);

		/* create a new framebuffer */
		_Framebuffer.reset(Wyrd::FrameBuffer::Create(FrameBufferConfig()));

		/* setup window config */
		config.windowPaddingX = 0.0f;
		config.windowPaddingY = 0.0f;

		CurrentTransformTool = TransformTool::Translate;
	}

	SceneViewer::~SceneViewer() 
	{
		WYRD_TRACE("SceneViewer::~");
	}

	void SceneViewer::OnUpdate(Timestep ts) 
	{
		if (_Scene != nullptr)
		{
			for (Entity e : EntitySet<EditorComponent, Transform3DComponent, RelationshipComponent>(*_Scene.get()))
			{
				EditorComponent* editorComponent = _Scene->Get<EditorComponent>(e);
				Transform3DComponent* transform3DComponent = _Scene->Get<Transform3DComponent>(e);
				RelationshipComponent* relationshipComponent = _Scene->Get<RelationshipComponent>(e);

				glm::quat q = glm::quat(glm::vec3(glm::radians(transform3DComponent->rotation.x), glm::radians(transform3DComponent->rotation.y), glm::radians(transform3DComponent->rotation.z)));

				glm::mat4 translate = glm::translate(glm::mat4(1), glm::vec3(transform3DComponent->position.x, transform3DComponent->position.y, transform3DComponent->position.z));
				glm::mat4 rotate = glm::toMat4(q);
				glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(transform3DComponent->scale.x, transform3DComponent->scale.y, transform3DComponent->scale.z));

				transform3DComponent->modelMatrix = translate * rotate * scale;

				if (relationshipComponent->parent != ENTITY_INVALID)
				{
					Transform3DComponent* parentTransform3DComponent = _Scene->Get<Transform3DComponent>(relationshipComponent->parent);
					if (parentTransform3DComponent != nullptr)
					{
						transform3DComponent->parentModelMatrix = parentTransform3DComponent->modelMatrix;
					}
				}

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
				Color bgColor = Utils::ToColor(_Settings->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__BGBOLOR, "1,1,1,1"));

				_Framebuffer->Bind();
				renderer.Clear(bgColor.r, bgColor.g, bgColor.b);

				for (Entity e : EntitySet<Transform3DComponent, MeshRendererComponent, MaterialComponent>(*_Scene.get()))
				{
					Transform3DComponent* transform = _Scene->Get<Transform3DComponent>(e);
					MeshRendererComponent* meshRenderer = _Scene->Get<MeshRendererComponent>(e);
					MaterialComponent* material = _Scene->Get<MaterialComponent>(e);

					Wyrd::DrawMeshCommand cmd{};
					cmd.modelMatrix = transform->modelMatrix * transform->parentModelMatrix;
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
					//for (Entity e : EntitySet<Transform3DComponent, EditorComponent>(*_Scene.get()))
					//{
					//	Transform3DComponent* transform = _Scene->Get<Transform3DComponent>(e);
					//	EditorComponent* editorComponent = _Scene->Get<EditorComponent>(e);
					//
					//	renderer.DrawDebugBoundingBox(editorComponent->inputBoundingBox, { transform->position.x, transform->position.y, transform->position.z }, 1.0f, Color::MAGENTA, glm::identity<glm::mat4>(), _CameraController->GetCamera().GetProjectionMatrix(), _CameraController->GetCamera().GetViewMatrix());
					//
					//	renderer.DrawDebugVector(transform->position, { 10.0f, 0.0f, 0.0f }, 10.0f, Color::RED,  glm::identity<glm::mat4>(), _CameraController->GetCamera().GetProjectionMatrix(), _CameraController->GetCamera().GetViewMatrix());
					//	renderer.DrawDebugVector(transform->position, { 0.0f, 10.0f, 0.0f }, 10.0f, Color::GREEN, glm::identity<glm::mat4>(), _CameraController->GetCamera().GetProjectionMatrix(), _CameraController->GetCamera().GetViewMatrix());
					//	renderer.DrawDebugVector(transform->position, { 0.0f, 0.0f, 10.0f }, 10.0f, Color::BLUE, glm::identity<glm::mat4>(), _CameraController->GetCamera().GetProjectionMatrix(), _CameraController->GetCamera().GetViewMatrix());
					//}

					if (_SelectedEntity != ENTITY_INVALID)
					{
						Transform3DComponent* transform = _Scene->Get<Transform3DComponent>(_SelectedEntity);
						CameraComponent* cameraComponent = _Scene->Get<CameraComponent>(_SelectedEntity);

						if (transform != nullptr && cameraComponent != nullptr)
						{
							// As the camera component doesn't have a camera object, we need to create one to show the debug info
							
							Camera debugCamera;

							debugCamera.SetPosition({ transform->position.x, transform->position.y, transform->position.z });
							debugCamera.SetYaw(-DEG_TO_RAD(transform->rotation.y));
							debugCamera.SetPitch(DEG_TO_RAD(transform->rotation.x));
							debugCamera.SetMode(cameraComponent->projection == 0 ? Camera::Mode::Perspective : Camera::Mode::Orthographic);
							debugCamera.perspectiveSettings.nearPlane = cameraComponent->nearPlane;
							debugCamera.perspectiveSettings.farPlane = cameraComponent->farPlane;
							debugCamera.perspectiveSettings.aspect = cameraComponent->aspectRatio;

							debugCamera.orthoSettings.top = cameraComponent->top;
							debugCamera.orthoSettings.bottom = cameraComponent->bottom;
							debugCamera.orthoSettings.left = cameraComponent->left;
							debugCamera.orthoSettings.right = cameraComponent->right;
							debugCamera.orthoSettings.nearPlane = cameraComponent->nearPlane;
							debugCamera.orthoSettings.farPlane = cameraComponent->farPlane;
							debugCamera.Update();

							renderer.DrawDebugFrustum({ -transform->position.x, -transform->position.y, -transform->position.z }, debugCamera.GetForwardDirection(), debugCamera.frustum, 3.0f, Color::CYAN, glm::mat4(1), _CameraController->GetCamera().GetProjectionMatrix(), _CameraController->GetCamera().GetViewMatrix());
						}
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
		if (!_Dialogs->IsDialogOpen)
		{
			EventDispatcher dispatcher(event);
			dispatcher.Dispatch<MouseButtonPressedEvent>(WYRD_BIND_EVENT_FN(SceneViewer::OnMouseButtonPressedEvent), nullptr);
			dispatcher.Dispatch<MouseButtonReleasedEvent>(WYRD_BIND_EVENT_FN(SceneViewer::OnMouseButtonReleasedEvent), nullptr);
			dispatcher.Dispatch<MouseScrolledEvent>(WYRD_BIND_EVENT_FN(SceneViewer::OnMouseScrolledEvent), nullptr);
			dispatcher.Dispatch<MouseMovedEvent>(WYRD_BIND_EVENT_FN(SceneViewer::OnMouseMovedEvent), nullptr);
			dispatcher.Dispatch<KeyPressedEvent>(WYRD_BIND_EVENT_FN(SceneViewer::OnKeyPressedEvent), nullptr);
			dispatcher.Dispatch<KeyReleasedEvent>(WYRD_BIND_EVENT_FN(SceneViewer::OnKeyReleasedEvent), nullptr);

			/* Set camera settings */
			if (_Scene != nullptr)
			{
				//_Scene->cameraZoom = _CameraController->GetSize();
				_Scene->cameraPosition = _CameraController->GetPosition();
				_Scene->cameraOrientation = _CameraController->GetOrientation();
			}
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
		ImGui::PushID("TranslateToolBtn");
		if (ImGui::IconButton(_Resources->RetrieveIcon("common", "transform_translate"), 2000, true, size, -1, CurrentTransformTool == TransformTool::Translate ? ImVec4(1, 1, 1, 1) : ImVec4(0, 0, 0, 0), ImVec4(0.5f, 0.5f, 0.5f, 1.0f)) == true)
		{
			CurrentTransformTool = TransformTool::Translate;
		}
		ImGui::SameLine();
		ImGui::PopID();


		ImGui::PushID("RotateToolBtn");
		if (ImGui::IconButton(_Resources->RetrieveIcon("common", "transform_rotate"), 2001, true, size, -1, CurrentTransformTool == TransformTool::Rotate ? ImVec4(1, 1, 1, 1) : ImVec4(0, 0, 0, 0), ImVec4(0.5f, 0.5f, 0.5f, 1.0f)) == true)
		{
			CurrentTransformTool = TransformTool::Rotate;
		}
		ImGui::SameLine();
		ImGui::PopID();


		ImGui::PushID("ScaleToolBtn");
		if (ImGui::IconButton(_Resources->RetrieveIcon("common", "transform_scale"), 2002, true, size, -1, CurrentTransformTool == TransformTool::Scale ? ImVec4(1, 1, 1, 1) : ImVec4(0, 0, 0, 0), ImVec4(0.5f, 0.5f, 0.5f, 1.0f)) == true)
		{
			CurrentTransformTool = TransformTool::Scale;
		}
		ImGui::SameLine();
		ImGui::PopID();
		
		/* build the top toolbar */
		ImGui::Checkbox("show stats", &showStats);
		ImGui::SameLine();
		ImGui::Checkbox("show EC", &showEditorComponent);
		ImGui::SameLine();

		ImGui::SetCursorPosX(ImGui::GetWindowSize().x - (size.x * 2.0f));
		if (ImGui::BeginPopupContextItem("sceneviewer_settings_popup"))
		{
			/* Camera Settings */
			ImGui::SeparatorText("Camera");
			ImGui::InputFloat("Far", &_CameraController->GetCamera().perspectiveSettings.nearPlane);
			ImGui::InputFloat("Near", &_CameraController->GetCamera().perspectiveSettings.farPlane);
			ImGui::InputFloat("FOV", &_CameraController->GetCamera().perspectiveSettings.fov);
			ImGui::InputFloat("aspect", &_CameraController->GetCamera().perspectiveSettings.aspect);

			ImGui::SeparatorText("Viewport");
			Color bgColor = Utils::ToColor(_Settings->GetSetting(CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__BGBOLOR, "1,1,1,1"));
			if (ImGui::ColorPicker3("Background Color", (float*)&bgColor))
			{
				_Settings->SetSetting(Utils::ToString(bgColor), CONFIG_SCENEVIEWER, CONFIG_SCENEVIEWER__BGBOLOR);
			}
			ImGui::EndPopup();
		}

		if (ImGui::IconButton(_Resources->RetrieveIcon("common", "transform_scale"), 2003, true, size, -1, CurrentTransformTool == TransformTool::Scale ? ImVec4(1, 1, 1, 1) : ImVec4(0, 0, 0, 0), ImVec4(0.5f, 0.5f, 0.5f, 1.0f)) == true)
		{
			ImGui::OpenPopup("sceneviewer_settings_popup");
		}

		
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

		if (_SelectedEntity != ENTITY_INVALID)
		{
			Transform3DComponent* transform = _Scene->Get<Transform3DComponent>(_SelectedEntity);
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
			ImGuizmo::SetRect(_ViewportBoundary._position.x, _ViewportBoundary._position.y, _ViewportBoundary._size.x, _ViewportBoundary._size.y);
			glm::mat4 camViewInverseMat = _CameraController->GetCamera().GetViewMatrix();
			glm::mat4 projectionMat = _CameraController->GetCamera().GetProjectionMatrix();
			glm::mat4 transformMat = transform->modelMatrix * transform->parentModelMatrix;

			//transform->modelMatrix * transform->parentModelMatrix

			ImGuizmo::OPERATION op;
			switch (CurrentTransformTool)
			{
			case TransformTool::Translate: op = ImGuizmo::OPERATION::TRANSLATE; break;
			case TransformTool::Rotate: op = ImGuizmo::OPERATION::ROTATE; break;
			case TransformTool::Scale: op = ImGuizmo::OPERATION::SCALE; break;
			}

			ImGuizmo::Enable(true);
			ImGuizmo::Manipulate(glm::value_ptr(camViewInverseMat), glm::value_ptr(projectionMat), op, ImGuizmo::LOCAL, glm::value_ptr(transformMat));


			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Maths::DecomposeTransform(transformMat, translation, rotation, scale);

				glm::vec3 dRot = { RAD_TO_DEG(rotation.x) - transform->rotation.x, RAD_TO_DEG(rotation.y) - transform->rotation.y, RAD_TO_DEG(rotation.z) - transform->rotation.z };
				transform->position = translation;
				transform->rotation = transform->rotation + dRot;
				transform->scale = scale;
			}
		}

		if (showStats == true)
		{
			ImGui::Checkbox("use ortho", &useOrtho);
			ImGui::SetCursorPosY(58.0f);
			ImGui::Text("Camera:");
			auto pos = _CameraController->GetCamera().GetPosition();
			ImGui::Text("\tPos	[%f,%f,%f]", pos.x, pos.y, pos.z);

			ImGui::Text("Axis:");
			auto fwd = _CameraController->GetCamera().GetForwardDirection();
			ImGui::Text("\tForward	[%f,%f,%f]", fwd.x, fwd.y, fwd.z);

			auto right = _CameraController->GetCamera().GetRightDirection();
			ImGui::Text("\tRight	[%f,%f,%f]", right.x, right.y, right.z);

			auto up = _CameraController->GetCamera().GetUpDirection();
			ImGui::Text("\tUp	[%f,%f,%f]", up.x, up.y, up.z);


			ImGui::Text("Camera Controller:");
			switch (_CameraController->GetTransformMode())
			{
				case CameraController::Mode::None:
					ImGui::Text("\tTransform Mode: NONE");
					break;
				case CameraController::Mode::Pivot:
					ImGui::Text("\tTransform Mode: PIVOT");
					ImGui::Text("\t_InitialPitch [%f]", _CameraController->_InitialPitch);
					ImGui::Text("\t_InitialYaw [%f]", _CameraController->_InitialYaw);
					ImGui::Text("\t_PitchDelta [%f]", _CameraController->_PivotPitchDelta);
					ImGui::Text("\t_PitchYaw [%f]", _CameraController->_PivotYawDelta);
					break;
			}
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
		_Simulation->SetMouseButtonState(e.GetMouseButton(), true);

		/* build the initial mouse position vector */
		glm::vec2 mousePos = { e.GetPositionX(), e.GetPositionY() };

		/* viewport mouse position */
		glm::vec2 viewportPos = GetViewportSpaceFromPoint(mousePos);

		/* we only want to process mouse events within the viewport of the scene */
		if (_Viewport.ContainsPoint(viewportPos) == true)
		{
			if (_Scene && !ImGuizmo::IsOver())
			{
				for (Entity e : EntitySet<EditorComponent, MetaDataComponent, Transform3DComponent>(*_Scene.get()))
				{
					MetaDataComponent* metaDataComponent = _Scene->Get<MetaDataComponent>(e);
					Transform3DComponent* transform3DComponent = _Scene->Get<Transform3DComponent>(e);
					EditorComponent* editorComponent = _Scene->Get<EditorComponent>(e);

					Ray r;
					PhysicsUtils::ScreenPosToWorldRay(viewportPos, _ViewportBoundary._size, _CameraController->GetCamera().GetViewMatrix(), _CameraController->GetCamera().GetProjectionMatrix(), r);

					if (PhysicsUtils::IntersectRayBoundingBox(transform3DComponent->modelMatrix * transform3DComponent->parentModelMatrix, r, editorComponent->inputBoundingBox))
					{
						_Events->Publish(Editor::Events::EventType::SelectedEntityChanged, std::make_unique<Events::SelectedEntityChangedArgs>(e));
						return true;
					}
				}
			}
		}

		return false;
	}

	bool SceneViewer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e, void* data)
	{
		_CameraController->OnEvent(e);
		_Simulation->SetMouseButtonState(e.GetMouseButton(), false);
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
				//_CameraController->Translate({ worldSpaceDelta.x, worldSpaceDelta.y });
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
			_Simulation->SetInputState(e.GetKeyCode(), 0);
		}
		else
		{
			_Simulation->SetInputState(e.GetKeyCode(), 1);
		}

		return true;
	}

	bool SceneViewer::OnKeyReleasedEvent(KeyReleasedEvent& e, void* data)
	{
		_CameraController->OnEvent(e);

		if (_Simulation->IsRunning())
		{
			_Simulation->SetInputState(e.GetKeyCode(), 2);
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
		_CameraController->SetPosition(glm::vec3(_Scene->cameraPosition.x, _Scene->cameraPosition.y, _Scene->cameraPosition.z));
		_CameraController->SetOrientation(glm::vec3(_Scene->cameraOrientation.x, _Scene->cameraOrientation.y, _Scene->cameraOrientation.z));
	}

	void SceneViewer::OnSelectedEntityChanged(Events::EventArgs& args)
	{
		Events::SelectedEntityChangedArgs& evtArgs = static_cast<Events::SelectedEntityChangedArgs&>(args);
		_SelectedEntity = evtArgs.entity;
	}
}