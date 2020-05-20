#pragma once

/* Core includes */
#include <Osiris.h>
#include "core/pipeline/OrthographicCamera.h"
#include "events/MouseEvent.h"
#include "events/KeyEvent.h"
#include "events/ApplicationEvent.h"

namespace Osiris::Editor
{
	/* Editor Orthographic Camera Controller */
	class OrthographicCameraController
	{
	public:
		/* Constructors */
		OrthographicCameraController(float aspectRatio, float zoom);

	public:
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		/* Getters and Setters */
		inline OrthographicCamera& GetCamera() { return _Camera; }
		inline const OrthographicCamera& GetCamera() const { return _Camera; }
		inline glm::vec3 GetPosition() const { return _Camera.GetPosition(); }
		inline void SetPosition(glm::vec3 position) { _Camera.SetPosition(position); }
		inline float GetZoomLevel() const { return _ZoomLevel; }
		inline void SetZoomLevel(float level) { _ZoomLevel = level; _Camera.SetProjection(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel);}
		inline float GetAspectRatio() const { return _AspectRatio; }

		void Translate(glm::vec2 delta);

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);

	private:
		float _AspectRatio;
		float _ZoomLevel;
		OrthographicCamera _Camera;

		Timestep _timestep;
		
		glm::vec2 _CameraPositionVelocity = { 0.0f, 0.0f };
		float _InitialCameraTranslationSpeed = 5.0f, _InitialCameraRotationSpeed = 180.0f, _InitialCameraZoomSpeed = 0.75f;
		float _CameraTranslationSpeed = _InitialCameraTranslationSpeed;
		float _CameraZoomSpeed = _InitialCameraZoomSpeed;
	};
}
