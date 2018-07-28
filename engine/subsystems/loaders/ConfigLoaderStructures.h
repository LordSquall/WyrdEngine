#ifndef _CONFIG_LOADER_STRUCTURES_H_
#define _CONFIG_LOADER_STRUCTURES_H_

namespace OrisisEngine
{
	struct FrameworkConfig {
		int width;
		int height;
		int x;
		int y;

		FrameworkConfig() : width(800), height(600), x(0), y(0)
		{

		}

		FrameworkConfig(int w, int h, int x, int y) : width(w), height(h), x(x), y(y)
		{

		}
	};
}

#endif