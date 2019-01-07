#pragma once

#include "config.h"

#ifdef OSR_PLATFORM_WINDOWS
	#ifdef OSR_BUILD_DLL
		#define OSIRIS_API __declspec(dllexport)
	#else
		#define OSIRIS_API __declspec(dllimport)
	#endif
#else
	#error Orisis only supports Windows!
#endif

#ifdef OSR_ENABLE_ASSERTS
	#define OSR_ASSERT(x, ...) { if(!(x)) { OSR_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define OSR_CORE_ASSERT(x, ...) { if(!(x)) { OSR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define OSR_ASSERT(x, ...)
	#define OSR_CORE_ASSERT(x, ...)
#endif