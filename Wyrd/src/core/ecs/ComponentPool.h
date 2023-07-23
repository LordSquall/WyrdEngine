#pragma once

#include "core/export.h"

namespace Wyrd
{
	struct ComponentPool;

	typedef void (*ComponentResetFunc)(ComponentPool* pool, Entity entity);

	struct WYRD_LIBRARY_API ComponentPool
	{
		ComponentPool(const std::string& name, int32_t idx, size_t elementSize, const std::string& scriptName, bool serialise, ComponentResetFunc resetFunc)
		{
			this->name = name;
			this->idx = idx;
			this->elementSize = elementSize;
			this->scriptName = scriptName;
			this->serialise = serialise;
			this->data = new char[this->elementSize * (MAX_ENTITIES + 1)];
			memset(this->data, 0, this->elementSize * (MAX_ENTITIES + 1));
			this->resetFunc = resetFunc;
		}

		~ComponentPool()
		{
			delete[] data;
		}

		inline void* get(size_t index)
		{
			return data + (index - 1) * elementSize;
		}

		uint32_t idx{ 0 };

		std::string name{ "" };

		std::string scriptName{ "" };

		char* data{ nullptr };

		uint32_t count{ 0 };

		uint32_t used{ 0 };

		size_t elementSize{ 0 };

		bool serialise{ true };

		ComponentResetFunc resetFunc;
	};
};