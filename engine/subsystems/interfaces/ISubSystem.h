#ifndef _I_SUBSYSTEM_H_
#define _I_SUBSYSTEM_H_

#include <iostream>
#include <string.h>

#include "subsystems\interfaces\ILogger.h"

using namespace std;

namespace OrisisEngine
{
	class ISubSystem {
	public:
		virtual void RegisterLogger(ILogger* logger) = 0;
		virtual ILogger* GetLogger() = 0;
	};
}

#endif