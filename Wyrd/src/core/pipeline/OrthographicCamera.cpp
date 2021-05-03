#pragma once
#include "wyrdpch.h"

#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Wyrd
{
	OrthographicCamera::OrthographicCamera()
		: _ProjectionMatrix(1.0f), _ViewMatrix(1.0f), _ViewProjectionMatrix(1.0f), _Size(64.0), _NearPlane(-1.0f), _FarPlane(1.0f), _AspectRatio(1.0f), _Viewport({{ 0.0f, 0.0f }, {0.0f, 0.0f}})
	{
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: _ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), _ViewMatrix(1.0f), _Size(1.0), _NearPlane(-1.0f), _FarPlane(1.0f), _AspectRatio(1.0f), _Viewport({ { 0.0f, 0.0f }, {0.0f, 0.0f} })
	{
		_ViewProjectionMatrix = _ProjectionMatrix * _ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float size, float nearPlane, float farPlane)
	{
		_Size = size;
		_NearPlane = nearPlane;
		_FarPlane = farPlane;

		RecalulateProjection();
	}

	void OrthographicCamera::RecalulateProjection()
	{
		float l = -_Size * _AspectRatio * 0.5f;
		float r = _Size * _AspectRatio * 0.5f;
		float b = -_Size * 0.5f;
		float t = _Size * 0.5f;

		_Viewport.position = { l, b };
		_Viewport.size = { (r * 2.0f), (t * 2.0f) };

		_ProjectionMatrix = glm::ortho(l, r, b, t, _NearPlane, _FarPlane);
	}

	void OrthographicCamera::RecalulateView()
	{
		_ViewMatrix = glm::translate(glm::mat4(1.0f), -_Position);
	}

	void OrthographicCamera::RecalulateViewProjection()
	{
		_ViewProjectionMatrix = _ProjectionMatrix * _ViewMatrix;
	}

	glm::vec2 OrthographicCamera::GetNDCFromPoint(const glm::vec2& point, const Wyrd::Rect& boundary)
	{
		return { -(1.0 - (point.x * (1.0f / (boundary.size.x * 0.5f)))), 1.0f - (point.y * (1.0f / (boundary.size.y * 0.5f))) };
	}

	glm::vec2 OrthographicCamera::GetWorldSpaceFromPoint(const glm::vec2& point, const Wyrd::Rect& boundary)
	{
		glm::vec2 ndcMouseCoords = GetNDCFromPoint(point, boundary);

		glm::vec4 eyeCoords = glm::inverse(_ProjectionMatrix) * glm::vec4(ndcMouseCoords, -1.0f, 1.0f);

		glm::vec4 worldCoords = glm::inverse(_ViewMatrix) * eyeCoords;

		return glm::vec2(worldCoords.x, worldCoords.y);
	}

	glm::vec2 OrthographicCamera::GetEyeSpaceFromPoint(const glm::vec2& point, const Wyrd::Rect& boundary)
	{
		glm::vec2 ndcMouseCoords = GetNDCFromPoint(point, boundary);

		glm::vec4 eyeCoords = glm::inverse(_ProjectionMatrix) * glm::vec4(ndcMouseCoords, -1.0f, 1.0f);

		return glm::vec2(eyeCoords.x, eyeCoords.y);
	}
}
