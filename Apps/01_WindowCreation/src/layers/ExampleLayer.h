#pragma once

#include <memory>

#include "core/Layer.h"
#include "core/renderer/Shader.h"
#include "core/renderer/Buffer.h"
#include "core/renderer/VertexArray.h"

using namespace Osiris;

class ExampleLayer : public Layer
{
public:
	ExampleLayer() : Layer("ExampleLayer")
	{

	}

	bool OnAttach() override;

	void OnDetach() override;

	void OnUpdate(Timestep ts) override;

	void OnRender(Timestep ts, Renderer& renderer) override;

private:
	std::shared_ptr<Shader> _Shader;
	std::unique_ptr<VertexArray> _VertexArray;
	std::unique_ptr<VertexBuffer> _VertexBuffer;
	std::unique_ptr<IndexBuffer> _IndexBuffer;

	int tempId;
};