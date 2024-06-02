#ifdef WYRD_PLATFORM_WINDOWS
#define _CRT_SECURE_NO_WARNINGS
#endif

/* System Headers */
#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <regex>
#include <mutex>
#include <bitset>
#include <math.h>
#include <thread>

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

/* Wyrd Common Headers */
#include "core/Structures.h"
#include "serial/LogTypeFunctions.h"
#include "core/Log.h"

#ifdef WYRD_PLATFORM_WINDOWS	
#define NOMINMAX
	#include <Windows.h>
#endif