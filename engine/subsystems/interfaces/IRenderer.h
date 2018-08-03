#ifndef _I_RENDERER_H_
#define _I_RENDERER_H_

#include <iostream>
#include <string.h>

#include "ILogger.h"

#include "subsystems\interfaces\ISubSystem.h"
#include "subsystems\interfaces\IWindow.h"

using namespace std;

namespace OrisisEngine
{
	class IRenderer : public ISubSystem{
	public:
		virtual bool Initialise(IWindow* window) = 0;
		virtual void Shutdown() = 0;
	};
}

#endif