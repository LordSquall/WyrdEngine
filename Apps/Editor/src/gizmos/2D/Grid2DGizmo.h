/* core wyrd includes */
#include <core/export.h>
#include <core/Timestep.h>
#include <core/Application.h>
#include <core/Resources.h>
#include <core/renderer/Renderer.h>

/* local includes */
#include "gizmos/Gizmo.h"
#include "datamodels/CameraController.h"

namespace Wyrd::Editor
{
	class Grid2DGizmo : public Gizmo
	{
	public:
		Grid2DGizmo(SceneViewer* sceneViewer);
		~Grid2DGizmo();

		void Render(Timestep ts, Renderer& renderer) override;
		void OnEvent(Event& event) override;

		void OnSettingsChanged(Events::EventArgs& args);

		inline const bool IsEnabled() const { return _Enabled; }
		inline void ToggleEnabled() { _Enabled = !_Enabled; }

		inline void SetColor(Color& color) { _Color = color; }
		inline const Color& GetColor() const { return _Color; }

		bool OnMouseMovedEvent(MouseMovedEvent& e, void* data);
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e, void* data);

	private:
		void BuildGrid();

	private:
		std::shared_ptr<CameraController> _CameraController;

		bool _Enabled;
		Color _Color;
		glm::vec4 cursorPosition;
	};
}