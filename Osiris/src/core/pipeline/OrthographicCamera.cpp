#pragma once
#include "osrpch.h"

#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Osiris
{
	OrthographicCamera::OrthographicCamera()
		: _ProjectionMatrix(1.0f), _ViewMatrix(1.0f), _ViewProjectionMatrix(1.0f), _Left(0.0f), _Right(0.0f), _Bottom(0.0f), _Top(0.0f)
	{
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: _ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), _ViewMatrix(1.0f), _Left(left), _Right(right), _Bottom(bottom), _Top(top)
	{
		_ViewProjectionMatrix = _ProjectionMatrix * _ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		_Left = left;
		_Right = right;
		_Bottom = bottom;
		_Top = top;

		RecalulateProjection();
	}

	void OrthographicCamera::RecalulateProjection()
	{
		_ProjectionMatrix = glm::ortho(_Left, _Right, _Bottom, _Top, -1.0f, 1.0f);
	}

	void OrthographicCamera::RecalulateView()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), _Position);

		_ViewMatrix = glm::inverse(transform);
	}

	void OrthographicCamera::RecalulateViewProjection()
	{
		_ViewProjectionMatrix = _ProjectionMatrix * _ViewMatrix;
	}
}
