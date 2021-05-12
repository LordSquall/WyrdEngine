#pragma once

#include <core/ecs/Entity.h>

struct ComponentPool
{
	ComponentPool(size_t elementSize)
	{
		this->elementSize = elementSize;
		data = new char[this->elementSize * MAX_ENTITIES];
	}


	~ComponentPool()
	{
		delete[] data;
	}

	inline void* get(size_t index)
	{
		// looking up the component at the desired index
		return data + index * elementSize;
	}

	char* data{ nullptr };
	size_t elementSize{ 0 };
};