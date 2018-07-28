#ifndef _I_LOGGER_H_
#define _I_LOGGER_H_

#include <iostream>
#include <string.h>

using namespace std;

namespace OrisisEngine
{
	class ILogger {
	public:
		virtual void LogDebug(string msg) = 0;
		virtual void LogInfo(string msg) = 0;
		virtual void LogWarning(string msg) = 0;
		virtual void LogError(string msg) = 0;
	};
}

#endif