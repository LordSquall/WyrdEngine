#pragma once

/* core wyrd includes */
#include <core/export.h>
#include <core/Timestep.h>
#include <core/renderer/Renderer.h>

/* local includes */
#include "gizmos/Gizmo.h"

namespace Wyrd::Editor
{
	class SceneViewer;

	class Rotate2DGizmo : public Gizmo
	{
	public:
		Rotate2DGizmo(SceneViewer* sceneViewer);
		~Rotate2DGizmo();

		void Render(Timestep ts, Renderer& renderer) override;
		void OnEvent(Event& event) override;

		void OnSelectedEntityChanged(Events::EventArgs& args);

	private:
		void Build();

	private:
		bool _XYMoveActive;
		glm::vec2 _LastMouseWorldPos;
	};
}