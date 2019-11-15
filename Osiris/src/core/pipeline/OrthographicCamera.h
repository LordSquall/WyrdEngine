#pragma once

#include <glm/glm.hpp>

namespace Osiris
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		/* Getters and Setters */
		inline const glm::vec3& GetPosition() const { return _Position; }
		inline void SetPosition(const glm::vec3& position) { _Position = position; Recalulate(); }

		inline float GetRotation() const { return _Rotation; }
		inline void SetRotation(float rotation) { _Rotation = rotation; Recalulate(); }

		inline const glm::mat4& GetProjectionMatrix() const { return _ProjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return _ViewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return _ViewProjectionMatrix; }

	private:
		void Recalulate();

	private:
		glm::mat4 _ProjectionMatrix;
		glm::mat4 _ViewMatrix;
		glm::mat4 _ViewProjectionMatrix;

		glm::vec3 _Position = { 0.0f, 0.0f, 0.0f };
		float _Rotation = 0.0f;
	};
}
