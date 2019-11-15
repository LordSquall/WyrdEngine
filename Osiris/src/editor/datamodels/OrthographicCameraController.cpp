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
		float translationSpeed = _CameraTranslationSpeed;
		float rotationSpeed = _CameraRotationSpeed;

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

		if (Input::IsKeyPressed(OSR_KEY_A))
		{
			_CameraPosition.x -= cos(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * ts;
			_CameraPosition.y -= sin(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(OSR_KEY_D))
		{
			_CameraPosition.x += cos(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * ts;
			_CameraPosition.y += sin(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * ts;
		}

		if (Input::IsKeyPressed(OSR_KEY_W))
		{
			_CameraPosition.x += -sin(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * ts;
			_CameraPosition.y += cos(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(OSR_KEY_S))
		{
			_CameraPosition.x -= -sin(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * ts;
			_CameraPosition.y -= cos(glm::radians(_CameraRotation)) * _CameraTranslationSpeed * ts;
		}

		if (_Rotation)
		{
			if (Input::IsKeyPressed(OSR_KEY_Q))
				_CameraRotation += _CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(OSR_KEY_E))
				_CameraRotation -= _CameraRotationSpeed * ts;

			if (_CameraRotation > 180.0f)
				_CameraRotation -= 360.0f;
			else if (_CameraRotation <= -180.0f)
				_CameraRotation += 360.0f;

			_Camera.SetRotation(_CameraRotation);
		}

		_Camera.SetPosition(_CameraPosition);

		_InitialCameraTranslationSpeed = _ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(OSR_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(OSR_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		_ZoomLevel -= e.GetYOffset() * _CameraZoomSpeed;
		_ZoomLevel = std::max(_ZoomLevel, _CameraZoomSpeed);
		_Camera.SetProjection(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		_Camera.SetProjection(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel);
		return false;
	}

}