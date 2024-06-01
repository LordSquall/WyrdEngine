#include "wyrdpch.h"

#include "core/systems/TransformUpdateSystem.h"
#include "core/Log.h"
#include "core/ecs/ECS.h"
#include "core/ecs/EntitySet.h"
#include "core/ecs/HierarchyEntitySet.h"
#include "core/ecs/Components.h"

/* external includes */
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Wyrd
{
	TransformUpdateSystem::TransformUpdateSystem()
	{
		_MatrixStack.push({ ENTITY_INVALID, ENTITY_INVALID, glm::mat4(1.0f) });
	}

	bool TransformUpdateSystem::OnUpdate(ISystemContext& context)
	{
		Entity currentParent = ENTITY_INVALID;

		for (Entity e : HierarchyEntitySet<Transform3DComponent, RelationshipComponent>(context.scene))
		{
			Transform3DComponent* transform3DComponent = context.scene.Get<Transform3DComponent>(e);
			RelationshipComponent* relationshipComponent = context.scene.Get<RelationshipComponent>(e);

			glm::quat q = glm::quat(glm::vec3(glm::radians(transform3DComponent->rotation.x), glm::radians(transform3DComponent->rotation.y), glm::radians(transform3DComponent->rotation.z)));

			glm::mat4 translate = glm::translate(glm::mat4(1), glm::vec3(transform3DComponent->position.x, transform3DComponent->position.y, transform3DComponent->position.z));
			glm::mat4 rotate = glm::toMat4(q);
			glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(transform3DComponent->scale.x, transform3DComponent->scale.y, transform3DComponent->scale.z));

			transform3DComponent->modelMatrix = translate * rotate * scale;
			
			const auto& lastParent = _MatrixStack.top();
			if (relationshipComponent->parent == lastParent.from)	// Same a previous 
			{
				transform3DComponent->parentModelMatrix = lastParent.mat;
			}
			else if (relationshipComponent->parent != lastParent.to && relationshipComponent->parent == lastParent.from)
			{
				_MatrixStack.pop();
				transform3DComponent->parentModelMatrix = _MatrixStack.top().mat;
			}
			else
			{
				_MatrixStack.push({ relationshipComponent->parent, e, transform3DComponent->modelMatrix * lastParent.mat });
				transform3DComponent->parentModelMatrix = lastParent.mat;
			}
		}

		return true;
	}
}