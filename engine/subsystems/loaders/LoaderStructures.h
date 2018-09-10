#ifndef _SPRITESHEET_LOADER_STRUCTURES_H_
#define _SPRITESHEET_LOADER_STRUCTURES_H_

#include <cstring>
#include <string>
#include <vector>

using namespace std;

namespace OrisisEngine
{
	struct SpriteData {
		int w;
		int h;
		float u;
		float v;

		SpriteData(int w, int h, float u, float v) : w(w), h(h), u(u), v(v)
		{ 
		}
	};

	struct SpriteSheetData {
		
		string textureFile;

		int width;
		int height;

		vector<SpriteData*> entries;
		
		SpriteSheetData()
		{
			
		}
	};
}

#endif