#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "export.h"

#include "subsystems\interfaces\ISubSystem.h"
#include "subsystems\interfaces\ILogger.h"
#include "subsystems\interfaces\IRenderer.h"

#include <vector>

using namespace std;

namespace OrisisEngine
{
	class Sprite {
	public:
		Sprite(string name, int width, int height);
		Sprite(string name, int width, int height, Texture* texture);
		Sprite(const Sprite &obj);
		~Sprite();

		Texture* GetTexture();

	public:
		string Name;
		int Width;
		int Height;
		float TextureU[4]; 
		float TextureV[4];

	private:
		Texture* _texture;
	};
}

#endif