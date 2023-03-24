#pragma once

/* core wyrd includes */
#include <core/export.h>
#include <core/Timestep.h>
#include <core/renderer/Renderer.h>
#include <core/renderer/Buffer.h>
#include <core/renderer/VertexArray.h>

/* local includes */
#include "datamodels/OrthographicCameraController.h"
#include "events/EditorEvents.h"
#include "services/ServiceManager.h"

namespace Wyrd::Editor
{
	class SceneViewer;

	class Gizmo
	{
	public:
		typedef std::function<void(int)> MouseEnterFunc;
		typedef std::function<void(int)> MouseExitFunc;
		typedef std::function<void(int)> MouseButtonPressedFunc;
		typedef std::function<void(int)> MouseButtonReleaseFunc;

	public:
		struct Region
		{
			Rect area;
			int index;
			int vertexOffset;
			Color color;
			Color hoverColor;
			MouseEnterFunc mouseEnterFunc;
			MouseExitFunc mouseExitFunc;
			MouseButtonPressedFunc mouseButtonPressedFunc;
			MouseButtonReleaseFunc mouseButtonReleasedFunc;
		};

	public:
		Gizmo(SceneViewer* sceneViewer, Entity entity);
		virtual ~Gizmo() = default;

		virtual void Render(Timestep ts, Renderer& renderer) = 0;
		virtual void OnEvent(Event& event) = 0;

	protected:
		std::shared_ptr<OrthographicCameraController> _CameraController;
		SceneViewer* _SceneViewer;

		std::shared_ptr<EventService>		_EventService;
		Entity		_Entity;

		std::shared_ptr<VertexArray>	_VertexArray;
		std::shared_ptr<VertexBuffer>	_VertexBuffer;
		std::shared_ptr<IndexBuffer>	_IndexBuffer;

		std::vector<Vertex2D> _Vertices;
		std::vector<uint32_t> _Indices;
		std::map<std::string, std::pair<uint32_t, uint32_t>> _VertexGroups;
	};
}