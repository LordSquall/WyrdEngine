#include "ControlLayer.h"
#include "import/ImportManager.h"
#include "managers/SceneManager.h"

#include <core/Log.h>
#include <core/Application.h>
#include <core/ecs/ECS.h>
#include <core/ecs/EntitySet.h>
#include <core/Maths.h>
#include <core/scene/Scene.h>
#include <core/renderer/Texture.h>
#include <core/renderer/FontType.h>

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

std::shared_ptr<FontType> fonttype;

bool ControlLayer::OnAttach()
{
	//baseDirectory = "C:/Projects/games/ScriptingTests/.builds/";
	WYRD_CORE_INFO("GameDir: {0}", _layerProps.gameDir);
	
	if (!LoadCoreFile())
	{
		WYRD_CORE_ERROR("Unable to load Core File");
		return false;
	}

	if (!LoadGameFile())
	{
		WYRD_CORE_ERROR("Unable to load Game File");
		return false;
	}
	
	if (!LoadCommonBundleFile())
	{
		WYRD_CORE_ERROR("Unable to load Common Bundle File");
		return false;
	}

	auto& sceneManager = SceneManager::getInstance();

	SceneManager::getInstance().LoadInitialScene(_layerProps.gameDir);

	/* create a new framebuffer */
	unsigned int width = Application::Get().GetWindow().GetWidth();
	unsigned int height = Application::Get().GetWindow().GetHeight();

	auto scene = SceneManager::getInstance().GetScene();

 	_CameraEntity = scene->GetPrimaryCameraEntity();
	Transform3DComponent* transformComponent = scene->Get<Transform3DComponent>(_CameraEntity);
	CameraComponent* cameraComponent = scene->Get<CameraComponent>(_CameraEntity);
	if (transformComponent == nullptr || cameraComponent == nullptr)
	{
		WYRD_CORE_ERROR("Unable to start scene!. Can't locate valid Camera configuration!");
		return false;
	}

	_Camera.SetPosition(glm::vec3(transformComponent->position.x, transformComponent->position.y, transformComponent->position.z));

	_Camera.perspectiveSettings.farPlane = cameraComponent->farPlane;
	_Camera.perspectiveSettings.nearPlane = cameraComponent->nearPlane;
	_Camera.perspectiveSettings.aspect = cameraComponent->aspectRatio;
	_Camera.SetMode(Camera::Mode::Perspective);

	Application::Get().GetRenderer().SetViewport(width, height);

	Application::Get().GetBehaviour().SetSceneManager(&sceneManager);

	Application::Get().GetBehaviour().Start(std::shared_ptr<Wyrd::Scene>(scene));

	return true;
}

void ControlLayer::OnDetach() { }

void ControlLayer::OnUpdate(Timestep ts) 
{
	auto scene = SceneManager::getInstance().GetScene();

	if (_CameraEntity != ENTITY_INVALID)
	{
		Transform3DComponent* transformComponent = scene->Get<Transform3DComponent>(_CameraEntity);
		CameraComponent* cameraComponent = scene->Get<CameraComponent>(_CameraEntity);
		RelationshipComponent* relationshipComponent = scene->Get<RelationshipComponent>(_CameraEntity);

		glm::vec3 translation = { transformComponent->position.x, transformComponent->position.y, transformComponent->position.z };
		glm::vec3 rotation;
		glm::vec3 scale;

		if (relationshipComponent != nullptr && relationshipComponent->parent != ENTITY_INVALID)
		{
			Transform3DComponent* parentTransform3DComponent = scene->Get<Transform3DComponent>(relationshipComponent->parent);

			glm::vec3 parentTranslation;
			glm::vec3 parentRotation;
			glm::vec3 parentScale;
			//Maths::DecomposeTransform(parentTransform3DComponent->modelMatrix, parentTranslation, rotation, scale);

			translation += parentTranslation;
		}

		_Camera.SetPosition(translation);
		_Camera.SetYaw(-DEG_TO_RAD(transformComponent->rotation.y));
		_Camera.SetPitch(DEG_TO_RAD(transformComponent->rotation.x));
		_Camera.SetMode(cameraComponent->projection == 0 ? Camera::Mode::Perspective : Camera::Mode::Orthographic);

		_Camera.perspectiveSettings.nearPlane = cameraComponent->nearPlane;
		_Camera.perspectiveSettings.farPlane = cameraComponent->farPlane;
		_Camera.perspectiveSettings.aspect = cameraComponent->aspectRatio;

		_Camera.orthoSettings.top = cameraComponent->top;
		_Camera.orthoSettings.bottom = cameraComponent->bottom;
		_Camera.orthoSettings.left = cameraComponent->left;
		_Camera.orthoSettings.right = cameraComponent->right;
		_Camera.orthoSettings.nearPlane = cameraComponent->nearPlane;
		_Camera.orthoSettings.farPlane = cameraComponent->farPlane;

		_Camera.Update();
	}

	for (Entity e : Wyrd::EntitySet<Wyrd::Transform3DComponent>(*scene))
	{
		Wyrd::Transform3DComponent* transform3DComponent = scene->Get<Wyrd::Transform3DComponent>(e);

		glm::quat q = glm::quat(glm::vec3(glm::radians(transform3DComponent->rotation.x), glm::radians(transform3DComponent->rotation.y), glm::radians(transform3DComponent->rotation.z)));

		glm::mat4 translate = glm::translate(glm::mat4(1), glm::vec3(transform3DComponent->position.x, transform3DComponent->position.y, transform3DComponent->position.z));
		glm::mat4 rotate = glm::toMat4(q);
		glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(transform3DComponent->scale.x, transform3DComponent->scale.y, transform3DComponent->scale.z));

		//transform3DComponent->modelMatrix = translate * rotate * scale;
	}
}

void ControlLayer::OnEvent(Event& event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyReleasedEvent>(WYRD_BIND_EVENT_FN(ControlLayer::OnKeyReleasedEvent), nullptr);
	dispatcher.Dispatch<KeyPressedEvent>(WYRD_BIND_EVENT_FN(ControlLayer::OnKeyPressedEvent), nullptr);
	dispatcher.Dispatch<MouseButtonPressedEvent>(WYRD_BIND_EVENT_FN(ControlLayer::OnMouseButtonPressedEvent), nullptr);
	dispatcher.Dispatch<MouseButtonReleasedEvent>(WYRD_BIND_EVENT_FN(ControlLayer::OnMouseButtonReleasedEvent), nullptr);
	dispatcher.Dispatch<MouseMovedEvent>(WYRD_BIND_EVENT_FN(ControlLayer::OnMouseMovedEvent), nullptr);
}

void ControlLayer::OnRender(Timestep ts, Renderer& renderer)
{ 
	auto scene = SceneManager::getInstance().GetScene();

	renderer.Clear(0.1f, 0.1f, 0.1f);
	for (Entity e : EntitySet<Wyrd::Transform3DComponent, Wyrd::MeshRendererComponent, Wyrd::MaterialComponent>(*scene))
	{
		Wyrd::Transform3DComponent* transform = scene->Get<Wyrd::Transform3DComponent>(e);
		Wyrd::MeshRendererComponent* meshRenderer = scene->Get<Wyrd::MeshRendererComponent>(e);
		Wyrd::MaterialComponent* material = scene->Get<Wyrd::MaterialComponent>(e);

		Wyrd::DrawMeshCommand cmd{};
		//cmd.modelMatrix = transform->modelMatrix;
		cmd.viewMatrix = _Camera.GetViewMatrix();
		cmd.projectionMatrix = _Camera.GetProjectionMatrix();
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

bool ControlLayer::LoadCoreFile()
{
	return ImportManager::ImportCore(_layerProps.gameDir);
}

bool ControlLayer::LoadGameFile()
{
	return ImportManager::ImportGame(_layerProps.gameDir);
}

bool ControlLayer::LoadCommonBundleFile()
{
	return ImportManager::ImportCommonBundle(_layerProps.gameDir);
}

bool ControlLayer::OnKeyReleasedEvent(KeyReleasedEvent& e, void* data)
{
	Application::Get().GetBehaviour().SetInputState(e.GetKeyCode(), 2);
	return false;
}

bool ControlLayer::OnKeyPressedEvent(KeyPressedEvent& e, void* data)
{
	if (e.GetRepeatCount() == 1)
	{
		Application::Get().GetBehaviour().SetInputState(e.GetKeyCode(), 0);
	}
	else
	{
		Application::Get().GetBehaviour().SetInputState(e.GetKeyCode(), 1);
	}
	return false;
}


bool ControlLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e, void* data)
{
	Application::Get().GetBehaviour().SetMouseButtonState(e.GetMouseButton(), true);
	return false;
}

bool ControlLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e, void* data)
{
	Application::Get().GetBehaviour().SetMouseButtonState(e.GetMouseButton(), false);
	return false;
}

bool ControlLayer::OnMouseMovedEvent(MouseMovedEvent& e, void* data)
{
	Application::Get().GetBehaviour().SetMouseState(e.GetX(), e.GetY());
	return false;
}