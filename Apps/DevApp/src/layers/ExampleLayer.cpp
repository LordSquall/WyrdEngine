#pragma once

#include "ExampleLayer.h"

void ExampleLayer::OnAttach()
{

	/* Read a file */
	/* Load resources (e.g. sprites, sounds, input mappings) */
	/* Initialise layer behvaiours */
	/* Finalise to orisis */

	/* create a default vertex array */
	_VertexArray.reset(VertexArray::Create());

	float vertices[3 * 2];
	
	switch (tempId)
	{
	case 0:
		vertices[0] = -0.9f; vertices[1] = 0.1f;
		vertices[2] = -0.5f; vertices[3] = 0.9f;
		vertices[4] = -0.1f; vertices[5] = 0.1f;
		break;
	case 1:
		vertices[0] = 0.1f; vertices[1] = 0.1f;
		vertices[2] = 0.5f; vertices[3] = 0.9f;
		vertices[4] = 0.9f; vertices[5] = 0.1f;
		break;
	case 2:
		vertices[0] = -0.9f; vertices[1] = -0.9f;
		vertices[2] = -0.5f; vertices[3] = -0.1f;
		vertices[4] = -0.1f; vertices[5] = -0.9f;
		break;
	case 3:
		vertices[0] = 0.1f; vertices[1] = -0.9f;
		vertices[2] = 0.5f; vertices[3] = -0.1f;
		vertices[4] = 0.9f; vertices[5] = -0.9f;
		break;
	}

	_VertexBuffer.reset(VertexBuffer::Create(&vertices[0], sizeof(vertices), "examplelayer"));

	_VertexArray->SetAttribute(0, 0, 2);

	uint32_t indices[3] = { 0, 1, 2 };
	
	_IndexBuffer.reset(IndexBuffer::Create(&indices[0], sizeof(indices) / sizeof(uint32_t)));
}

void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate(Timestep ts)
{

}

void ExampleLayer::OnEvent(Event& event)
{

}

void ExampleLayer::OnRender(Timestep ts, Renderer& renderer)
{
	_VertexArray->Bind();
	_IndexBuffer->Bind();
	_VertexBuffer->Bind();
	renderer.DrawElements(RendererDrawType::Triangles, _IndexBuffer->GetCount());
}