#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include <system_error>

namespace rtr
{
	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		sEngineLogger = spdlog::stdout_color_mt("ENGINE");
		sEngineLogger->set_level(spdlog::level::trace);

		sGameLogger = spdlog::stdout_color_mt("GAME");
		sGameLogger->set_level(spdlog::level::trace);
	}

	std::string Log::GetErrorMessageForResultCode(int result)
	{
		return std::system_category().message(result);
	}

	std::shared_ptr<spdlog::logger> Log::sEngineLogger;
	std::shared_ptr<spdlog::logger> Log::sGameLogger;

}