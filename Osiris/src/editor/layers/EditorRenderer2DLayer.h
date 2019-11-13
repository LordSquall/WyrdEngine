#pragma once

#include <Osiris.h>

#include "editor/events/EditorEvents.h"
#include "layers/Renderer2DLayer.h"

using namespace Osiris;

namespace Osiris::Editor
{
	class EditorRenderer2DLayer : public Layers::Renderer2DLayer
	{
	public:
		EditorRenderer2DLayer(std::string name);

		void OnRender(Renderer& renderer) override;

	private:
		std::shared_ptr<Scene> _Scene;

		void OnSceneOpened(Events::EventArgs& args);
	};
}