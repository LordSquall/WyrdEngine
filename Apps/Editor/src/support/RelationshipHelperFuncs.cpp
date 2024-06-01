#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/ecs/Components.h>
#include <core/scene/Scene.h>

/* local includes */
#include "RelationshipHelperFuncs.h"

namespace Wyrd::Editor
{
	bool RelationshipHelperFuncs::CanAddChild(Wyrd::Scene* scene, Entity parent, Entity child, AddOp addOp)
	{
		RelationshipComponent* source_rc = scene->Get<RelationshipComponent>(child);
		RelationshipComponent* target_rc = scene->Get<RelationshipComponent>(parent);

		// source and target must both have valid relationship components
		if (source_rc == nullptr || target_rc == nullptr)
			return false;

		// can't add a child to it's own parent
		if (source_rc->parent == parent)
			return false;

		// can't add a child to one of it's own children
		if (source_rc->first == parent)
			return false;
		 
		RelationshipComponent* cur = target_rc;
		while (cur->parent != ENTITY_INVALID)
		{
			if (cur->parent == child)
				return false;

			cur = scene->Get<RelationshipComponent>(cur->parent);
		}

		if (parent == child)
			return false;


		return true;
	}

	void RelationshipHelperFuncs::AddChild(Wyrd::Scene* scene, Entity source, Entity target, AddOp addOp)
	{
		/* first check to ensure we have all the components we need to facilitate the move operation */
		RelationshipComponent* source_rc = scene->Get<RelationshipComponent>(source);
		RelationshipComponent* target_rc = scene->Get<RelationshipComponent>(target);

		if (source_rc != nullptr && target_rc != nullptr)
		{
			/* if we are first child, we can just complete the operation */
			if (target_rc->childrenCnt == 0)
			{
				target_rc->childrenCnt++;
				target_rc->first = source;
				target_rc->last = source;

				source_rc->parent = target;
				source_rc->depth = target_rc->depth + 1;
			}
			else if (target_rc->childrenCnt > 0)
			{
				/* update last element in child list */
				RelationshipComponent* targetLast_rc = scene->Get<RelationshipComponent>(target_rc->last);
				targetLast_rc->next = source;

				/* update parent */
				target_rc->childrenCnt++;
				target_rc->last = source;

				/* update the source */
				source_rc->parent = target;
				source_rc->depth = target_rc->depth + 1;
				source_rc->previous = target_rc->last;
				source_rc->next = ENTITY_INVALID;
			}
		}
	}  


	void RelationshipHelperFuncs::ClearEntity(Wyrd::Scene* scene, Entity entity)
	{
		RelationshipComponent* source_rc = scene->Get<RelationshipComponent>(entity);

		source_rc->first = 0;
		source_rc->last = 0;
		source_rc->previous = 0;
		source_rc->next = 0;
		source_rc->parent = 0;
		source_rc->childrenCnt = 0;
		source_rc->depth = 0;
	}

	//void RelationshipHelperFuncs::AddChildBefore(Wyrd::Scene* scene, Entity target, Entity source)
	//{
	//	RelationshipComponent* source_rc = scene->Get<RelationshipComponent>(source);
	//	RelationshipComponent* target_rc = scene->Get<RelationshipComponent>(target);
	//
	//	if (source_rc != nullptr && target_rc != nullptr)
	//	{
	//		bool areSiblings = source_rc->parent == target_rc->parent;
	//		bool isFirst = source_rc->previous == ENTITY_INVALID;
	//
	//		/* remove the entity from the linked list */
	//		{
	//			if (source_rc->previous != ENTITY_INVALID)
	//			{
	//				RelationshipComponent* prevSource_rc = scene->Get<RelationshipComponent>(source_rc->previous);
	//				prevSource_rc->next = source_rc->next;
	//			}
	//
	//			if (source_rc->next != ENTITY_INVALID)
	//			{
	//				RelationshipComponent* nextSource_rc = scene->Get<RelationshipComponent>(source_rc->next);
	//				nextSource_rc->previous = source_rc->previous;
	//			}
	//
	//			if (isFirst)
	//			{
	//				if (source_rc->parent != ENTITY_INVALID)
	//				{
	//					RelationshipComponent* originalParent_rc = scene->Get<RelationshipComponent>(source_rc->parent);
	//					originalParent_rc->first = source_rc->next;
	//				}
	//			}
	//		}
	//
	//		/* if we target and source are not already siblings, then we can skip updating parent links */
	//		if (!areSiblings)
	//		{
	//			if (source_rc->parent != ENTITY_INVALID)
	//			{
	//				RelationshipComponent* originalParent_rc = scene->Get<RelationshipComponent>(source_rc->parent);
	//				originalParent_rc->childrenCnt--;
	//				originalParent_rc->first = source_rc->next != ENTITY_INVALID ? source_rc->next : source_rc->previous;
	//			}
	//
	//			/* add new parent */
	//			if (target_rc->parent != ENTITY_INVALID)
	//			{
	//				RelationshipComponent* originalParent_rc = scene->Get<RelationshipComponent>(target_rc->parent);
	//				originalParent_rc->childrenCnt++;
	//			}
	//			source_rc->parent = target_rc->parent;
	//		}
	//
	//		/* update previous */
	//		if (target_rc->previous != ENTITY_INVALID)
	//		{
	//			RelationshipComponent* prevTarget_rc = scene->Get<RelationshipComponent>(target_rc->previous);
	//			prevTarget_rc->next = source; 
	//			source_rc->previous = target_rc->previous;
	//		}
	//		else
	//		{
	//			if (target_rc->parent != ENTITY_INVALID)
	//			{
	//				RelationshipComponent* originalParent_rc = scene->Get<RelationshipComponent>(target_rc->parent);
	//				originalParent_rc->first = source;
	//			}
	//			source_rc->previous = ENTITY_INVALID;
	//		}
	//
	//		source_rc->next = target;
	//		target_rc->previous = source;
	//	}
	//}

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
					parentRC->last = ENTITY_INVALID;
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