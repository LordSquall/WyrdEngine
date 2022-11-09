#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/ecs/Components.h>
#include <core/scene/Scene.h>

/* local includes */
#include "RelationshipHelperFuncs.h"

namespace Wyrd::Editor
{
    void RelationshipHelperFuncs::AddChild(Wyrd::Scene* scene, Entity parent, Entity child)
    {
		RelationshipComponent* rc = scene->Get<RelationshipComponent>(child);
		
		if (rc != nullptr)
		{
			/* set the parent link */
			rc->parent = parent;
		
			/* increment the parent child count */
			RelationshipComponent* parentRC = scene->Get<RelationshipComponent>(parent);
			parentRC->childrenCnt += 1;
		
			/* add new child to start of child list */
			RelationshipComponent* firstChildRC = scene->Get<RelationshipComponent>(parentRC->first);
			if (firstChildRC != nullptr)
			{
				firstChildRC->previous = child;
				rc->next = parentRC->first;
				parentRC->first = child;
			}
			else
			{
				parentRC->first = child;
			}
		}
    }

	void RelationshipHelperFuncs::Remove(Wyrd::Scene* scene, Entity entity)
	{
		RelationshipComponent* rc = scene->Get<RelationshipComponent>(entity);
		
		if (rc != nullptr)
		{
			/* mark the relationship for removal */
			rc->remove = true;
		
			/* decrement the parent child count */
			RelationshipComponent* parentRC = scene->Get<RelationshipComponent>(rc->parent);
		
		
			if (parentRC != nullptr)
			{
				parentRC->childrenCnt -= 1;
		
				/* only child removed */
				if (parentRC->childrenCnt == 0)
				{
					parentRC->first = ENTITY_INVALID;
				}
		
				/* first child removed */
				if (parentRC->first == entity)
				{
					parentRC->first = rc->next;
				}
		
				RelationshipComponent* previousRC = scene->Get<RelationshipComponent>(rc->previous);
				if (previousRC != nullptr)
				{
					previousRC->next = rc->next;
				}
		
				RelationshipComponent* nextRC = scene->Get<RelationshipComponent>(rc->next);
				if (nextRC != nullptr)
				{
					nextRC->previous = rc->previous;
				}
			}
		}
	}
}