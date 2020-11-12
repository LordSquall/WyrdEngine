#pragma once
#include "osrpch.h"
#include "core/Log.h"
#include "Transform2DComponent.h"
#include "core/scene/GameObject.h"


namespace Osiris
{
	Transform2DComponent::Transform2DComponent(std::shared_ptr<GameObject> gameObject) : IBaseComponent(gameObject, SceneComponentType::Transform2D), matrix(glm::identity<glm::mat4>()), _IsMatrixValid(false)
	{
		position = vec2(0.0f, 0.0f);
		rotation = 0.0f;
		scale = vec2(1.0f, 1.0f);

		ClearTransformationDelta();
	}

	Transform2DComponent::Transform2DComponent(const Transform2DComponent& obj) : IBaseComponent(obj.Owner, SceneComponentType::Transform2D), matrix(glm::identity<glm::mat4>())
	{
		position = obj.position;
		rotation = obj.rotation;
		scale = obj.scale;

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

	void Transform2DComponent::Recalculate()
	{
		if (_IsMatrixValid == false)
		{
			glm::mat4 parentMatrix = Owner->parent != nullptr ? Owner->parent->transform2D->matrix : glm::mat4(1.0);

			position += _PositionDelta;
			rotation += _RotationDelta;
			scale += _ScaleDelta;

			matrix = parentMatrix * glm::translate(glm::identity<glm::mat4>(), glm::vec3(position.x, position.y, 0.0f));

			globalPosition = parentMatrix * matrix * glm::vec4(position.x, position.y, 0.0f, 1.0f);

			_IsMatrixValid = true;

			ClearTransformationDelta();
		}
	}
}