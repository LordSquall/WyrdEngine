#pragma once

#include "ControlLayer.h"
#include "import/ImportManager.h"
#include "managers/SceneManager.h"

#include <core/Log.h>
#include <core/Application.h>
#include <core/ecs/ECS.h>
#include <core/ecs/EntitySet.h>
#include <core/scene/Scene.h>

#include <fstream>

bool ControlLayer::OnAttach()
{
	baseDirectory = "C:/Projects/GameProjects/DemoProject/.builds/";

	
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

	sceneManager.LoadScene(baseDirectory, UID("9e38ccae-3887-4f73-b2fa-003f5d5c6a61"));

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

	Application::Get().GetBehaviour().Start(std::shared_ptr<Wyrd::Scene>(scene));

	return true;
}

void ControlLayer::OnDetach() { }

void ControlLayer::OnUpdate(Timestep ts) 
{
	Application::Get().GetBehaviour().Update(ts);

	_Camera.RecalulateViewProjection();

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
		cmd.vpMatrix = _Camera.GetViewProjectionMatrix();
		cmd.shader = Application::Get().GetResources().Shaders["Sprite"].get();
		cmd.texture = Application::Get().GetResources().Textures[sprite->texture].get();
		cmd.color = sprite->color;

		renderer.Submit(cmd);
	}

	renderer.Flush();
}

bool ControlLayer::LoadCoreFile()
{
	ImportManager::ImportCore(baseDirectory);
	return true;
}

bool ControlLayer::LoadGameFile()
{
	ImportManager::ImportGame(baseDirectory);
	return true;
}

bool ControlLayer::LoadCommonBundleFile()
{
	ImportManager::ImportCommonBundle(baseDirectory);
	return true;
}