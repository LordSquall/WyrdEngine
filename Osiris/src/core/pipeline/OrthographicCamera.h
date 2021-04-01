#pragma once


/* core osiris includes */
#include <core/maths/Rect.h>
#include <core/export.h>

/* local includes */

/* external includes */
#include <glm/glm.hpp>

namespace Osiris
{
	class OSR_LIBRARY_API OrthographicCamera
	{
	public:
		OrthographicCamera();
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float size, float nearPlane, float farPlane);
		inline void SetViewportSize(float width, float height) { _AspectRatio = width / height; RecalulateProjection(); }

		inline const Rect& GetViewport() { return _Viewport; }

		/* Getters and Setters */
		inline const glm::vec3& GetPosition() const { return _Position; }
		inline void SetPosition(const glm::vec3& position) { _Position = position; RecalulateView(); }

		inline float GetRotation() const { return _Rotation; }
		inline void SetRotation(float rotation) { _Rotation = rotation; RecalulateView(); }

		inline float GetSize() const { return _Size; }
		inline void SetSize(float size) { _Size = size; RecalulateProjection(); }

		inline float GetAspectRatio() const { return _AspectRatio; }

		inline const glm::mat4& GetProjectionMatrix() const { return _ProjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return _ViewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return _ViewProjectionMatrix; }

		glm::vec2 GetNDCFromPoint(const glm::vec2& point, const Osiris::Rect& boundary);
		glm::vec2 GetWorldSpaceFromPoint(const glm::vec2& point, const Osiris::Rect& boundary);
		glm::vec2 GetEyeSpaceFromPoint(const glm::vec2& point, const Osiris::Rect& boundary);

		void RecalulateViewProjection();

	private:
		void RecalulateProjection();
		void RecalulateView();

	private:
		glm::mat4 _ProjectionMatrix;
		glm::mat4 _ViewMatrix;
		glm::mat4 _ViewProjectionMatrix;

		float _Size;
		float _NearPlane;
		float _FarPlane;
		float _AspectRatio;

		Rect _Viewport;

		glm::vec3 _Position = { 0.0f, 0.0f, 0.0f };
		float _Rotation = 0.0f;
		float _ZoomLevel = 0.0f;
	};
}
