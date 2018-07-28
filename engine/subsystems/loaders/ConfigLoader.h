#ifndef _CONFIG_LOADER_H_
#define _CONFIG_LOADER_H_

#include "export.h"
#include "subsystems\loaders\ConfigLoaderStructures.h"
#include "subsystems\interfaces\ISubSystem.h"
#include "subsystems\interfaces\ILogger.h"
#include "subsystems\interfaces\IRenderer.h"


namespace OrisisEngine
{
	class FrameworkConfigLoader {
		public:
			FrameworkConfigLoader();
			FrameworkConfigLoader(const FrameworkConfigLoader &obj);
			~FrameworkConfigLoader();

			static FrameworkConfig LoadFile(string filePath);
	};
}

#endif