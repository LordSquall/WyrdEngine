#include "osrpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_sinks.h"

namespace Osiris {


	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;


	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger = spdlog::stdout_logger_mt("Osiris");
		s_CoreLogger->set_level(spdlog::level::trace);
		
		s_ClientLogger = spdlog::stdout_logger_mt("App");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}