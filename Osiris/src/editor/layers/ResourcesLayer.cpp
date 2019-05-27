#pragma once

#include "osrpch.h"

#include "ResourcesLayer.h"

#include "core/Application.h"
#include "core/Layer.h"

#include "events/Event.h"


namespace Osiris::Editor
{
	ResourcesLayer::ResourcesLayer()
		: Layer("ResourcesLayer")
	{
		
	}

	ResourcesLayer::~ResourcesLayer()
	{

	}

	void ResourcesLayer::OnAttach()
	{
		Osiris::Resources& resources = Application::Get().GetResources();

		std::shared_ptr<Osiris::Shader> shader = Osiris::Shader::CreateFromFiles("C:/Projects/Active/OsirisEngine/OsirisEditor/res/shaders/sprite_shader.vs", "C:/Projects/Active/OsirisEngine/OsirisEditor/res/shaders/sprite_shader.fs");

		resources.AddShader(shader);
		
	}

	void ResourcesLayer::OnDetach()
	{

	}

	void ResourcesLayer::OnRender(std::shared_ptr<Renderer> renderer)
	{
		
		
	}

	void ResourcesLayer::OnEvent(Event& event)
	{
		
	}
}