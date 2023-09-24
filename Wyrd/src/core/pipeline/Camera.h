#pragma once


/* core wyrd includes */
#include <core/maths/Rect.h>
#include <core/export.h>
#include <core/maths/Frustum.h>

/* local includes */

/* external includes */
#include <glm/glm.hpp>

// Default camera values
const float YAW = 0.0f;
const float PITCH = 0.0f;

namespace Wyrd
{
	class WYRD_LIBRARY_API Camera
	{
	public:
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f), float yaw = YAW, float pitch = PITCH);

		void Update();

		void LookAt(const glm::vec3 target);

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		glm::quat GetOrientationQuat() const;

		inline const glm::vec3 GetWorldUp() const { return _WorldUp; }
		inline const glm::vec3 GetWorldForward() const { return _WorldForward; }

		inline const float GetYaw() const { return _Yaw; }
		inline void SetYaw(float yaw) { _Yaw = yaw; }

		inline const float GetPitch() const { return _Pitch; }
		inline void SetPitch(float pitch) { _Pitch = pitch; }

		inline const glm::vec3 GetPosition() const { return _Position; }
		inline void SetPosition(const glm::vec3 position) { _Position = position; }

		inline const glm::vec3 GetOrientation() const { return { _Pitch, 0.0f, _Yaw }; }
		inline void SetOrientation(const glm::vec3 orientation) { SetPitch(orientation.x); SetYaw(orientation.z); }

		inline const glm::mat4 GetViewMatrix() const { return _viewMatrix; }
		inline const glm::mat4 GetProjectionMatrix() const { return _projectionMatrix; }

		const glm::vec3 GetWorldSpaceFromPoint(const glm::vec3& point, const Wyrd::Rect& boundary);

		inline const glm::vec3 GetNDCFromPoint(const glm::vec3& point, const Wyrd::Rect& boundary)
		{
			return { -(1.0 - (point.x * (1.0f / (boundary._size.x * 0.5f)))), 1.0f - (point.y * (1.0f / (boundary._size.y * 0.5f))), 0.0f };
		}

	public:

		enum class Mode { Orthographic, Perspective } _Mode;

		inline void SetMode(Camera::Mode mode) { _Mode = mode; }
		inline const Camera::Mode GetMode() const { return _Mode; }

		struct PerspectiveSettings
		{
			float fov;
			float aspect;
			float nearPlane;
			float farPlane;

			PerspectiveSettings() : fov(45.0f), aspect(1.33f), nearPlane(0.1f), farPlane(100.0f) {}
			
		};

		struct OrthoSettings
		{
			float size;
			float left;
			float right;
			float bottom;
			float top;
			float nearPlane;
			float farPlane;

			OrthoSettings() : size(1.0f), left(0.0f), right(800.0f), bottom(0.0f), top(600.0f), nearPlane(0.1f), farPlane(100.0f) {}
		};

	private:

		glm::vec3 _WorldUp;
		glm::vec3 _WorldForward;

		glm::mat4 _viewMatrix;
		glm::mat4 _projectionMatrix;

		glm::vec3 _Position = { 0.0f, 0.0f, 0.0f };

		float _Distance = 0.0f;
		float _Yaw = 0.0f;
		float _Pitch = 0.0f;

		glm::vec3 CalculatePosition() const;

	public:
		OrthoSettings orthoSettings;
		PerspectiveSettings perspectiveSettings;

		Frustum frustum;
	};
}
