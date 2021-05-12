#pragma once

#include "wyrdpch.h"
#include "core/UID.h"

typedef unsigned long long Entity;

typedef unsigned int EntityIndex;

const int MAX_ENTITIES = 10000;
const int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> ComponentMask;

struct EntityDesc
{
	Entity id;
	ComponentMask mask;
};

extern int s_componentCounter;
template <class T>
int GetId()
{
	static int s_componentId = s_componentCounter++;
	return s_componentId;
}

inline bool IsEntityValid(Entity id)
{
	// Check if the index is our invalid index
	return (id >> 32) != EntityIndex(-1);
}