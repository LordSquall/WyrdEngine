#ifndef _SPRITE_LAYER_LOADER_H_
#define _SPRITE_LAYER_LOADER_H_

#include "export.h"
#include "subsystems\interfaces\ISubSystem.h"
#include "subsystems\interfaces\ILogger.h"
#include "subsystems\interfaces\IRenderer.h"

#include "subsystems\sprites\SpriteSystem.h"

namespace OrisisEngine
{
	class SpriteLayerLoader {
		public:
			SpriteLayerLoader();
			SpriteLayerLoader(const SpriteLayerLoader &obj);
			~SpriteLayerLoader();

			static int LoadFile(string filePath, ILogger* logger, SpriteSystem* data);
	};
}

#endif