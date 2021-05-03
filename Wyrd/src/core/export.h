#pragma once

#ifdef OSR_SHARED_LIB
#ifdef OSR_LIBRARY_EXPORT
#define WYRD_LIBRARY_API __declspec(dllexport)
#else
#define WYRD_LIBRARY_API __declspec(dllimport)
#endif
#else
#define WYRD_LIBRARY_API 
#endif

#ifdef OSR_ENABLE_ASSERTS
	#define OSR_ASSERT(x, ...) { if(!(x)) { OSR_CLIENT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define OSR_CORE_ASSERT(x, ...) { if(!(x)) { WYRD_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define OSR_ASSERT(x, ...)
	#define OSR_CORE_ASSERT(x, ...)
#endif