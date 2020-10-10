#pragma once

/* local includes */
#include "core/export.h"

/* external includes */
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Osiris {

	/**
	 * @brief Logger Subsystem
	 * Provides a number of logging functions and defines to allow logging to the console from anywhere within the application
	 * Also provides internal logging for the core functions
	*/
	class OSR_LIBRARY_API Log
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
#define OSR_CORE_TRACE(...)		::Osiris::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define OSR_CORE_INFO(...)		::Osiris::Log::GetCoreLogger()->info(__VA_ARGS__)
#define OSR_CORE_WARN(...)		::Osiris::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define OSR_CORE_ERROR(...)		::Osiris::Log::GetCoreLogger()->error(__VA_ARGS__)

#define OSR_TRACE(...)		::Osiris::Log::GetClientLogger()->trace(__VA_ARGS__)
#define OSR_INFO(...)		::Osiris::Log::GetClientLogger()->info(__VA_ARGS__)
#define OSR_WARN(...)		::Osiris::Log::GetClientLogger()->warn(__VA_ARGS__)
#define OSR_ERROR(...)		::Osiris::Log::GetClientLogger()->error(__VA_ARGS__)