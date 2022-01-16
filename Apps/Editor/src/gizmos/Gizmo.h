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
#include "gizmos/GizmoHandle.h"
#include "services/ServiceManager.h"

namespace Wyrd::Editor
{
	class SceneViewer;

	class Gizmo
	{
	public:
		Gizmo(SceneViewer* sceneViewer, Entity entity);
		virtual ~Gizmo() = default;

		void OnEvent(Event& event);

		virtual void Render(Timestep ts, Renderer& renderer) = 0;

	protected:
		GizmoHandle& CreateHandle(glm::vec2 position, glm::vec2 size, Gizmo* parent, glm::mat4 matrix);

		bool OnMouseButtonPressedEventProxy(MouseButtonPressedEvent& e, void* data);
		bool OnMouseButtonReleasedEventProxy(MouseButtonReleasedEvent& e, void* data);
		bool OnMouseMovedEventProxy(MouseMovedEvent& e, void* data);

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

		std::vector<GizmoHandle> _Handles;
	};
}