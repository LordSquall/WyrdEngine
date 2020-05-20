#pragma once

#include "PlaygroundLayer.h"

#include <fstream>

bool PlaygroundLayer::OnAttach()
{

	/* Load any resources which are required to renderer (e.g. shaders) */
	std::ifstream vertexStream("res/shader.vert");
	std::string vertexSrc((std::istreambuf_iterator<char>(vertexStream)), std::istreambuf_iterator<char>());

	std::ifstream fragmentStream("res/shader.frag");
	std::string fragmentSrc((std::istreambuf_iterator<char>(fragmentStream)), std::istreambuf_iterator<char>());

	_Shader.reset(Shader::Create());
	
	if (_Shader->Build(vertexSrc, fragmentSrc) == false)
	{
		OSR_ERROR("Unable to build shader.");
		return false;
	}
	_Shader->Bind();

	/* Set the shaders uniforms  */
	_Shader->SetVPMatrix(glm::identity<glm::mat4>());
	_Shader->SetUniformVec3("blendColor", glm::vec3{ 0.8f, 0.2f, 0.2f });

	/* Create and bind a default VAO */
	_VertexArray.reset(VertexArray::Create());
	_VertexArray->Bind();

	/* define data for a simple rectangle with matching indices */
	float vertices[8] =
	{
		-0.5f, -0.5f,
		-0.5f,  0.5f,
		0.5f,   0.5f,
		0.5f,  -0.5f
	};

	uint32_t indices[6] = 
	{ 
		0, 1, 2, 2, 3, 0 
	};

	/* Create a new Vertex and Index buffer on the GPU */
	_VertexBuffer.reset(VertexBuffer::Create(&vertices[0], sizeof(vertices), "Test VBO"));
	_IndexBuffer.reset(IndexBuffer::Create(&indices[0], sizeof(indices) / sizeof(uint32_t)));

	/* Setup the Vertex array attribute data */
	_VertexArray->SetAttribute(0, 0, 2);

	return true;
}

void PlaygroundLayer::OnDetach()
{

}

void PlaygroundLayer::OnUpdate(Timestep ts)
{

}

void PlaygroundLayer::OnRender(Timestep ts, Renderer& renderer)
{
	/* Draw the rectangle buffers */
	renderer.DrawElements(RendererDrawType::Triangles, _IndexBuffer->GetCount());
}