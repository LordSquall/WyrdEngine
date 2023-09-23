#include "wyrdpch.h"

/* core wyrd includes */
#include <core/Input.h>
#include <core/KeyCodes.h>
#include <core/Log.h>

/* local includes */
#include "CameraController.h"

namespace Wyrd::Editor {

	CameraController::CameraController() :
		_PanSensitivity(0.05f),
		_PivotSensitivity(0.1f),
		_PrevXPos(0.0f),
		_PrevYPos(0.0f),
		_LeftShift(false)
	{
		_Camera.SetPosition({0.0f, 5.0f, 20.0f});
		//_Camera.SetPosition({ 10.0f, 10.0f, 10.0f });
		//_Camera.SetPitch(-45.0f);
		//_Camera.SetYaw(-135.0f);
	}

	void CameraController::SetViewportSize(float width, float height)
	{
		_Camera.perspectiveSettings.aspect = width / height;

		// TODO needs work :/
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

	bool CameraController::OnMouseButtonPressed(MouseButtonPressedEvent& e, void* data)
	{
		if (e.GetMouseButton() == 2) // TODO: Mouse Wheel
		{
			if (_LeftShift == false)
			{
				_TransformMode = CameraController::Mode::Pivot;
				_InitialPitch = _Camera.GetPitch();
				_InitialYaw = _Camera.GetYaw();
				_PivotPitchDelta = 0;
				_PivotYawDelta = 0;
			}
			else
			{
				_TransformMode = CameraController::Mode::Pan;

			}
		}

		return true;
	}

	bool CameraController::OnMouseButtonReleased(MouseButtonReleasedEvent& e, void* data)
	{
		_TransformMode = CameraController::Mode::None;

		_PivotPitchDelta = 0;
		_PivotYawDelta = 0;

		return true;
	}

	bool CameraController::OnMouseMoved(MouseMovedEvent& e, void* data)
	{
		const glm::vec2& mouse{ e.GetX(), e.GetY() };
		glm::vec2 delta = glm::vec2((mouse.x - _PrevXPos), (mouse.y - _PrevYPos)) * 0.003f;

		switch (_TransformMode)
		{
		case Mode::Pivot:
			MouseRotate(delta);
			break;
		case Mode::Pan:
			MousePan(delta);
			break;
		}

		_PrevXPos = mouse.x;
		_PrevYPos = mouse.y;

		return true;
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e, void* data)
	{
		MouseZoom(e.GetYOffset());
		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e, void* data)
	{
		return false;
	}


	bool CameraController::OnKeyPressed(KeyPressedEvent& e, void* data)
	{
		glm::vec3 currentCamPos = _Camera.GetPosition();
		switch (e.GetKeyCode())
		{
		case OSR_KEY_R:
			_Camera.SetPosition({ currentCamPos.x, currentCamPos.y + 1.0f, currentCamPos.z });
			break;
		case OSR_KEY_F:
			_Camera.SetPosition({ currentCamPos.x, currentCamPos.y - 1.0f, currentCamPos.z });
			break;
		case OSR_KEY_W:
			_Camera.SetPosition(currentCamPos  + _Camera.GetForwardDirection());
			break;
		case OSR_KEY_S:
			_Camera.SetPosition(currentCamPos - _Camera.GetForwardDirection());
			break;
		case OSR_KEY_A:
			_Camera.SetPosition(currentCamPos - _Camera.GetRightDirection());
			break;
		case OSR_KEY_D:
			_Camera.SetPosition(currentCamPos + _Camera.GetRightDirection());
			break;
		case OSR_KEY_LEFT_SHIFT:
			_LeftShift = true;
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

	void CameraController::MousePan(const glm::vec2& delta)
	{
		glm::vec3 currentCamPos = _Camera.GetPosition();
		glm::vec3 cameraDir = glm::normalize((_Camera.GetRightDirection()) * delta.x + (_Camera.GetUpDirection() * delta.y));
		glm::vec3 newCamPos = currentCamPos - (cameraDir * PanSpeed());
		_Camera.SetPosition(newCamPos);
	}

	void CameraController::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = _Camera.GetUpDirection().y < 0 ? -1.0f : 1.0f;
		float newYaw = (yawSign * delta.x * RotationSpeed());
		float newPitch = (delta.y * RotationSpeed());

		_PivotYawDelta += newYaw;
		_PivotPitchDelta += newPitch;

		_Camera.SetYaw(_InitialYaw + _PivotYawDelta);
		_Camera.SetPitch(_InitialPitch + _PivotPitchDelta);
	}

	void CameraController::MouseZoom(float delta)
	{
		glm::vec3 currentCamPos = _Camera.GetPosition();
		_Camera.SetPosition(currentCamPos + (_Camera.GetForwardDirection() * (delta * ZoomSpeed())));
	}

	float CameraController::RotationSpeed() const
	{
		return 0.5f;
	}

	float CameraController::ZoomSpeed() const
	{
		return 0.5f;
	}

	float CameraController::PanSpeed() const
	{
		return 0.5f;
	}
}