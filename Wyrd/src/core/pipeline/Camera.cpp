#include "wyrdpch.h"

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Wyrd
{
	Camera::Camera(glm::vec3 position, glm::vec3 worldUp, glm::vec3 worldForward, float yaw, float pitch) :
		_Mode(Camera::Mode::Perspective), 
		_Position(position),
		_Up(0.0f, 1.0f, 0.0f),
		_Yaw(yaw),
		_Pitch(pitch),
		_Forward(0.0f, 0.0f, -1.0f), 
		_Right(-1.0f, 0.0f, 0.0f), 
		_WorldUp(worldUp),
		_WorldForward(worldForward),
		_viewMatrix(glm::identity<glm::mat4>()), 
		_projectionMatrix(glm::identity<glm::mat4>())
	{
		Camera::Update();
	}

	void Camera::LookAt(const glm::vec3 target)
	{
		glm::vec3 delta = _Position - target;
		_Yaw = glm::degrees(atan2f(delta.z, delta.x));
		_Pitch = glm::degrees(-atan2f(sqrtf(delta.z * delta.z + delta.x * delta.x), delta.y) + WYRD_PI);
	}

	void Camera::Update()
	{
		if (_Mode == Mode::Perspective)
		{
			_projectionMatrix = glm::perspective(glm::radians(perspectiveSettings.fov), perspectiveSettings.aspect, perspectiveSettings.nearPlane, perspectiveSettings.farPlane);
		}
		else
		{
			_projectionMatrix = glm::ortho(orthoSettings.left, orthoSettings.right, orthoSettings.bottom, orthoSettings.top, orthoSettings.nearPlane, orthoSettings.farPlane);
		}

		glm::vec3 front;
		front.x = cos(glm::radians(_Yaw)) * cos(glm::radians(_Pitch));
		front.y = sin(glm::radians(_Pitch));
		front.z = sin(glm::radians(_Yaw)) * cos(glm::radians(_Pitch));
		_Forward	= glm::normalize(front);
		_Right		= glm::normalize(glm::cross(_Forward, _WorldUp));
		_Up			= glm::normalize(glm::cross(_Right, _Forward));

		_viewMatrix = glm::lookAt(_Position,
			_Position + _Forward,
			_Up);
	}
}
