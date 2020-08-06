#pragma once

/* local includes */
#include "core/export.h"

/* external includes */
#include <glm/glm.hpp>

namespace Osiris
{
	class OSR_LIBRARY_API OrthographicCamera
	{
	public:
		OrthographicCamera();
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		/* Getters and Setters */
		inline const glm::vec3& GetPosition() const { return _Position; }
		inline void SetPosition(const glm::vec3& position) { _Position = position; RecalulateView(); }

		inline float GetRotation() const { return _Rotation; }
		inline void SetRotation(float rotation) { _Rotation = rotation; RecalulateView(); }

		inline float GetLeft() const { return _Left; }
		inline void SetLeft(float left) { _Left = left; RecalulateProjection(); }

		inline float GetRight() const { return _Right; }
		inline void SetRight(float right) { _Right = right; RecalulateProjection(); }

		inline float GetTop() const { return _Top; }
		inline void SetTop(float top) { _Top = top; RecalulateProjection(); }

		inline float GetBottom() const { return _Bottom; }
		inline void SetBottom(float bottom) { _Bottom = bottom; RecalulateProjection(); }

		inline float GetHeight() const { return fabs(_Right) + fabs(_Left); }
		inline float GetWidth() const { return fabs(_Top) + fabs(_Bottom); }

		inline const glm::mat4& GetProjectionMatrix() const { return _ProjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return _ViewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return _ViewProjectionMatrix; }

		void RecalulateViewProjection();

	private:
		void RecalulateProjection();
		void RecalulateView();

	private:
		glm::mat4 _ProjectionMatrix;
		glm::mat4 _ViewMatrix;
		glm::mat4 _ViewProjectionMatrix;

		float _Left;
		float _Right;
		float _Bottom;
		float _Top;

		glm::vec3 _Position = { 0.0f, 0.0f, 0.0f };
		float _Rotation = 0.0f;
		float _ZoomLevel = 0.0f;
	};
}
