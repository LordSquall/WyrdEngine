#include "LoadingLayer.h"


#include <core/Resources.h>
#include <graphics/Shader.h>
#include <graphics/Sprite.h>

LoadingLayer::LoadingLayer() : Layer("Loading Layer")
{
	/* load in the custom shader */
	std::shared_ptr<Osiris::Shader> shader = Osiris::Shader::CreateFromFiles(
	"C:/Projects/Active/OsirisEngine/Apps/DevApp/res/shaders/shader.vs", 
	"C:/Projects/Active/OsirisEngine/Apps/DevApp/res/shaders/shader.fs");

	/* if successful then we need to add it to the resources subsystem. this will also generate the renderer data required to use the shader
	the smart pointer move operation hands ownership of the shader pointer to the resources subsystem and will therefore handle deallocation */
	if (shader)
		Osiris::Resources::Get().AddShader(shader);
}

void LoadingLayer::OnUpdate()
{
}

void LoadingLayer::OnEvent(Osiris::Event& e)
{
}