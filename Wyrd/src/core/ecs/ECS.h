#pragma once

#include "wyrdpch.h"
#include "core/export.h"

const int MAX_ENTITIES = 1000;
const int MAX_COMPONENTS = 64;
const unsigned long long ENTITY_INVALID = 0;

namespace Wyrd 
{
	typedef unsigned long long Entity;

	typedef unsigned int EntityIndex;

	typedef std::bitset<MAX_COMPONENTS> ComponentMask;

	struct WYRD_LIBRARY_API EntityDesc
	{
		Entity id;
		ComponentMask mask;
	};

	extern int _ComponentCounter;

	template <class ComponentType>
	int GetID()
	{
		static int _ComponentID = _ComponentCounter++;
		return _ComponentID;
	}

	inline bool IsEntityValid(Wyrd::Entity id)
	{
		// Check if the index is our invalid index
		return (id >> 32) != EntityIndex(-1);
	}
};