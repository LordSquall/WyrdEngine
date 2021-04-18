#pragma once
#include "osrpch.h"
#include "core/Log.h"
#include "Transform2DComponent.h"
#include "core/scene/GameObject.h"
#include "serial/TypeSerialisers.h"

namespace Osiris
{
	Transform2DComponent::Transform2DComponent(GameObject* gameObject) : TransformComponent(gameObject, SceneComponentType::Transform2D), _ModelMatrix(glm::identity<glm::mat4>()), worldMatrix(glm::identity<glm::mat4>()), _IsMatrixValid(false)
	{
		position = vec2(0.0f, 0.0f);
		rotation = 0.0f;
		scale = vec2(1.0f, 1.0f);

		Initialised = true;

		ClearTransformationDelta();

		_IsMatrixValid = false;
	}

	void Transform2DComponent::SetPosition(const vec2& pos)
	{ 
		position = pos;
		_IsMatrixValid = false;
	}

	void Transform2DComponent::SetRotation(const float rot)
	{ 
		rotation = rot;
		_IsMatrixValid = false;
	}

	void Transform2DComponent::SetScale(const vec2& scl)
	{ 
		scale = scl;
		_IsMatrixValid = false;
	}

	void Transform2DComponent::Translate(const vec2& pos)
	{
		_PositionDelta += pos;
		_IsMatrixValid = false;
	}

	void Transform2DComponent::Rotate(const float rot)
	{
		_RotationDelta += rot;
		_IsMatrixValid = false;
	}

	void Transform2DComponent::Scale(const vec2& scl)
	{
		_ScaleDelta += scl;
		_IsMatrixValid = false;
	}


	void Transform2DComponent::ClearTransformationDelta()
	{
		_PositionDelta = { 0.0f, 0.0f };
		_RotationDelta = 0.0f;
		_ScaleDelta = { 0.0f, 0.0f };
	}

	bool Transform2DComponent::Initialise()
	{
		_IsMatrixValid = false;

		ClearTransformationDelta();

		Recalculate();

		return true;
	}

	void Transform2DComponent::Recalculate()
	{
		/* one update the model matrix if the source data is not longer valid */
		if (_IsMatrixValid == false)
		{
			position += _PositionDelta;
			rotation += _RotationDelta;
			scale += _ScaleDelta;

			_ModelMatrix = glm::translate(glm::identity<glm::mat4>(), glm::vec3(position.x, position.y, 0.0f))
				* glm::rotate(glm::identity<glm::mat4>(), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
				* glm::scale(glm::identity<glm::mat4>(), glm::vec3(scale.x, scale.y, 1.0f));
				
			globalPosition = _ModelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			_IsMatrixValid = true;

			ClearTransformationDelta();
		}
	}

	bool Transform2DComponent::ToJson(jsonxx::Object& object)
	{
		/* position */
		object << "position" << position;

		return true;
	}

	bool Transform2DComponent::FromJson(jsonxx::Object& object)
	{
		/* position */
		if (object.has<jsonxx::Array>("position"))
			position << object.get<jsonxx::Array>("position");

		return true;
	}
}