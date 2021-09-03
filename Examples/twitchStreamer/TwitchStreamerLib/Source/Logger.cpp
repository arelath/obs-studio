#include <Windows.h>
#include <stdio.h>
#include <debugapi.h>
#include <fstream>
#include <util/base.h>
#include <chrono>

#include "Logger.h"

using namespace std;

// Taken from Win-test example and obs-app
static void do_log(int log_level, const char *msg, va_list args, void *param)
{
	Logger* logger = static_cast<Logger*>(param);

	char bla[4096];
	vsnprintf(bla, 4095, msg, args);

	OutputDebugStringA(bla);
	OutputDebugStringA("\n");

	if (log_level <= LOG_ERROR && IsDebuggerPresent())
		__debugbreak();

	logger->LogStringChunk(bla, log_level);
}

string Logger::GetCurrentTimeString()
{
	using namespace std::chrono;

	struct tm tstruct;
	char buf[80];

	auto tp = system_clock::now();
	auto now = system_clock::to_time_t(tp);
	tstruct = *localtime(&now);

	size_t written = strftime(buf, sizeof(buf), "%X", &tstruct);
	if (ratio_less<system_clock::period, seconds::period>::value &&
	    written && (sizeof(buf) - written) > 5) {
		auto tp_secs = time_point_cast<seconds>(tp);
		auto millis = duration_cast<milliseconds>(tp - tp_secs).count();

		snprintf(buf + written, sizeof(buf) - written, ".%03u",
			 static_cast<unsigned>(millis));
	}

	return string(buf);
}

void Logger::LogString(const string &timeString, const string &str, int logLevel)
{
	string msg;
	msg += timeString;
	msg += str;

	logFile << msg << endl;
}

void Logger::LogStringChunk(char *str, int log_level)
{
	char *nextLine = str;
	std::string timeString = GetCurrentTimeString();
	timeString += ": ";

	while (*nextLine) {
		char *nextLine = strchr(str, '\n');
		if (!nextLine)
			break;

		if (nextLine != str && nextLine[-1] == '\r') {
			nextLine[-1] = 0;
		} else {
			nextLine[0] = 0;
		}

		LogString(timeString, str, log_level);
		nextLine++;
		str = nextLine;
	}

	LogString(timeString, str, log_level);
}

Logger::Logger(const std::string & filename)
{
	logFile.open(filename.c_str()); 

	if (logFile.is_open()) {
		base_set_log_handler(do_log, this);
	} else {
		blog(LOG_ERROR, "Failed to open log file");
	}
}
