#pragma once
#include "osrpch.h"

/* Local includes */
#include "OrthographicCameraController.h"

namespace Osiris::Editor {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: _AspectRatio(aspectRatio), _Camera(-_AspectRatio * _ZoomLevel, _AspectRatio* _ZoomLevel, -_ZoomLevel, _ZoomLevel), _Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		_CameraPosition.x += (_CameraPositionVelocity.x * ts);
		_CameraPosition.y += (_CameraPositionVelocity.y * ts);

		_Camera.SetPosition(_CameraPosition);
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
			_CameraRotationSpeed = _InitialCameraRotationSpeed * 4.0f;
			_CameraZoomSpeed = _InitialCameraZoomSpeed * 4.0f;
		}
		else
		{
			_CameraTranslationSpeed = _InitialCameraTranslationSpeed;
			_CameraRotationSpeed = _InitialCameraRotationSpeed;
			_CameraZoomSpeed = _InitialCameraZoomSpeed;
		}

		switch (e.GetKeyCode())
		{
		case OSR_KEY_A:
			velocity.x += -(cos(glm::radians(_CameraRotation)) * _CameraTranslationSpeed);
			velocity.y += -(sin(glm::radians(_CameraRotation)) * _CameraTranslationSpeed);
			break;
		case OSR_KEY_D:
			velocity.x += cos(glm::radians(_CameraRotation)) * _CameraTranslationSpeed;
			velocity.y += sin(glm::radians(_CameraRotation)) * _CameraTranslationSpeed;
			break;
		case OSR_KEY_W:
			velocity.x += -sin(glm::radians(_CameraRotation)) * _CameraTranslationSpeed;
			velocity.y += cos(glm::radians(_CameraRotation)) * _CameraTranslationSpeed;
			break;
		case OSR_KEY_S:
			velocity.x += -(-sin(glm::radians(_CameraRotation)) * _CameraTranslationSpeed);
			velocity.y += -(cos(glm::radians(_CameraRotation)) * _CameraTranslationSpeed);
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