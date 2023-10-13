#include "wyrdpch.h"

#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Wyrd
{
	Camera::Camera(glm::vec3 position, glm::vec3 worldUp, glm::vec3 worldForward, float yaw, float pitch) :
		_Mode(Camera::Mode::Perspective), 
		_Position(position),
		_Yaw(yaw),
		_Pitch(pitch),
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

			_Position = CalculatePosition();
			glm::quat orientation = GetOrientationQuat();

			_viewMatrix = glm::translate(glm::mat4(1.0f), _Position) * glm::toMat4(orientation);
			_viewMatrix = glm::inverse(_viewMatrix);

			// Update frustum
			const float halfVSide = perspectiveSettings.farPlane * tanf(glm::radians(perspectiveSettings.fov) * .5f);
			const float halfHSide = halfVSide * perspectiveSettings.aspect;
			const glm::vec3 frontMultFar = perspectiveSettings.farPlane * -GetForwardDirection();

			glm::vec3 invertedPos = -_Position;

			frustum.nearFace = { invertedPos + perspectiveSettings.nearPlane * -GetForwardDirection(),
				GetForwardDirection() };

			frustum.farFace = { invertedPos + frontMultFar,
				-GetForwardDirection() };

			frustum.rightFace = { invertedPos,
				glm::cross(frontMultFar - GetRightDirection() * halfHSide, GetUpDirection()) };

			frustum.leftFace = { invertedPos,
				glm::cross(GetUpDirection(), frontMultFar + GetRightDirection() * halfHSide) };

			frustum.bottomFace = { invertedPos,
				glm::cross(GetRightDirection(), frontMultFar - GetUpDirection() * halfVSide) };

			frustum.topFace = { invertedPos,
				glm::cross(frontMultFar + GetUpDirection() * halfVSide, GetRightDirection()) };

		}
		else
		{
			_projectionMatrix = glm::ortho(orthoSettings.left, orthoSettings.right, orthoSettings.bottom, orthoSettings.top, orthoSettings.nearPlane, orthoSettings.farPlane);

			_Position = CalculatePosition();
			glm::quat orientation = GetOrientationQuat();

			_viewMatrix = glm::translate(glm::mat4(1.0f), _Position) * glm::toMat4(orientation);
			_viewMatrix = glm::inverse(_viewMatrix);

			glm::vec3 invertedPos = -_Position;

			frustum.nearFace = { invertedPos + orthoSettings.nearPlane * -GetForwardDirection(), GetForwardDirection() };
			frustum.farFace = { invertedPos + orthoSettings.farPlane * -GetForwardDirection(), -GetForwardDirection() };
			frustum.rightFace = { invertedPos + (orthoSettings.right), GetRightDirection() };
			frustum.leftFace = { invertedPos + (orthoSettings.left), -GetRightDirection() };
			frustum.bottomFace = { invertedPos + (orthoSettings.bottom), GetUpDirection() };
			frustum.topFace = { invertedPos + (orthoSettings.top), -GetUpDirection() };
		}

	}


	glm::vec3 Camera::GetUpDirection() const
	{
		return glm::rotate(GetOrientationQuat(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 Camera::GetRightDirection() const
	{
		return glm::rotate(GetOrientationQuat(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 Camera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientationQuat(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 Camera::CalculatePosition() const
	{
		return _Position;
	}

	glm::quat Camera::GetOrientationQuat() const
	{
		return glm::quat(glm::vec3(_Pitch, _Yaw, 0.0f));
	}

	const glm::vec3 Camera::GetWorldSpaceFromPoint(const glm::vec3& point, const Wyrd::Rect& boundary)
	{
		glm::vec3 ndcMouseCoords = GetNDCFromPoint(point, boundary);

		glm::vec4 eyeCoords = glm::inverse(_projectionMatrix) * glm::vec4(ndcMouseCoords.x, ndcMouseCoords.y, -1.0f, 1.0f);

		glm::vec4 worldCoords = glm::inverse(_viewMatrix) * eyeCoords;

		return glm::vec3(worldCoords.x, worldCoords.y, worldCoords.z);
	}
}
