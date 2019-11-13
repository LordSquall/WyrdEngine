#pragma once
#include "osrpch.h"

#include "Layer2D.h"
#include "core/renderer/Renderer.h"

namespace Osiris::Editor
{
	Layer2D::Layer2D() : name("Untitled")
	{

	}

	Layer2D::Layer2D(const std::string& name) : name(name)
	{

	}

	Layer2D::Layer2D(const Layer2D& obj) : name(obj.name), gameObjects(obj.gameObjects)
	{

	}

	Layer2D::~Layer2D()
	{

	}


	void Layer2D::Render(Renderer& renderer)
	{
		for (auto go : gameObjects)
		{
			std::shared_ptr<Sprite> sprite = go->spriteRender->Sprite;

			sprite->GetVertexArray()->Bind();
			sprite->GetVertexBuffer()->Bind();
			sprite->GetIndexBuffer()->Bind();
			(*sprite->GetTexture())->Bind();

			renderer.DrawElements(RendererDrawType::Triangles, 6);
		}
	}
}