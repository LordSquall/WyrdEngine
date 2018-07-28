#ifndef _I_RENDERER_H_
#define _I_RENDERER_H_

#include <iostream>
#include <string.h>

#include "ILogger.h"

#include "subsystems\interfaces\ISubSystem.h"

using namespace std;

namespace OrisisEngine
{
	class IRenderer : public ISubSystem{
	public:
		virtual bool Initialise() = 0;
		virtual void Shutdown() = 0;
	};
}

#endif