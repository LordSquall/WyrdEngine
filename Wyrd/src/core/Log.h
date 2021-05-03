#pragma once

/* local includes */
#include "core/export.h"

/* external includes */
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

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
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

		/**
		 * @brief Get the Client Logger used for all User Appilication logging functions
		 * @return spd::logger - Client Logger
		*/
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Log macros
#define WYRD_CORE_TRACE(...)		::Wyrd::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define WYRD_CORE_INFO(...)		::Wyrd::Log::GetCoreLogger()->info(__VA_ARGS__)
#define WYRD_CORE_WARN(...)		::Wyrd::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define WYRD_CORE_ERROR(...)		::Wyrd::Log::GetCoreLogger()->error(__VA_ARGS__)

#define WYRD_TRACE(...)		::Wyrd::Log::GetClientLogger()->trace(__VA_ARGS__)
#define WYRD_INFO(...)		::Wyrd::Log::GetClientLogger()->info(__VA_ARGS__)
#define WYRD_WARN(...)		::Wyrd::Log::GetClientLogger()->warn(__VA_ARGS__)
#define WYRD_ERROR(...)		::Wyrd::Log::GetClientLogger()->error(__VA_ARGS__)