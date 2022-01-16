#pragma once

/* core wyrd includes */
#include <core/export.h>
#include <core/Timestep.h>

/* local includes */
#include "gizmos/GizmoHandle.h"
#include "events/EditorEvents.h"
#include "events/MouseEvent.h"
#include "events/KeyEvent.h"

namespace Wyrd::Editor
{
	class SceneViewer;
	class Gizmo;

	class GizmoHandle
	{
	public:
		GizmoHandle(SceneViewer* sceneViewer, glm::vec2 position, glm::vec2 size, Gizmo* parent, glm::mat4 matrix)
			: area(position, size), matrix(matrix), parent(parent), _SceneViewer(sceneViewer), _Entity(ENTITY_INVALID) {}

		~GizmoHandle() = default;

		inline std::function<bool(MouseButtonPressedEvent&)> GetMouseButtonPressedCallback() const { return _MouseButtonPressedCallback; };
		inline std::function<bool(MouseButtonReleasedEvent&)> GetMouseButtonReleaseCallback() const { return _MouseButtonReleasedCallback; };
		inline std::function<bool(MouseMovedEvent&)> GetMouseMovedCallback() const { return _MouseMovedCallback; };

		inline void SetMouseButtonPressedCallback(std::function<bool(MouseButtonPressedEvent&)> callback) { _MouseButtonPressedCallback = callback; }
		inline void SetMouseButtonReleaseCallback(std::function<bool(MouseButtonReleasedEvent&)> callback) { _MouseButtonReleasedCallback = callback; }
		inline void SetMouseMovedCallback(std::function<bool(MouseMovedEvent&)> callback) { _MouseMovedCallback = callback; }

		bool ContainsPoint(glm::vec2 worldSpacePt);

		Wyrd::Rect		area;
		glm::mat4       matrix;
		Gizmo*			parent;

	private:
		SceneViewer*	_SceneViewer;
		Entity			_Entity;

		std::function<bool(MouseButtonPressedEvent&)> _MouseButtonPressedCallback;
		std::function<bool(MouseButtonReleasedEvent&)> _MouseButtonReleasedCallback;
		std::function<bool(MouseScrolledEvent&)> _MouseScrollCallback;
		std::function<bool(MouseMovedEvent&)> _MouseMovedCallback;
		std::function<bool(KeyPressedEvent&)> _KeyPressedCallback;
		std::function<bool(KeyReleasedEvent&)> _KeyReleasedCallback;
	};
}