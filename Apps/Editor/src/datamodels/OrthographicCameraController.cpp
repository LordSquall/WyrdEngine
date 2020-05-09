#pragma once
#include "osrpch.h"

/* Local includes */
#include "OrthographicCameraController.h"

namespace Osiris::Editor {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, float zoomLevel) : _AspectRatio(aspectRatio)
	{
		_Camera.SetProjection(-_AspectRatio * zoomLevel, _AspectRatio * zoomLevel, -zoomLevel, zoomLevel);
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		glm::vec3 position = _Camera.GetPosition();
		position.x += (_CameraPositionVelocity.x * ts);
		position.y += (_CameraPositionVelocity.y * ts);

		_Camera.SetPosition(position);

		_Camera.RecalulateViewProjection();
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(OSR_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(OSR_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
		dispatcher.Dispatch<KeyPressedEvent>(OSR_BIND_EVENT_FN(OrthographicCameraController::OnKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(OSR_BIND_EVENT_FN(OrthographicCameraController::OnKeyReleased));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		_ZoomLevel -= e.GetYOffset() * _CameraZoomSpeed;
		_ZoomLevel = std::max(_ZoomLevel, _CameraZoomSpeed);

		_Camera.SetProjection(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel);

		_InitialCameraTranslationSpeed = _ZoomLevel;
		
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		_Camera.SetProjection(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel);

		return false;
	}


	bool OrthographicCameraController::OnKeyPressed(KeyPressedEvent& e)
	{
		float translationSpeed = _CameraTranslationSpeed;

		glm::vec2 velocity = { 0.0f, 0.0f };

		if (Input::IsKeyPressed(OSR_KEY_LEFT_SHIFT))
		{
			_CameraTranslationSpeed = _InitialCameraTranslationSpeed * 4.0f;
			_CameraZoomSpeed = _InitialCameraZoomSpeed * 4.0f;
		}
		else
		{
			_CameraTranslationSpeed = _InitialCameraTranslationSpeed;
			_CameraZoomSpeed = _InitialCameraZoomSpeed;
		}

		switch (e.GetKeyCode())
		{
		case OSR_KEY_A:
			velocity.x -= _CameraTranslationSpeed;
			break;
		case OSR_KEY_D:
			velocity.x += _CameraTranslationSpeed;
			break;
		case OSR_KEY_W:
			velocity.y += _CameraTranslationSpeed;
			break;
		case OSR_KEY_S:
			velocity.y -= _CameraTranslationSpeed;
			break;
		}

		_CameraPositionVelocity = velocity;

		return true;
	}

	bool OrthographicCameraController::OnKeyReleased(KeyReleasedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case OSR_KEY_A:
		case OSR_KEY_D:
		case OSR_KEY_W:
		case OSR_KEY_S:
			_CameraPositionVelocity.x = 0.0f;
			_CameraPositionVelocity.y = 0.0f;
			break;
		}

		return true;
	}

}