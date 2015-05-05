#include "Log.hpp"

#include <ctime>

#include <chrono>
#include <iostream>
#include <fstream>
#include <ostream>
#include <stdexcept>
#include <string>


namespace engine {


LogPriority LogPriority::critical(LOG_LEVEL_CRITICAL, "CRITICAL: ");
LogPriority LogPriority::error(LOG_LEVEL_ERROR, "ERROR: ");
LogPriority LogPriority::warn(LOG_LEVEL_WARN, "WARN: ");
LogPriority LogPriority::info(LOG_LEVEL_INFO, "INFO: ");
LogPriority LogPriority::debug(LOG_LEVEL_DEBUG, "DEBUG: ");
LogPriority LogPriority::verbose(LOG_LEVEL_VERBOSE, "VERBOSE: ");

std::ofstream Log::nullStream_(nullptr);


LogPriority::LogPriority(LogLevel level, const std::string identifier) :
	level_(level),
	identifier_(identifier)
{

}

Log::Log(const char *filename, LogLevel level) :
	file_(filename),
	logLevel_(level),
	logPriority_()
{
	if (!file_.is_open()) {
		std::string message("Could not open log file: ");
		message.append(filename);
		throw std::runtime_error(message);
	}

	nullStream_.setstate(std::ios_base::badbit);

	for (int i = 0; i < LOG_LEVEL_CNT; ++i)
		logPriority_[i] = i;

}

Log::~Log()
{
	file_.close();
}

void Log::setLogLevel(const LogLevel level)
{
	logLevel_ = level;
}


void Log::setLogPriority(const LogLevel priority, const LogLevel level)
{
	logPriority_[priority] = level;
}


void Log::currentTime()
{
	time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	*this << LogPriority::info << "Current time: " << ctime(&tt);
}


std::ostream & Log::operator << (const char *s)
{
	file_ << s;
	return file_;
}

std::ostream & Log::operator << (const std::string &s)
{
	file_ << s;
	return file_;
}


std::ostream & Log::operator<< (const LogPriority &p)
{
	if (logLevel_ >= logPriority_[p.level_]) {
		file_ << p.identifier_;
		return file_;
	}

	return nullStream_;
}




} // namespace engine
