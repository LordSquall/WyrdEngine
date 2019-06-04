#pragma once

#include <Osiris.h>

using namespace Osiris;

class ExampleLayer : public Layer
{
public:
	ExampleLayer() : Layer("Example Layer")
	{

	}

	void OnAttach() override;

	void OnDetach() override;

	void OnUpdate() override;

	void OnEvent(Event& event) override;

	void OnRender(Renderer& renderer) override;

private:
	std::unique_ptr<VertexArray> _VertexArray;
	std::unique_ptr<VertexBuffer> _VertexBuffer;
	std::unique_ptr<IndexBuffer> _IndexBuffer;
};