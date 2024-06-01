#pragma once

/* core includes */
#include <wyrdpch.h>
#include <core/export.h>
#include <core/Timestep.h>
#include <core/systems/ISystem.h>
#include <core/renderer/Renderer.h>

#include "datamodels/CameraController.h"

namespace Wyrd::Editor
{
	struct SceneRenderSystemContext : public ISystemContext
	{
		Renderer& renderer;
		CameraController& cameraController;

		SceneRenderSystemContext(Scene& s, Renderer& renderer, CameraController& cameraController) : ISystemContext(s), renderer(renderer), cameraController(cameraController) { };
	};


	class SceneRenderSystem : public ISystem
	{
	public:
		virtual bool OnUpdate(ISystemContext& context);
	};
}