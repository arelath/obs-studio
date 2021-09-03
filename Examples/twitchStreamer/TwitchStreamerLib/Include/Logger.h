#pragma once

#include <string>
#include <fstream>

class Logger {
public:
	Logger(const std::string & filename);

	void LogStringChunk(char *str, int log_level);

private:
	std::string GetCurrentTimeString();
	
	void LogString(const std::string &timeString, const std::string &str, int logLevel);

	std::ofstream logFile;
};
