#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace rtr
{
	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return sEngineLogger; }
		static std::shared_ptr<spdlog::logger>& GetGameLogger() { return sGameLogger; }

	private:
		static std::shared_ptr<spdlog::logger> sEngineLogger;
		static std::shared_ptr<spdlog::logger> sGameLogger;
	};
}

#define RTR_ENGINE_TRACE(...) ::rtr::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define RTR_ENGINE_INFO(...)  ::rtr::Log::GetEngineLogger()->info(__VA_ARGS__)
#define RTR_ENGINE_WARN(...)  ::rtr::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define RTR_ENGINE_ERROR(...) ::rtr::Log::GetEngineLogger()->error(__VA_ARGS__)
#define RTR_ENGINE_FATAL(...) ::rtr::Log::GetEngineLogger()->fatal(__VA_ARGS__)

#define RTR_TRACE(...) ::rtr::Log::GetGameLogger()->trace(__VA_ARGS__)
#define RTR_INFO(...)  ::rtr::Log::GetGameLogger()->info(__VA_ARGS__)
#define RTR_WARN(...)  ::rtr::Log::GetGameLogger()->warn(__VA_ARGS__)
#define RTR_ERROR(...) ::rtr::Log::GetGameLogger()->error(__VA_ARGS__)
#define RTR_FATAL(...) ::rtr::Log::GetGameLogger()->fatal(__VA_ARGS__)