#pragma once

#include "ExampleLayer.h"

void ExampleLayer::OnAttach()
{
	/* create a default vertex array */
	_VertexArray.reset(VertexArray::Create());

	float vertices[3 * 3] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	_VertexBuffer.reset(VertexBuffer::Create(&vertices[0], sizeof(vertices)));

	_VertexArray->SetAttribute(0, 0, 3);

	unsigned int indices[3] = { 0, 1, 2 };

	_IndexBuffer.reset(IndexBuffer::Create(&indices[0], sizeof(indices) / sizeof(uint32_t)));
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnUpdate()
{

}

void ExampleLayer::OnEvent(Event& event)
{
}

void ExampleLayer::OnRender(Renderer& renderer)
{
	renderer.DrawElements(RendererDrawType::Triangles, _IndexBuffer->GetCount());
}