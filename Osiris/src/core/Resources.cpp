#include "osrpch.h"
#include "Resources.h"

Resources::Resources()
{

}

void Resources::SetRenderer(std::shared_ptr<Osiris::Renderer> renderer)
{
	m_Renderer = renderer;
}

bool Resources::AddShader(std::unique_ptr<Osiris::Shader> shader)
{
	if (m_Renderer->LoadShader(shader) == 0)
	{
		OSR_CLIENT_ERROR("Unable to load Shader!");
		return false;
	}

	m_ShaderMap.insert(std::pair<std::string, std::unique_ptr<Osiris::Shader>>("Shader 1", std::move(shader)));

	return true;
}

bool Resources::AddMesh(std::unique_ptr<Osiris::Mesh> mesh)
{
	if (m_Renderer->LoadMesh(mesh) == 0)
	{
		OSR_CLIENT_ERROR("Unable to load Mesh!");
		return false;
	}

	m_MeshMap.insert(std::pair<std::string, std::unique_ptr<Osiris::Mesh>>("Mesh 1", std::move(mesh)));

	return true;
}

bool Resources::AddSprite(std::unique_ptr<Osiris::Sprite> sprite)
{
	/*if (m_Renderer->LoadSprite(sprite) == 0)
	{
		OSR_CLIENT_ERROR("Unable to load Sprite!");
		return false;
	}

	m_Sprites.insert(std::move(sprite));*/

	return true;
}