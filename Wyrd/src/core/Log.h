#pragma once

/* local includes */
#include "core/export.h"

//#include <spdlog/fwd.h>

namespace Wyrd {

	/**
	 * @brief Logger Subsystem
	 * Provides a number of logging functions and defines to allow logging to the console from anywhere within the application
	 * Also provides internal logging for the core functions
	*/
	class WYRD_LIBRARY_API Log
	{
	public:
		/**
		 * @brief Initialise all single use resources
		*/
		static void Init();

		/**
		 * @brief Get the Core Logger used for all Core library logging functions
		 * @return spd::logger - Core Logger
		*/
		//static std::shared_ptr<spdlog::logger>& GetCoreLogger();

		/**
		 * @brief Get the Client Logger used for all User Appilication logging functions
		 * @return spd::logger - Client Logger
		*/
		//static std::shared_ptr<spdlog::logger>& GetClientLogger();

		template<typename FormatString, typename... Args>
		static void CoreTrace(const char* fmt, Args&&...args);

		template<typename FormatString, typename... Args>
		static void CoreInfo(const char* fmt, Args&&...args);

		template<typename FormatString, typename... Args>
		static void CoreWarning(const char* fmt, Args&&...args);

		template<typename FormatString, typename... Args>
		static void CoreError(const char* fmt, Args&&...args);

		template<typename FormatString, typename... Args>
		static void ClientTrace(const char* fmt, Args&&...args);

		template<typename FormatString, typename... Args>
		static void ClientInfo(const char* fmt, Args&&...args);

		template<typename FormatString, typename... Args>
		static void ClientWarning(const char* fmt, Args&&...args);

		template<typename FormatString, typename... Args>
		static void ClientError(const char* fmt, Args&&...args);

	private:
		//static std::shared_ptr<spdlog::logger> s_CoreLogger;
		//static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Log macros
#define WYRD_CORE_TRACE(...)		//::Wyrd::Log::CoreTrace(__VA_ARGS__)
#define WYRD_CORE_INFO(...)			//::Wyrd::Log::CoreInfo(__VA_ARGS__)
#define WYRD_CORE_WARN(...)			//::Wyrd::Log::CoreWarning(__VA_ARGS__)
#define WYRD_CORE_ERROR(...)		//::Wyrd::Log::CoreError(__VA_ARGS__)

#define WYRD_TRACE(...)		//::Wyrd::Log::ClientTrace(__VA_ARGS__)
#define WYRD_INFO(...)		//::Wyrd::Log::ClientInfo(__VA_ARGS__)
#define WYRD_WARN(...)		//::Wyrd::Log::ClientWarning(__VA_ARGS__)
#define WYRD_ERROR(...)		//::Wyrd::Log::ClientError(__VA_ARGS__)