#pragma once

#include "core/pipeline/OrthographicCamera.h"
#include "core/Timestep.h"

#include "events/Event.h"
#include "events/MouseEvent.h"
#include "events/ApplicationEvent.h"

namespace Osiris::Editor
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		inline OrthographicCamera& GetCamera() { return _Camera; }
		inline const OrthographicCamera& GetCamera() const { return _Camera; }

		inline float GetZoomLevel() const { return _ZoomLevel; }
		inline void SetZoomLevel(float level) { _ZoomLevel = level; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float _AspectRatio;
		float _ZoomLevel = 1.0f;
		OrthographicCamera _Camera;

		bool _Rotation;

		glm::vec3 _CameraPosition = { 0.0f, 0.0f, 0.0f };
		float _CameraRotation = 0.0f;
		float _InitialCameraTranslationSpeed = 5.0f, _InitialCameraRotationSpeed = 180.0f, _InitialCameraZoomSpeed = 0.75f;
		float _CameraTranslationSpeed = _InitialCameraTranslationSpeed;
		float _CameraRotationSpeed = _InitialCameraRotationSpeed;
		float _CameraZoomSpeed = _InitialCameraZoomSpeed;
	};
}
