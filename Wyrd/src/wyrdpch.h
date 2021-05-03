#pragma once

#ifdef WYRD_PLATFORM_WINDOWS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <regex> 
#include <mutex>

#include <string>
#include <optional>
#include <ostream>
#include <sstream>
#include <vector>
#include <array>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <iterator>

#ifdef WYRD_PLATFORM_WINDOWS	
	#include <Windows.h>
#endif