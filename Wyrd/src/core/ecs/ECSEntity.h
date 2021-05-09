#pragma once

#include <wyrdpch.h>

typedef uint32_t ECSEntity;

namespace Wyrd
{
	ECSEntity CreateEntity()
	{
		static ECSEntity next = 0;
		return ++next;
	}
}