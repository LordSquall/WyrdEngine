#pragma once

/* core wyrd includes */
#include <core/export.h>
#include <core/Timestep.h>
#include <core/renderer/Renderer.h>

/* local includes */
#include "gizmos/Gizmo.h"

namespace Wyrd::Editor
{
	/* Declared Classes */
	class SceneViewer;

	class Translate2DGizmo : public Gizmo
	{
	public:
		Translate2DGizmo(SceneViewer* sceneViewer);
		~Translate2DGizmo();

		void Render(Timestep ts, Renderer& renderer) override;
		void OnEvent(Event& event) override;

		/* Game events */
		void OnSelectedEntityChanged(Events::EventArgs& args);

		/* Input events */
		bool OnMouseMovedEvent(MouseMovedEvent& e, void* data);
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e, void* data);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e, void* data);

	private:
		void Build();

	private:
		bool _DragActive;
		Vector2 _StartDragPosition;

		std::vector<Region> _Regions;
		int _ActiveRegion;
	};
}