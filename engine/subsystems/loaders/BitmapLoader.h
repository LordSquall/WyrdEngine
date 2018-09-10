#ifndef _BITMAP_LOADER_H_
#define _BITMAP_LOADER_H_

#include "export.h"
#include "subsystems\interfaces\ISubSystem.h"
#include "subsystems\interfaces\ILogger.h"
#include "subsystems\interfaces\IRenderer.h"


namespace OrisisEngine
{
	class BitmapLoader {
		public:
			BitmapLoader();
			BitmapLoader(const BitmapLoader &obj);
			~BitmapLoader();

			static void LoadFile(string filePath, ILogger* logger, Texture* texture);
	};
}

#endif