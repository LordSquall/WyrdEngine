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
#include "gizmos/Gizmo.h"
#include "services/ServiceManager.h"
#include "datamodels/OrthographicCameraController.h"
#include "events/EditorEvents.h"

namespace Wyrd::Editor
{
	class SceneViewer;

	class TranslationGizmo : public Gizmo
	{
		enum class InputState
		{
			NONE,
			OVER,
			ACTIVE
		};

		enum class AxisState
		{
			NONE,
			X,
			Y,
			XY
		};

		enum class MovementState
		{
			LOCAL,
			GLOBAL
		};

	public:
		TranslationGizmo(SceneViewer* sceneViewer, Shader* shader);
		~TranslationGizmo();

		void Render(Timestep ts, Renderer& renderer) override;

		void OnEvent(Event& event);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);

		void OnSelectedGameObjectChanged(Events::EventArgs& args);

	private:
		void SetInputState(InputState state);
		void SetAxisState(AxisState state);
		void SetMovementState(MovementState state);
		void UpdateVertexGroupColor(const std::string& groupName, const Color& color);

	private:
		std::shared_ptr<EventService>		_EventService;
		GameObject*		_GameObject;

		InputState _InputState;
		AxisState _AxisState;
		MovementState _MovementState;
		glm::vec2 _LastMouseWorldPos;
	};
}