#pragma once

#include <Osiris.h>

#ifdef OSR_EDITOR_ENABLED
#include "editor/layers/EditorLayer.h"
#endif

using namespace Osiris;

class ExampleLayer : public Layer
{
public:
	ExampleLayer() : Layer("Example Layer")
	{

	}

	void OnAttach() override
	{
	}

	void OnDetach() override
	{
	}

	void OnUpdate() override
	{
		
	}

	void OnEvent(Event& event) override
	{
	}

	void OnRender(std::shared_ptr<Renderer> renderer) override
	{

	}
};