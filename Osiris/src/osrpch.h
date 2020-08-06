#pragma once

#ifdef OSR_PLATFORM_WINDOWS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <ostream>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <iterator>

#ifdef OSR_PLATFORM_WINDOWS	
	#include <Windows.h>
#endif