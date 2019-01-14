#pragma once

#include <osrpch.h>
#include <core/Layer.h>
#include <core/Renderer.h>

#include <graphics/Sprite.h>

class Resources
{
public:
	Resources();

	void SetRenderer(std::shared_ptr<Osiris::Renderer> renderer);

	bool AddShader(std::unique_ptr<Osiris::Shader> shader);

	bool AddMesh(std::unique_ptr<Osiris::Mesh> mesh);

	bool AddSprite(std::unique_ptr<Osiris::Sprite> sprite);

public:
	std::shared_ptr<Osiris::Renderer> m_Renderer;

	std::unordered_map<std::string, std::unique_ptr<Osiris::Shader>> m_ShaderMap;

	std::unordered_map<std::string, std::unique_ptr<Osiris::Mesh>> m_MeshMap;
	
	std::vector<std::unique_ptr<Osiris::Sprite>> m_Sprites;
};