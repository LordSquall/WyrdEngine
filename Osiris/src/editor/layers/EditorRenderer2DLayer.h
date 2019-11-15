#pragma once

#include <Osiris.h>

#include "editor/events/EditorEvents.h"
#include "layers/Renderer2DLayer.h"
#include "editor/datamodels/OrthographicCameraController.h"

using namespace Osiris;

namespace Osiris::Editor
{
	class EditorRenderer2DLayer : public Layers::Renderer2DLayer
	{
	public:
		EditorRenderer2DLayer(std::string name);

		void OnRender(Timestep ts, Renderer& renderer) override;

		void OnEvent(Event& event) override;

	private:
		std::shared_ptr<OrthographicCameraController> _CameraController;
		std::shared_ptr<Scene> _Scene;

		void OnSceneOpened(Events::EventArgs& args);
	};
}