#include "wyrdpch.h"

/* core wyrd includes */
#include <core/Input.h>
#include <core/KeyCodes.h>

/* local includes */
#include "CameraController.h"

namespace Wyrd::Editor {

	CameraController::CameraController() :
		_PanSensitivity(0.05f),
		_PivotSensitivity(0.1f),
		_InitialXPos(0.0f),
		_InitialYPos(0.0f),
		_LeftShift(false)
	{
		_Camera.SetPosition({ 10.0f, 10.0f, 10.0f });
		_Camera.SetPitch(-45.0f);
		_Camera.SetYaw(-135.0f);
	}

	void CameraController::SetViewportSize(float width, float height)
	{
		_Camera.perspectiveSettings.aspect = width / height;

		// TODO needs work
		float a = glm::radians(45.0f);
		float halfY = 20 * sqrt(1 + a * a) / 2.0f * 3.1415926f / 180.0f;
		float top = 1.0f * tan(halfY);
		float right = top * _Camera.perspectiveSettings.aspect;

		top *= 50.0f;
		right *= 50.0f;

		_Camera.orthoSettings.left = -right;
		_Camera.orthoSettings.right = right;
		_Camera.orthoSettings.top = top;
		_Camera.orthoSettings.bottom = -top;
		_Camera.orthoSettings.nearPlane = 0.1f;
		_Camera.orthoSettings.farPlane = 100.0f;
	}

	void CameraController::OnUpdate(Timestep ts)
	{
		_Camera.Update();
	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(WYRD_BIND_EVENT_FN(CameraController::OnMouseButtonPressed), nullptr);
		dispatcher.Dispatch<MouseButtonReleasedEvent>(WYRD_BIND_EVENT_FN(CameraController::OnMouseButtonReleased), nullptr);
		dispatcher.Dispatch<MouseMovedEvent>(WYRD_BIND_EVENT_FN(CameraController::OnMouseMoved), nullptr);
		dispatcher.Dispatch<MouseScrolledEvent>(WYRD_BIND_EVENT_FN(CameraController::OnMouseScrolled), nullptr);
		dispatcher.Dispatch<WindowResizeEvent>(WYRD_BIND_EVENT_FN(CameraController::OnWindowResized), nullptr);
		dispatcher.Dispatch<KeyPressedEvent>(WYRD_BIND_EVENT_FN(CameraController::OnKeyPressed), nullptr);
		dispatcher.Dispatch<KeyReleasedEvent>(WYRD_BIND_EVENT_FN(CameraController::OnKeyReleased), nullptr);
	}


	void CameraController::Translate(glm::vec2 delta)
	{
		_Camera.SetPosition(_Camera.GetPosition() + (glm::vec3(delta, 0.0f)));
	}

	bool CameraController::OnMouseButtonPressed(MouseButtonPressedEvent& e, void* data)
	{
		if (e.GetMouseButton() == 2) // TODO: Mouse Wheel
		{
			if (_LeftShift == false)
			{
				_TransformMode = CameraController::Mode::Pivot;
			}
			else
			{
				_TransformMode = CameraController::Mode::Pan;

			}
			_InitialXPos = e.GetPositionX();
			_InitialYPos = e.GetPositionY();
		}

		return true;
	}

	bool CameraController::OnMouseButtonReleased(MouseButtonReleasedEvent& e, void* data)
	{
		_TransformMode = CameraController::Mode::None;
		return true;
	}

	bool CameraController::OnMouseMoved(MouseMovedEvent& e, void* data)
	{
		switch (_TransformMode)
		{
		case Mode::Pivot:
			{
				float xOffset = (_InitialXPos - e.GetX()) * _PivotSensitivity;
				float yOffset = (e.GetY() - _InitialYPos) * _PivotSensitivity;
				_InitialXPos = e.GetX();
				_InitialYPos = e.GetY();

				float currentPitch = _Camera.GetPitch();
				float currentYaw = _Camera.GetYaw();

				currentPitch += yOffset;
				currentYaw += xOffset;

				if (currentPitch > 89.0f)
					currentPitch = 89.0f;
				if (currentPitch < -89.0f)
					currentPitch = -89.0f;

				_Camera.SetPitch(currentPitch);
				_Camera.SetYaw(currentYaw);
			}
			break;
		case Mode::Pan:
			{
				float xOffset = (e.GetX() - _InitialXPos) * _PanSensitivity;
				float yOffset = (_InitialYPos - e.GetY()) * _PanSensitivity;
				_InitialXPos = e.GetX();
				_InitialYPos = e.GetY();

				glm::vec3 delta = glm::normalize((-_Camera.GetRight() * xOffset) + (-_Camera.GetForward() * yOffset));
				_Camera.SetPosition(_Camera.GetPosition() + glm::vec3(delta.x, 0.0f, delta.z));
			}
			break;
		}
		return true;
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e, void* data)
	{
		glm::vec3 newPosition = _Camera.GetPosition() + (_Camera.GetForward() * e.GetYOffset());

		_Camera.SetPosition(newPosition);

		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e, void* data)
	{
		return false;
	}


	bool CameraController::OnKeyPressed(KeyPressedEvent& e, void* data)
	{
		switch (e.GetKeyCode())
		{
		case OSR_KEY_W:
			_Camera.SetPosition(_Camera.GetPosition() + _Camera.GetForward());
			break;
		case OSR_KEY_S:
			_Camera.SetPosition(_Camera.GetPosition() - _Camera.GetForward());
			break;
		case OSR_KEY_A:
			_Camera.SetPosition(_Camera.GetPosition() - _Camera.GetRight());
			break;
		case OSR_KEY_D:
			_Camera.SetPosition(_Camera.GetPosition() + _Camera.GetRight());
			break;
		case OSR_KEY_R:
			_Camera.SetPosition(_Camera.GetPosition() + _Camera.GetUp());
			break;
		case OSR_KEY_F:
			_Camera.SetPosition(_Camera.GetPosition() - _Camera.GetUp());
			break;
		case OSR_KEY_T:
			_Camera.SetPosition(_Camera.GetPosition() + _Camera.GetWorldUp());
			break;
		case OSR_KEY_G:
			_Camera.SetPosition(_Camera.GetPosition() - _Camera.GetWorldUp());
			break;

		case OSR_KEY_8:
			_Camera.SetPitch(_Camera.GetPitch() + 1.0f);
			break;
		case OSR_KEY_2:
			_Camera.SetPitch(_Camera.GetPitch() - 1.0f);
			break;
		case OSR_KEY_4:
			_Camera.SetYaw(_Camera.GetYaw() + 1.0f);
			break;
		case OSR_KEY_6:
			_Camera.SetYaw(_Camera.GetYaw() - 1.0f);
			break;
		case OSR_KEY_LEFT_SHIFT:
			_LeftShift = true;
			break;
		}
		return true;
	}

	bool CameraController::OnKeyReleased(KeyReleasedEvent& e, void* data)
	{
		switch (e.GetKeyCode())
		{
		case OSR_KEY_A:
		case OSR_KEY_D:
		case OSR_KEY_W:
		case OSR_KEY_S:
			break;

		case OSR_KEY_LEFT_SHIFT:
			_LeftShift = false;
			break;
		}

		return true;
	}

}