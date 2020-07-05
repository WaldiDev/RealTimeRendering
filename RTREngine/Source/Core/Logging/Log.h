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

		static std::string GetErrorMessageForResultCode(int result);

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

#define RTR_PRINT_RESULT(result) (result < 0) ? RTR_ENGINE_ERROR(rtr::Log::GetErrorMessageForResultCode(result)) : RTR_ENGINE_ERROR(rtr::Log::GetErrorMessageForResultCode(result))
#define RTR_PRINT_RESULT_WITH_MESSAGE(message, result) (result < 0) ? RTR_ENGINE_ERROR("{0}: {1}", message, rtr::Log::GetErrorMessageForResultCode(result)) : RTR_ENGINE_INFO("{0}:{1}", message, rtr::Log::GetErrorMessageForResultCode(result))
