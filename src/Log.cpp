#include "Log.h"

void Log::log(std::string_view pMessage, TypeOfProblem pTypeOfProblem, 
			  const std::filesystem::path& pFilePath, 
			  int32_t pLine)
{
	auto tmpTime = std::chrono::system_clock::now();
	auto nowTime = std::chrono::system_clock::to_time_t(tmpTime);

	std::tm time;
	localtime_s(&time, &nowTime);

	std::string timeStr = [&]() -> std::string
		{
			std::ostringstream streamTime;
			streamTime << std::put_time(&time, "%Y-%m-%d %H:%M:%S");
			return streamTime.str();
		}();
	
	std::string message = std::string(pMessage);
	correctMessage(message);

	switch (pTypeOfProblem)
	{
	case Log::TypeOfProblem::WARNING:
		manageWarning(message, timeStr, pFilePath, pLine);
		break;
	case Log::TypeOfProblem::ERROR:
		manageError(message, timeStr, pFilePath, pLine);
		break;
	case Log::TypeOfProblem::CRITICAL_ERROR:
		manageCriticalError(message, timeStr, pFilePath, pLine);
		break;
	default:
		std::cout << std::format("You cant reach this point: {}\n", message);
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
		std::cout << std::format("[FILE: {}][LINE: {}]\n", pFilePath.string(), 
														   pLine > 0 ? std::to_string(pLine) : "0");
	}
	else 
		std::cout << '\n';
}

void Log::manageError(std::string_view pMessage, std::string_view pTime,
					  const std::filesystem::path& pFilePath, 
					  int32_t pLine)
{
	std::cout << std::format("[ERROR][TIME: {}][MESSAGE: {}]", std::string(pTime), std::string(pMessage));

	if (!pFilePath.empty())
	{
		std::cout << std::format("[FILE: {}][LINE: {}]\n", pFilePath.string(),
														   pLine > 0 ? std::to_string(pLine) : "0");
	}
	else
		std::cout << '\n';

	exit(1);
}

void Log::manageCriticalError(std::string_view pMessage, std::string_view pTime,
							  const std::filesystem::path& pFilePath, 
							  int32_t pLine)
{
	std::cout << std::format("[CRITICAL ERROR][TIME: {}][MESSAGE: {}]", std::string(pTime), std::string(pMessage));

	if (!pFilePath.empty())
	{
		std::cout << std::format("[FILE: {}][LINE: {}]\n", pFilePath.string(),
														   pLine > 0 ? std::to_string(pLine) : "0");
	}
	else
		std::cout << '\n';
	
	exit(1);
}

void Log::correctMessage(std::string& pMessage)
{
	if (!pMessage.empty())
	{
		char spaceSymbol = pMessage[pMessage.size() - 1];
		if (std::isspace(spaceSymbol))
			pMessage.pop_back();
	}
}
