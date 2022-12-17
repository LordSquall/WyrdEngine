#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
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
			bool isSiblings = source_rc->parent == target_rc->parent;
			bool isFirst = source_rc->previous == ENTITY_INVALID;
			bool isLast = source_rc->next == ENTITY_INVALID;

			/* we need to first the source node from the structure, this has been accounted for in the local list, plus parent structure */
			if (isFirst)
			{
				/* as we are the first in the local list, we just need to update the parent (if we have one) to point to the next as it's 'first' */
				if (source_rc->parent != ENTITY_INVALID)
				{
					RelationshipComponent* sourceParent_rc = scene->Get<RelationshipComponent>(source_rc->parent);
					sourceParent_rc->first = source_rc->next;
					sourceParent_rc->childrenCnt--;
				}

				/* we are a double linked-list so we need to update the counter link */
				if (source_rc->next != ENTITY_INVALID)
				{
					RelationshipComponent* sourceNext_rc = scene->Get<RelationshipComponent>(source_rc->next);
					sourceNext_rc->previous = ENTITY_INVALID;
				}
			}
			else
			{
				/* we are removing a node from the middle of a list, therefore we need to update both next and previous links*/
				{
					RelationshipComponent* sourcePrev_rc = scene->Get<RelationshipComponent>(source_rc->previous);
					sourcePrev_rc->next = source_rc->next;

					if (!isLast)
					{
						RelationshipComponent* sourceNext_rc = scene->Get<RelationshipComponent>(source_rc->next);
						sourceNext_rc->previous = source_rc->previous;
					}

					if (source_rc->parent != ENTITY_INVALID)
					{
						RelationshipComponent* sourceParent_rc = scene->Get<RelationshipComponent>(source_rc->parent);
						sourceParent_rc->childrenCnt--;
					}
				}
			}

			source_rc->parent = ENTITY_INVALID;
			source_rc->next = ENTITY_INVALID;
			source_rc->previous = ENTITY_INVALID;

			if (addOp == AddOp::On)
			{
				/* in this case we are adding the first child */
				if (target_rc->first == ENTITY_INVALID)
				{
					target_rc->first = source;
					target_rc->childrenCnt++;
					source_rc->parent = target;
				}
				else
				{
					/* as we are added to the end of a list, we need to first the last entity to attach to*/
					Entity targetLast = target_rc->first;
					for (int i = 0; i < target_rc->childrenCnt-1; i++)
					{
						RelationshipComponent* next_rc = scene->Get<RelationshipComponent>(targetLast);
						targetLast = next_rc->next;
					}

					RelationshipComponent* targetLast_rc = scene->Get<RelationshipComponent>(targetLast);
					targetLast_rc->next = source;
					target_rc->previous = targetLast;
					target_rc->childrenCnt++;
					source_rc->parent = target;
				}
			}
			else if (addOp == AddOp::After)
			{
				/* in this case we are adding the first child */
				if (target_rc->first == ENTITY_INVALID)
				{
					target_rc->first = source;
					target_rc->childrenCnt++;
					source_rc->parent = target;
				}
				else
				{
					/* as we are added to the end of a list, we need to first the last entity to attach to*/
					Entity targetLast = target_rc->first;
					RelationshipComponent* targetLast_rc = scene->Get<RelationshipComponent>(targetLast);
					targetLast_rc->next = source;
					target_rc->previous = targetLast;
					target_rc->childrenCnt++;
					source_rc->parent = target;
				}
			}
			else if (addOp == AddOp::Before)
			{
				/* update the parent count if not siblings */
				if (target_rc->parent != ENTITY_INVALID)
				{
					RelationshipComponent* targetParent_rc = scene->Get<RelationshipComponent>(target_rc->parent);
					targetParent_rc->childrenCnt++;
				}
				source_rc->parent = target_rc->parent;

				/* we should be able to just insert the entity into the double linked list */
				if (target_rc->previous != ENTITY_INVALID)
				{
					RelationshipComponent* targetPrev_rc = scene->Get<RelationshipComponent>(target_rc->previous);
					targetPrev_rc->next = source;
					source_rc->previous = target_rc->previous;
				}
				else
				{
					/* in this case we are added to the first item in the list, so the parent need to be updated */
					if (target_rc->parent != ENTITY_INVALID)
					{
						RelationshipComponent* targetParent_rc = scene->Get<RelationshipComponent>(target_rc->parent);
						targetParent_rc->first = source;
					}
				}
				if (target_rc->next != ENTITY_INVALID)
				{
					RelationshipComponent* targetNext_rc = scene->Get<RelationshipComponent>(target_rc->next);
					targetNext_rc->previous = source;
					source_rc->next = target_rc->next;
				}

				source_rc->next = target;
				target_rc->previous = source;
			}

			//if (isFirst)
			//{
			//	if (source_rc->previous != ENTITY_INVALID)
			//	{
			//		RelationshipComponent* prevSource_rc = scene->Get<RelationshipComponent>(source_rc->previous);
			//		prevSource_rc->next = source_rc->next;
			//	}
			//
			//	if (source_rc->next != ENTITY_INVALID)
			//	{
			//		RelationshipComponent* nextSource_rc = scene->Get<RelationshipComponent>(source_rc->next);
			//		nextSource_rc->previous = source_rc->previous;
			//	}
			//}
			//
			///* if we had a parent before ... */
			//if (source_rc->parent != ENTITY_INVALID)
			//{
			//	RelationshipComponent* originalParent_rc = scene->Get<RelationshipComponent>(source_rc->parent);
			//	originalParent_rc->childrenCnt--;
			//
			//	if (isFirst)
			//	{
			//		originalParent_rc->first = source_rc->next != ENTITY_INVALID ? source_rc->next : source_rc->previous;
			//	}
			//}
			//
			///* set the parent link */
			//source_rc->parent = parent;
			//
			///* increment the parent child count */
			//target_rc->childrenCnt += 1;
			//
			///* add new child to start of child list */
			//RelationshipComponent* firstChildRC = scene->Get<RelationshipComponent>(target_rc->first);
			//if (firstChildRC != nullptr)
			//{
			//	firstChildRC->previous = child;
			//	source_rc->next = target_rc->first;
			//	target_rc->first = child;
			//}
			//else
			//{
			//	target_rc->first = child;
			//}
		}
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