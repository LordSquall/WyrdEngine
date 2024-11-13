#pragma once

#include "core/export.h"
#include "core/scene/Scene.h"
#include "core/ecs/Components.h"

namespace Wyrd
{
	template<typename... ComponentTypes>
	struct WYRD_LIBRARY_API HierarchyEntitySet
	{
		HierarchyEntitySet(Scene& scene) : scene(&scene)
		{
			if (sizeof ...(ComponentTypes) == 0)
			{
				all = true;
			}
			else
			{
				int componentIds[] = { 0, GetID<ComponentTypes>() ... };
				for (int i = 1; i < (sizeof...(ComponentTypes) + 1); i++)
				{
					componentMask.set(componentIds[i]);
				}
			}
		}

		struct Iterator
		{
			Iterator(Scene* scene, Entity entity, EntityIndex index, ComponentMask mask, bool all) :
				scene(scene), entity(entity), index(index), mask(mask), all(all) {}

			Entity operator*() const
			{
				return scene->entities[entity-1].id;
			}

			bool operator==(const Iterator& other) const
			{
				return index == other.index || index == scene->entities.size();
			}

			bool operator!=(const Iterator& other) const
			{
				return index != other.index && index != scene->entities.size();
			}

			Iterator& operator++()
			{
				RelationshipComponent* rc = scene->Get<RelationshipComponent>(entity);
				if (rc->next != ENTITY_INVALID)
				{
					entity = rc->next;
				}
				index++;

				return(*this);
			}

			bool ValidIndex()
			{
				return
					// It's a valid entity ID
					IsEntityValid(scene->entities[index].id) &&
					// It has the correct component mask
					(all || mask == (mask & scene->entities[index].mask));
			}

			Entity entity;
			EntityIndex index;
			Scene* scene;
			ComponentMask mask;
			bool all{ false };
		};

		const Iterator begin() const
		{
			int firstIndex = 0;

			if (scene->entities.size() != 0)
			{
				while (firstIndex < scene->entities.size() &&
					(componentMask != (componentMask & scene->entities[firstIndex].mask)
						|| !IsEntityValid(scene->entities[firstIndex].id)))
				{
					firstIndex++;
				}
				return Iterator(scene, scene->entities[0].id, firstIndex, componentMask, all);
			}
			else
			{
				return Iterator(scene, ENTITY_INVALID, ENTITY_INVALID, componentMask, all);
			}
		}

		const Iterator end() const
		{
			return Iterator(scene, ENTITY_INVALID, EntityIndex(scene->entities.size()), componentMask, all);
		}

		Scene* scene{ nullptr };
		ComponentMask componentMask;
		bool all{ false };
	};
};