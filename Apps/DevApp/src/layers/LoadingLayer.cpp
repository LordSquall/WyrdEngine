#include "LoadingLayer.h"

#include "graphics/Shader.h"
#include "graphics/Sprite.h"

LoadingLayer::LoadingLayer() : Layer("Loading Layer")
{
	///* save reference to the renderer */
	//m_Resources = resources;
	//
	///* load in the custom shader */
	//std::unique_ptr<Osiris::Shader> shader = Osiris::Shader::CreateFromFiles(
	//"C:/Projects/Active/OsirisEngine/Apps/DevApp/res/shaders/shader.vs", 
	//"C:/Projects/Active/OsirisEngine/Apps/DevApp/res/shaders/shader.fs");

	///* if successful then we need to add it to the resources subsystem. this will also generate the renderer data required to use the shader
	//the smart pointer move operation hands ownership of the shader pointer to the resources subsystem and will therefore handle deallocation */
	//if (shader)
	//	resources->AddShader(std::move(shader));

	/////* load the cube mesh file */
	////std::unique_ptr<Osiris::Mesh> mesh = Osiris::Mesh::CreateFromFile("C:/Projects/Active/OsirisEngine/Apps/DevApp/res/meshs/cube.obj");

	/////* if successful then we need to add it to the resources subsystem. we also want to generate the initialise renderer data
	////the smart pointer move operation hands ownership of the mesh pointer to the resources subsystem and will therefore handle deallocation */
	////if (mesh)
	////{
	////	m_Resources->AddMesh(std::move(mesh));
	////}

	//std::unique_ptr<Osiris::Sprite> sprite = std::make_unique<Osiris::Sprite>();

	//if (sprite)
	//{
	//	m_Resources->AddSprite(std::move(sprite));
	//}



}

void LoadingLayer::OnUpdate()
{
}

void LoadingLayer::OnEvent(Osiris::Event& e)
{
}