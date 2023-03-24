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

	class Scale2DGizmo : public Gizmo
	{
	public:
		Scale2DGizmo(SceneViewer* sceneViewer);
		~Scale2DGizmo();

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