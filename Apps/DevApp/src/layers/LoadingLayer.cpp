#include "LoadingLayer.h"


#include <core/Resources.h>
#include <graphics/Shader.h>
#include <graphics/Texture.h>
#include <graphics/Sprite.h>

LoadingLayer::LoadingLayer() : Layer("Loading Layer")
{
	std::shared_ptr<Osiris::Shader> shader = Osiris::Shader::CreateFromFiles(
	"C:/Projects/Active/OsirisEngine/Apps/DevApp/res/shaders/shader.vs", 
	"C:/Projects/Active/OsirisEngine/Apps/DevApp/res/shaders/shader.fs");

	if (shader)
		Osiris::Resources::Get().AddShader(shader);

	std::string path = "./res/textures/";
	for (const auto & entry : std::filesystem::directory_iterator(path))
	{
		std::shared_ptr<Osiris::Texture> texture = Osiris::Texture::CreateFromFile(
			entry.path().string().c_str()
		);

		if (texture)
			Osiris::Resources::Get().AddTexture(texture);

		std::cout << entry.path() << std::endl;
	}
}

void LoadingLayer::OnUpdate()
{
}

void LoadingLayer::OnEvent(Osiris::Event& e)
{
}