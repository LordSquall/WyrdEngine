#pragma once

#include "core/export.h"

namespace Wyrd
{
	struct WYRD_LIBRARY_API ComponentPool
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
			return data + index * elementSize;
		}

		char* data{ nullptr };

		size_t elementSize{ 0 };
	};
};