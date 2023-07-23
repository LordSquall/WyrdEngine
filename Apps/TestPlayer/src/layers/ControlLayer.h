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
		std::string gameDir;
	};
};

class ControlLayer : public Layer
{
public:
	ControlLayer(const LayerProps& props) : Layer("ControlLayer"), _layerProps(props), _Camera(), _CameraEntity(ENTITY_INVALID)
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

public:
	bool LoadCoreFile();
	bool LoadGameFile();
	bool LoadCommonBundleFile();

private:
	LayerProps _layerProps;

	Wyrd::Camera _Camera;
	Entity _CameraEntity;
};