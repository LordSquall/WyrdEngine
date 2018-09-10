#ifndef _SPRITE_LOADER_H_
#define _SPRITE_LOADER_H_

#include "export.h"
#include "subsystems\interfaces\ISubSystem.h"
#include "subsystems\interfaces\ILogger.h"
#include "subsystems\interfaces\IRenderer.h"

#include "subsystems\sprites\SpriteSystem.h"

namespace OrisisEngine
{
	class SpriteLoader {
		public:
			SpriteLoader();
			SpriteLoader(const SpriteLoader &obj);
			~SpriteLoader();

			static int LoadFile(string filePath, ILogger* logger, SpriteSystem* data);
	};
}

#endif