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

		//bool OnMouseMovedEvent(MouseMovedEvent& e);
		//bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		//bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);

		void OnSelectedEntityChanged(Events::EventArgs& args);

	private:
		void Build();

	private:
		bool _XYMoveActive;
		glm::vec2 _LastMouseWorldPos;
	};
}