#pragma once
#include <iostream>
#include <format>
#include <filesystem>
#include <string>
#include <chrono>
#include <ctime>

#include "Singleton.h"

class Log 
{
public:
	enum class TypeOfProblem;
public:
	void log(std::string_view pMessage,
			 TypeOfProblem pTypeOfProblem,
			 const std::filesystem::path& pFilePath = __FILE__,
			 int32_t pLine = __LINE__);
private:
	Log() = default;
	~Log() = default;
	Log(const Log&) = delete;
	Log& operator=(const Log&) = delete;
	Log(Log&&) = delete;
	Log& operator=(Log&&) = delete;

	void manageWarning(std::string_view pMessage, std::string_view pTime, 
					   const std::filesystem::path& pFilePath,
					   int32_t pLine);
	void manageError(std::string_view pMessage, std::string_view pTime,
					 const std::filesystem::path& pFilePath,
					 int32_t pLine);
	void manageCriticalError(std::string_view pMessage, std::string_view pTime,
							 const std::filesystem::path& pFilePath,
							 int32_t pLine);

private:
	enum class TypeOfProblem
	{
		WARNING = 0,
		ERROR = 1,
		CRITICAL_ERROR = 2
	};
};

