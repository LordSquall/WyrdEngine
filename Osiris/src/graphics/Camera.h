#pragma once

#include "core/export.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Osiris {

	// Default camera values
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float ZOOM = 1.0f;

	class OSIRIS_API Camera
	{
	public:
		/* Constructions/Destruction */
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float zoom = ZOOM) 
			: Front(glm::vec3(0.0f, 0.0f, -1.0f))
		{
			Position = position;
			WorldUp = up;
			Yaw = yaw;
			Pitch = pitch;
			Zoom = zoom;
			UpdateCameraVectors();

		};

		virtual ~Camera() { };

		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(Position, Position + Front, Up);
		}

		glm::mat4 GetProjectionMatrix()
		{
			return glm::orthoLH(0.0f, 1280.0f * Zoom, 0.0f, 720.0f * Zoom, 0.1f, 100.0f);
		}

	private:
		void UpdateCameraVectors();

	public:
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		float Yaw;
		float Pitch;

		float Zoom;

	};
}