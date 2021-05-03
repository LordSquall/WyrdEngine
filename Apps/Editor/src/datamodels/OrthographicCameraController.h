#pragma once

/* core wyrd includes */
#include <core/Timestep.h>
#include <events/MouseEvent.h>
#include <events/KeyEvent.h>
#include <events/ApplicationEvent.h>

/* local includes */
#include "core/pipeline/OrthographicCamera.h"

namespace Wyrd::Editor
{
	/* Editor Orthographic Camera Controller */
	class OrthographicCameraController
	{
	public:
		/* Constructors */
		OrthographicCameraController();

	public:
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		/* Getters and Setters */
		inline OrthographicCamera& GetCamera() { return _Camera; }
		inline const OrthographicCamera& GetCamera() const { return _Camera; }
		
		inline void SetViewportSize(float width, float height) { _Camera.SetViewportSize(width, height); }

		inline glm::vec3 GetPosition() const { return _Camera.GetPosition(); }
		inline void SetPosition(glm::vec3 position) { _Camera.SetPosition(position); }

		inline float GetSize() const { return _Camera.GetSize(); }
		inline void SetSize(float size) { _Camera.SetSize(size); }

		void Translate(glm::vec2 delta);
		
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);

	private:
		OrthographicCamera _Camera;

		Timestep _timestep;
		
		glm::vec2 _CameraPositionVelocity = { 0.0f, 0.0f };
		float _InitialCameraTranslationSpeed = 5.0f, _InitialCameraRotationSpeed = 180.0f, _InitialCameraZoomSpeed = 0.75f;
		float _CameraTranslationSpeed = _InitialCameraTranslationSpeed;
		float _CameraZoomSpeed = _InitialCameraZoomSpeed;
	};
}
