#pragma once

/* core wyrd includes */
#include <core/export.h>
#include <core/Timestep.h>
#include <core/renderer/Renderer.h>
#include <core/renderer/Buffer.h>
#include <core/renderer/VertexArray.h>
#include <core/maths/Rect.h>

/* local includes */
#include "gizmos/Gizmo.h"
#include "views/SceneViewer/SceneViewer.h"
#include "datamodels/EditorComponents.h"

namespace Wyrd::Editor
{
	Gizmo::Gizmo(SceneViewer* sceneViewer, Entity entity) : _SceneViewer(sceneViewer), _Entity(entity)
	{
		_CameraController = sceneViewer->GetCamera();
	}

	void Gizmo::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		for (auto handle : _Handles)
		{
			dispatcher.Dispatch<MouseButtonPressedEvent>(WYRD_BIND_EVENT_FN(Gizmo::OnMouseButtonPressedEventProxy), (void*)&handle);
			dispatcher.Dispatch<MouseButtonReleasedEvent>(WYRD_BIND_EVENT_FN(Gizmo::OnMouseButtonReleasedEventProxy), (void*)&handle);
		}
	}

	GizmoHandle& Gizmo::CreateHandle(glm::vec2 position, glm::vec2 size, Gizmo* parent, glm::mat4 matrix)
	{
		_Handles.push_back(GizmoHandle(_SceneViewer, position, size, parent, matrix));
		return _Handles[_Handles.size() - 1];
	}

	bool Gizmo::OnMouseButtonPressedEventProxy(MouseButtonPressedEvent& e, void* data)
	{
		GizmoHandle* handle = (GizmoHandle*)data;

		/* build the initial mouse position vector */
		glm::vec2 mousePos = { e.GetPositionX(), e.GetPositionY() };

		/* viewport mouse position */
		glm::vec2 viewportPos = _SceneViewer->GetViewportSpaceFromPoint(mousePos);

		/* we only want to process mouse events within the viewport of the scene */
		glm::vec2 ws = _SceneViewer->Convert2DToWorldSpace(mousePos);

		if (handle->ContainsPoint(ws))
		{
			handle->GetMouseButtonPressedCallback()(e);
		}

		return true;
	}

	bool Gizmo::OnMouseButtonReleasedEventProxy(MouseButtonReleasedEvent& e, void* data)
	{
		GizmoHandle* handle = (GizmoHandle*)data;

		handle->GetMouseButtonReleaseCallback()(e);

		return true;
	}

	bool Gizmo::OnMouseMovedEventProxy(MouseMovedEvent& e, void* data)
	{
		GizmoHandle* handle = (GizmoHandle*)data;

		return true;
	}
}