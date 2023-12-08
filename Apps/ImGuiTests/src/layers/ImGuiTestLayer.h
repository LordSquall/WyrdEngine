#pragma once

#include <memory>

#include <core/Application.h>
#include <core/Layer.h>
#include <core/renderer/Shader.h>
#include <core/renderer/Buffer.h>
#include <core/renderer/VertexArray.h>
#include <core/pipeline/Camera.h>
#include <core/ecs/ECS.h>
#include <events/KeyEvent.h>
#include <events/MouseEvent.h>

using namespace Wyrd;

namespace Wyrd
{
	struct LayerProps : AppProps
	{
		int keepAlive;
	};
};

class ImGuiTestLayer : public Layer
{
public:
	ImGuiTestLayer(const LayerProps& props) : Layer("ImGuiTestLayer"), _layerProps(props)
	{

	}

	bool OnAttach() override;
	void OnDetach() override;
	void OnUpdate(Timestep ts) override;
	void OnRender(Timestep ts, Renderer& renderer) override;
	void OnEvent(Event& event) override;

private:
	bool OnKeyReleasedEvent(KeyReleasedEvent& e, void* data);
	bool OnKeyPressedEvent(KeyPressedEvent& e, void* data);
	bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e, void* data);
	bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e, void* data);
	bool OnMouseMovedEvent(MouseMovedEvent& e, void* data);

private:
	LayerProps _layerProps;
};