#include "Log.h"

void Log::log(std::string_view pMessage, TypeOfProblem pTypeOfProblem, 
			  const std::filesystem::path& pFilePath, 
			  int32_t pLine)
{
	auto tmpTime = std::chrono::system_clock::now();
	auto nowTime = std::chrono::system_clock::to_time_t(tmpTime);

	std::tm time;
	localtime_s(&time, &nowTime);

	std::ostringstream streamTime;
	std::cout << std::put_time(&time, "%Y-%m-%d %H:%M:%S");
	
	switch (pTypeOfProblem)
	{
	case Log::TypeOfProblem::WARNING:
		manageWarning(pMessage, streamTime.str(), pFilePath, pLine);
		break;
	case Log::TypeOfProblem::ERROR:
		manageError(pMessage, streamTime.str(), pFilePath, pLine);
		break;
	case Log::TypeOfProblem::CRITICAL_ERROR:
		manageCriticalError(pMessage, streamTime.str(), pFilePath, pLine);
		break;
	default:
		std::cout << std::format("You cant reach this point: {}\n", std::string(pMessage));
		break;
	}
}

void Log::manageWarning(std::string_view pMessage, std::string_view pTime,
						const std::filesystem::path& pFilePath, 
						int32_t pLine)
{
	std::cout << std::format("[WARNING][TIME: {}][MESSAGE: {}]", std::string(pTime), std::string(pMessage));
	
	if (!pFilePath.empty())
	{
		std::cout << std::format("[FILE: {}][LINE: {}]", pFilePath.string(), 
														 pLine > 0 ? std::to_string(pLine) : "0");
	}
}

void Log::manageError(std::string_view pMessage, std::string_view pTime,
					  const std::filesystem::path& pFilePath, 
					  int32_t pLine)
{
	std::cout << std::format("[ERROR][TIME: {}][MESSAGE: {}]", std::string(pTime), std::string(pMessage));

	if (!pFilePath.empty())
	{
		std::cout << std::format("[FILE: {}][LINE: {}]", pFilePath.string(),
			pLine > 0 ? std::to_string(pLine) : "0");
	}
	
	exit(1);
}

void Log::manageCriticalError(std::string_view pMessage, std::string_view pTime,
							  const std::filesystem::path& pFilePath, 
							  int32_t pLine)
{
	std::cout << std::format("[CRITICAL ERROR][TIME: {}][MESSAGE: {}]", std::string(pTime), std::string(pMessage));

	if (!pFilePath.empty())
	{
		std::cout << std::format("[FILE: {}][LINE: {}]", pFilePath.string(),
			pLine > 0 ? std::to_string(pLine) : "0");
	}
	
	exit(1);
}
