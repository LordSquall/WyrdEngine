#pragma once

#include "config.h"

#ifdef OSR_ENABLE_ASSERTS
	#define OSR_ASSERT(x, ...) { if(!(x)) { OSR_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define OSR_CORE_ASSERT(x, ...) { if(!(x)) { OSR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define OSR_ASSERT(x, ...)
	#define OSR_CORE_ASSERT(x, ...)
#endif