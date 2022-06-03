#include "ControlLayer.h"
#include "import/ImportManager.h"
#include "managers/SceneManager.h"

#include <core/Log.h>
#include <core/Application.h>
#include <core/ecs/ECS.h>
#include <core/ecs/EntitySet.h>
#include <core/scene/Scene.h>
#include <core/renderer/Texture.h>
#include <core/renderer/FontType.h>

#include <fstream>

std::shared_ptr<FontType> fonttype;

bool ControlLayer::OnAttach()
{
	baseDirectory = "C:/Projects/GameProjects/Demo/.builds/";

	
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

	SceneManager::getInstance().LoadInitialScene(baseDirectory);

	/* create a new framebuffer */
	unsigned int width = Application::Get().GetWindow().GetWidth();
	unsigned int height = Application::Get().GetWindow().GetHeight();

	auto scene = SceneManager::getInstance().GetScene();

	_CameraEntity = scene->GetPrimaryCameraEntity();
	Transform2DComponent* transformComponent = scene->Get<Transform2DComponent>(_CameraEntity);
	CameraComponent* cameraComponent = scene->Get<CameraComponent>(_CameraEntity);
	if (transformComponent == nullptr || cameraComponent == nullptr)
	{
		return false;
	}

	_Camera.SetPosition(glm::vec3(transformComponent->position,  0.0f));
	_Camera.SetSize(cameraComponent->size);
	_Camera.SetViewportSize(width, height);

	Application::Get().GetRenderer().SetViewport(width, height);

	Application::Get().GetBehaviour().SetSceneManager(&sceneManager);

	Application::Get().GetBehaviour().Start(std::shared_ptr<Wyrd::Scene>(scene));

	return true;
}

void ControlLayer::OnDetach() { }

void ControlLayer::OnUpdate(Timestep ts) 
{
	auto scene = SceneManager::getInstance().GetScene();

	Application::Get().GetBehaviour().Update(ts);

	Transform2DComponent* transform = scene->Get<Transform2DComponent>(_CameraEntity);
	_Camera.SetPosition({ transform->position, 0.0f });
	_Camera.RecalulateViewProjection();

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

	for (Entity e : EntitySet<Wyrd::Transform2DComponent, Wyrd::SpriteComponent>(*scene))
	{
		Wyrd::Transform2DComponent* transform = scene->Get<Wyrd::Transform2DComponent>(e);
		Wyrd::SpriteComponent* sprite = scene->Get<Wyrd::SpriteComponent>(e);

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

	for (Entity e : EntitySet<Wyrd::Transform2DComponent, Wyrd::TextComponent>(*scene))
	{
		Transform2DComponent* transform = scene->Get<Wyrd::Transform2DComponent>(e);
		TextComponent* text = scene->Get<Wyrd::TextComponent>(e);

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

bool ControlLayer::LoadCoreFile()
{
	return ImportManager::ImportCore(baseDirectory);
}

bool ControlLayer::LoadGameFile()
{
	return ImportManager::ImportGame(baseDirectory);
}

bool ControlLayer::LoadCommonBundleFile()
{
	return ImportManager::ImportCommonBundle(baseDirectory);
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