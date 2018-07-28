#ifndef _CONFIG_LOADER_STRUCTURES_H_
#define _CONFIG_LOADER_STRUCTURES_H_

#include <cstring>
#include <string>

using namespace std;

namespace OrisisEngine
{
	struct FrameworkConfig {
		int width;
		int height;
		int x;
		int y;
		char title[255];

		FrameworkConfig() : width(800), height(600), x(0), y(0), title()
		{
			memcpy(title, "Application", sizeof(title));
		}

		FrameworkConfig(int w, int h, int x, int y, string t) : width(w), height(h), x(x), y(y), title()
		{
			memcpy(title, t.c_str(), sizeof(title));
		}
	};
}

#endif