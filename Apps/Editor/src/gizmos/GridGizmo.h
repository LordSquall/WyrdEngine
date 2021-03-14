/* core osiris includes */
#include <core/export.h>
#include <core/Timestep.h>
#include <core/Application.h>
#include <core/Resources.h>
#include <core/renderer/Renderer.h>
#include <core/renderer/Shader.h>
#include <core/renderer/Buffer.h>
#include <core/renderer/VertexArray.h>
#include <core/scene/GameObject.h>


/* local includes */
#include "gizmos/GizmoVertex.h"
#include "services/ServiceManager.h"
#include "datamodels/OrthographicCameraController.h"
#include "support/IconLibrary.h"
#include "events/EditorEvents.h"

namespace Osiris::Editor
{
	class GridGizmo
	{
	public:
		GridGizmo(std::shared_ptr<OrthographicCameraController> cameraController);
		~GridGizmo();

		void Render(Timestep ts, Renderer& renderer);

		void OnSettingsChanged(Events::EventArgs& args);

		inline const bool IsEnabled() const { return _Enabled; }
		inline void ToggleEnabled() { _Enabled = !_Enabled; }

		inline void SetColor(Color& color) { _Color = color; }
		inline const Color& GetColor() const { return _Color; }

	private:
		void BuildGrid();

	private:
		std::shared_ptr<EventService>		_EventService;
		std::shared_ptr<SettingsService>	_SettingsService;

		std::shared_ptr<OrthographicCameraController> _CameraController;
		std::shared_ptr<GameObject>		_GameObject;

		std::shared_ptr<Shader>			_Shader;
		std::shared_ptr<VertexArray>	_VertexArray;
		std::shared_ptr<VertexBuffer>	_VertexBuffer; 
		std::shared_ptr<IndexBuffer>	_IndexBuffer;

		std::vector<GizmoVertex> _Vertices;
		std::vector<uint32_t> _Indices;

		bool _Enabled;
		Color _Color;
	};
}