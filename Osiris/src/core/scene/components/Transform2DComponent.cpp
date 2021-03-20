#pragma once
#include "osrpch.h"
#include "core/Log.h"
#include "Transform2DComponent.h"
#include "core/scene/GameObject.h"
#include "serial/TypeSerialisers.h"

namespace Osiris
{
	Transform2DComponent::Transform2DComponent(GameObject* gameObject) : TransformComponent(gameObject, SceneComponentType::Transform2D), matrix(glm::identity<glm::mat4>()), worldMatrix(glm::identity<glm::mat4>()), _IsMatrixValid(false)
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

	void Transform2DComponent::Initialise()
	{
		_IsMatrixValid = false;

		ClearTransformationDelta();

		Recalculate();
	}

	void Transform2DComponent::Recalculate()
	{
		/* one update the model matrix if the source data is not longer valid */
		if (_IsMatrixValid == false)
		{
			glm::mat4 parentMatrix = (Owner->parent != nullptr && Owner->parent->transform2D != nullptr) ? Owner->parent->transform2D->matrix : glm::mat4(1.0);

			position += _PositionDelta;
			rotation += _RotationDelta;
			scale += _ScaleDelta;

			matrix = parentMatrix
				* glm::translate(glm::identity<glm::mat4>(), glm::vec3(position.x, position.y, 0.0f))
				* glm::rotate(glm::identity<glm::mat4>(), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f))
				* glm::scale(glm::identity<glm::mat4>(), glm::vec3(scale.x, scale.y, 1.0f));
				
			globalPosition = matrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			_IsMatrixValid = true;

			ClearTransformationDelta();
		}
	}

	jsonxx::Object Transform2DComponent::ToJson()
	{
		jsonxx::Object object;

		/* position */
		object << "position" << glm::vec3(position, 1.0f);

		return object;
	}

	bool Transform2DComponent::FromJson(jsonxx::Object& object)
	{
		/* position color */
		if (object.has<jsonxx::Array>("position"))
			position << object.get<jsonxx::Array>("position");

		return true;
	}
}