#pragma once

#include <memory>

#include <core/Layer.h>
#include <core/renderer/Shader.h>
#include <core/renderer/Buffer.h>
#include <core/renderer/VertexArray.h>
#include <core/pipeline/OrthographicCamera.h>
#include <core/ecs/ECS.h>
#include <events/KeyEvent.h>
#include <events/MouseEvent.h>

using namespace Wyrd;

class ControlLayer : public Layer
{
public:
	ControlLayer() : Layer("ControlLayer"), baseDirectory(""), _Camera(), _CameraEntity(ENTITY_INVALID)
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
	std::string baseDirectory;

	bool LoadCoreFile();
	bool LoadGameFile();
	bool LoadCommonBundleFile();

private:
	Wyrd::OrthographicCamera _Camera;
	Entity _CameraEntity;
};