#pragma once

#include <memory>

#include "core/Layer.h"
#include "core/renderer/Shader.h"
#include "core/renderer/Buffer.h"
#include "core/renderer/VertexArray.h"

using namespace Wyrd;

class TestLayer : public Layer
{
public:
	TestLayer() : Layer("TestLayer")
	{

	}

	bool OnAttach() override;

	void OnDetach() override;

	void OnUpdate(Timestep ts) override;

	void OnRender(Timestep ts, Renderer& renderer) override;
};