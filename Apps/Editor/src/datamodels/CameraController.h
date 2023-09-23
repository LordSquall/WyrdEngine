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

		enum class Mode { None, Pan, Pivot } _TransformMode;

		/* Getters and Setters */
		inline Camera& GetCamera() { return _Camera; }
		inline const Camera& GetCamera() const { return _Camera; }
		
		void SetViewportSize(float width, float height);

		inline glm::vec3 GetPosition() const { return _Camera.GetPosition(); }
		inline void SetPosition(glm::vec3 position) { _Camera.SetPosition(position); }

		inline glm::vec3 GetOrientation() const { return _Camera.GetOrientation(); }
		inline void SetOrientation(glm::vec3 orientation) { _Camera.SetOrientation(orientation); }

		inline void SetMode(const Camera::Mode mode) { _Camera.SetMode(mode); }
		inline const CameraController::Mode GetTransformMode() const { return _TransformMode; }


		float _InitialPitch;
		float _InitialYaw;
		float _PivotPitchDelta;
		float _PivotYawDelta;

	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e, void* data);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e, void* data);
		bool OnMouseMoved(MouseMovedEvent& e, void* data);
		bool OnMouseScrolled(MouseScrolledEvent& e, void* data);
		bool OnWindowResized(WindowResizeEvent& e, void* data);
		bool OnKeyPressed(KeyPressedEvent& e, void* data);
		bool OnKeyReleased(KeyReleasedEvent& e, void* data);

		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);

		float RotationSpeed() const;
		float ZoomSpeed() const;
		float PanSpeed() const;

	private:
		Camera _Camera;

		float _PanSensitivity;
		float _PivotSensitivity;

		float _PrevXPos;
		float _PrevYPos;

		bool _LeftShift;
	};
}
