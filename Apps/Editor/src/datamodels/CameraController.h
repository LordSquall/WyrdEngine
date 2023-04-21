#pragma once

/* core wyrd includes */
#include <core/Timestep.h>
#include <events/MouseEvent.h>
#include <events/KeyEvent.h>
#include <events/ApplicationEvent.h>

/* local includes */
#include "core/pipeline/Camera.h"

namespace Wyrd::Editor
{
	/* Editor Camera Controller */
	class CameraController
	{
	public:
		/* Constructors */
		CameraController();

	public:
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		/* Getters and Setters */
		inline Camera& GetCamera() { return _Camera; }
		inline const Camera& GetCamera() const { return _Camera; }
		
		void SetViewportSize(float width, float height);

		inline glm::vec3 GetPosition() const { return _Camera.GetPosition(); }
		inline void SetPosition(glm::vec3 position) { _Camera.SetPosition(position); }

		inline void SetMode(const Camera::Mode mode) { _Camera.SetMode(mode); }


		void Translate(glm::vec2 delta);
		
	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e, void* data);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e, void* data);
		bool OnMouseMoved(MouseMovedEvent& e, void* data);
		bool OnMouseScrolled(MouseScrolledEvent& e, void* data);
		bool OnWindowResized(WindowResizeEvent& e, void* data);
		bool OnKeyPressed(KeyPressedEvent& e, void* data);
		bool OnKeyReleased(KeyReleasedEvent& e, void* data);

		enum class Mode { None, Pan, Pivot } _TransformMode;
	private:
		Camera _Camera;

		float _PanSensitivity;
		float _PivotSensitivity;

		float _InitialXPos;
		float _InitialYPos;

		bool _LeftShift;
	};
}
