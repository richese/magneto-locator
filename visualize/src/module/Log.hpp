#ifndef _SDL2_ENGINE_MODULE_LOG_H_
#define _SDL2_ENGINE_MODULE_LOG_H_

#include <fstream>
#include <ostream>
#include <string>


namespace engine {


enum _LogLevels
{
	LOG_LEVEL_CRITICAL  = 0,
	LOG_LEVEL_ERROR     = 1,
	LOG_LEVEL_WARN      = 2,
	LOG_LEVEL_INFO      = 3,
	LOG_LEVEL_DEBUG     = 4,
	LOG_LEVEL_VERBOSE   = 5,

	LOG_LEVEL_CNT       = 6
};
typedef _LogLevels LogLevel;


class Log;


class LogPriority
{
	const LogLevel level_;
	const std::string identifier_;

public:
	static LogPriority critical;
	static LogPriority error;
	static LogPriority warn;
	static LogPriority info;
	static LogPriority debug;
	static LogPriority verbose;

	int getLevel() const { return level_; }

private:
	LogPriority(LogLevel level, const std::string identifier_);

	friend class Log;
};

typedef LogPriority Priority;


class Log
{
// static
	static std::ofstream nullStream_;

//
private:
	std::ofstream file_;
	int logLevel_;
	int logPriority_[LOG_LEVEL_CNT];

public:
	Log(const char* filename, LogLevel level = LOG_LEVEL_INFO);
	~Log();


	void setLogLevel(const LogLevel level);
	void setLogPriority(const LogLevel priority, const LogLevel level);

	void currentTime();
	std::ostream& operator << (const char *s);
	std::ostream& operator << (const std::string &s);
	std::ostream& operator << (const LogPriority &p);
};


} // namespace engine


#endif // _SDL2_ENGINE_MODULE_LOG_H_
