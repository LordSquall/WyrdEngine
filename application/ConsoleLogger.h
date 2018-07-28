#ifndef _CONSOLE_LOGGER_H_
#define _CONSOLE_LOGGER_H_

#include "subsystems\interfaces\ILogger.h"

namespace OrisisEngine
{
	class ConsoleLogger : public ILogger {
	public:
		ConsoleLogger();
		void LogDebug(string msg);
		void LogInfo(string msg);
		void LogWarning(string msg);
		void LogError(string msg);
	};
}

#endif