#pragma once

#include <memory>

#include "core/Layer.h"
#include "core/renderer/Shader.h"
#include "core/renderer/Buffer.h"
#include "core/renderer/VertexArray.h"

using namespace Wyrd;

class ControlLayer : public Layer
{
public:
	ControlLayer() : Layer("ControlLayer"), baseDirectory("")
	{

	}

	bool OnAttach() override;

	void OnDetach() override;

	void OnUpdate(Timestep ts) override;

	void OnRender(Timestep ts, Renderer& renderer) override;

public:
	std::string baseDirectory;

	bool LoadCoreFile();
	bool LoadGameFile();
	bool LoadCommonBundleFile();
};