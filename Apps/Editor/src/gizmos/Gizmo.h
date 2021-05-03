#pragma once

/* core wyrd includes */
#include <core/export.h>
#include <core/Timestep.h>
#include <core/renderer/Renderer.h>
#include <core/renderer/Shader.h>
#include <core/renderer/Buffer.h>
#include <core/renderer/VertexArray.h>
#include <core/scene/GameObject.h>

/* local includes */
#include "gizmos/GizmoVertex.h"
#include "datamodels/OrthographicCameraController.h"
#include "events/EditorEvents.h"

namespace Wyrd::Editor
{
	class SceneViewer;

	class Gizmo
	{
	public:
		Gizmo(SceneViewer* sceneViewer, Shader* shader);
		virtual ~Gizmo() = default;

		virtual void Render(Timestep ts, Renderer& renderer) = 0;

	protected:
		std::shared_ptr<OrthographicCameraController> _CameraController;
		SceneViewer* _SceneViewer;

		Shader*			_Shader;
		std::shared_ptr<VertexArray>	_VertexArray;
		std::shared_ptr<VertexBuffer>	_VertexBuffer;
		std::shared_ptr<IndexBuffer>	_IndexBuffer;

		std::vector<GizmoVertex> _Vertices;
		std::vector<uint32_t> _Indices;
		std::map<std::string, std::pair<uint32_t, uint32_t>> _VertexGroups;
	};
}