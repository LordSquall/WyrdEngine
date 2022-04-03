
#include "wyrdpch.h"
#include "Log.h"

/* external includes */
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Wyrd {


	//std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	//std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	//std::shared_ptr<spdlog::logger>& Log::GetCoreLogger() { return s_CoreLogger; }
	//std::shared_ptr<spdlog::logger>& Log::GetClientLogger() { return s_ClientLogger; }

	void Log::Init()
	{
		//spdlog::set_pattern("%^[%T] %n: %v%$");

		//s_CoreLogger = spdlog::stdout_color_mt("Wyrd");
		//s_CoreLogger->set_level(spdlog::level::trace);
		
		//s_ClientLogger = spdlog::stdout_color_mt("App");
		//s_ClientLogger->set_level(spdlog::level::trace);
	}


	template<typename FormatString, typename... Args>
	void Log::CoreTrace(const char* fmt, Args&&...args)
	{
		//s_CoreLogger->trace(fmt, args);
	}

	template<typename FormatString, typename... Args>
	void Log::CoreInfo(const char* fmt, Args&&...args)
	{
		//s_CoreLogger->info(fmt, args);
	}

	template<typename FormatString, typename... Args>
	void Log::CoreWarning(const char* fmt, Args&&...args)
	{
		//s_CoreLogger->warn(fmt, args);
	}

	template<typename FormatString, typename... Args>
	void Log::CoreError(const char* fmt, Args&&...args)
	{
		//s_CoreLogger->error(fmt, args);
	}

	template<typename FormatString, typename... Args>
	void Log::ClientTrace(const char* fmt, Args&&...args)
	{
		//s_ClientLogger->trace(fmt, args);
	}

	template<typename FormatString, typename... Args>
	void Log::ClientInfo(const char* fmt, Args&&...args)
	{
		//s_ClientLogger->info(fmt, args);
	}

	template<typename FormatString, typename... Args>
	void Log::ClientWarning(const char* fmt, Args&&...args)
	{
		//s_ClientLogger->warn(fmt, args);
	}

	template<typename FormatString, typename... Args>
	void Log::ClientError(const char* fmt, Args&&...args)
	{
		//s_ClientLogger->error(fmt, args);
	}
}