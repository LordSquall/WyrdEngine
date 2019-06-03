#pragma once

#include "core/export.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Osiris {

	class  Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
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
#define OSR_CORE_FATAL(...)		::Osiris::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define OSR_TRACE(...)		::Osiris::Log::GetClientLogger()->trace(__VA_ARGS__)
#define OSR_INFO(...)		::Osiris::Log::GetClientLogger()->info(__VA_ARGS__)
#define OSR_WARN(...)		::Osiris::Log::GetClientLogger()->warn(__VA_ARGS__)
#define OSR_ERROR(...)		::Osiris::Log::GetClientLogger()->error(__VA_ARGS__)
#define OSR_FATAL(...)		::Osiris::Log::GetClientLogger()->fatal(__VA_ARGS__)