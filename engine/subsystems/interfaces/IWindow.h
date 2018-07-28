#ifndef _I_WINDOW_H_
#define _I_WINDOW_H_

#include <iostream>
#include <string.h>

#include "ILogger.h"
#include "subsystems\loaders\ConfigLoaderStructures.h"

using namespace std;

namespace OrisisEngine
{
	class IWindow {
	public:
		virtual void RegisterLogger(ILogger* logger) = 0;
		virtual bool Create(FrameworkConfig* config) = 0;
		virtual void Shutdown() = 0;
		virtual bool IsRunning() = 0;
	};
}

#endif