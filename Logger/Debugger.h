#pragma once 

#include <algorithm>
#include <ctime>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <thread>
#include <vector> 
#include "ConsoleLogger.h"

#define UDebugM(LOGGERID, X,M,Y,Z)  Log(LOGGERID, 0x10, X,M,Y,Z )
#define UDebug1(LOGGERID, X)  UDebugM(LOGGERID, format_str(X), __FILE__, __func__, __LINE__) 
#define UDebug2(LOGGERID, X,M)  UDebugM(LOGGERID, format_str(X,M), __FILE__, __func__, __LINE__) 
#define UDebug3(LOGGERID, A,B,C)  UDebugM(LOGGERID, format_str(A,B,C), __FILE__, __func__, __LINE__)  
#define UINFOM(LOGGERID, X,M,Y,Z)  Log(LOGGERID, 0x08,X,M,Y,Z ) 
#define UINFO1(LOGGERID, X)  UINFOM(LOGGERID, format_str(X), __FILE__, __func__, __LINE__) 
#define UINFO2(LOGGERID, X,M)  UINFOM(LOGGERID, format_str(X,M), __FILE__, __func__, __LINE__) 
#define UINFO3(LOGGERID, A,B,C)  UINFOM(LOGGERID, format_str(A,B,C), __FILE__, __func__, __LINE__)  
#define UTrace(LOGGERID) Log(LOGGERID, 0x20, "", __FILE__, __func__, __LINE__) 
#define UTrace1(LOGGERID, X) Log(LOGGERID, 0x20, X, __FILE__, __func__, __LINE__) 
#define UTrace2(LOGGERID, X,M) Log(LOGGERID, 0x20, format_str(X,M), __FILE__, __func__, __LINE__)  
#define UCritical(LOGGERID, X) Log(LOGGERID, 0x01, X, __FILE__, __func__, __LINE__) 
#define UFault(LOGGERID, X) Log(LOGGERID, 0x02, X, __FILE__, __func__, __LINE__)
#define UFault2(LOGGERID, X,M) Log(LOGGERID, 0x02, format_str(X,M), __FILE__, __func__, __LINE__)

//Logger without unique id

#define DebugM(X,M,Y,Z)  Log(0x10, X,M,Y,Z )
#define Debug1(X)  DebugM(X, __FILE__, __func__, __LINE__) 
#define Debug2(X,M)  DebugM(format_str(X,M), __FILE__, __func__, __LINE__) 
#define Debug3(A,B,C)  DebugM(format_str(A,B,C), __FILE__, __func__, __LINE__)  
#define INFOM(L,M,Y,Z)  Log(0x08,L,M,Y,Z) 
#define INFO1(L)  INFOM(format_str(L), __FILE__, __func__, __LINE__) 
#define INFO2(L,M)  INFOM(format_str(L,M), __FILE__, __func__, __LINE__) 
#define INFO3(L,B,C)  INFOM(format_str(L,B,C), __FILE__, __func__, __LINE__)  
#define Trace Log(0x20, "", __FILE__, __func__, __LINE__) 
#define Trace1(X) Log(0x20, X, __FILE__, __func__, __LINE__) 
#define CriticalM(X,M,Y,Z) Log(0x01, X,M,Y,Z) 
#define Critical(X) CriticalM(X, __FILE__, __func__, __LINE__) 
#define Critical2(X,M)  CriticalM(format_str(X,M), __FILE__, __func__, __LINE__) 
#define Fault(X) Log(0x02, X, __FILE__, __func__, __LINE__)

class ConsoleLogger;


inline Logger_Type string_to_log_type(std::string value)
{
	if (value == "net" || value == "network")
	{
		return Logger_Type::Network;
	}
	else if (value == "file")
	{
		return Logger_Type::File;
	}
	else if (value == "console")
	{
		return Logger_Type::Console;
	}
	return Logger_Type::Invalid;
}
inline  Logger_Level string_to_log_level(std::string value)
{
	if (value == "off")
	{
		return Logger_Level::OFF;
	}
	else if (value == "critical")
	{
		return Logger_Level::Critical;
	}
	else if (value == "fault")
	{
		return Logger_Level::Fault;
	}
	else if (value == "warning")
	{
		return Logger_Level::Warning;
	}
	else if (value == "information")
	{
		return Logger_Level::Information;
	}
	else if (value == "debuging")
	{
		return Logger_Level::Debuging;
	}
	else if (value == "tracing")
	{
		return Logger_Level::Tracing;
	}
	else if (value == "all")
	{
		return Logger_Level::ALL;
	}
	return Logger_Level::Invalid;
}


typedef std::map<std::string, std::vector<std::shared_ptr<ILogHandler>>> Logger_Map;

class Debugger
{
public:

	static void log(std::string logger_id, const char* log_content, Logger_Level level)
	{
		std::map<std::string, std::vector< std::shared_ptr<ILogHandler>>>::iterator it = log_handlers_.find(logger_id);
		bool found = false;
		if (it != log_handlers_.end())
		{
			for (std::shared_ptr<ILogHandler> log : it->second)
				if (log->get_level() >= level)
				{
					found = true;
					log->write(log_content);
				}
		}
		else
			log(log_content, level);
	}
	static void log(const char* log_content, Logger_Level level)
	{
		std::map<std::string, std::vector<std::shared_ptr<ILogHandler>>>::iterator it = log_handlers_.find("general");
		if (it != log_handlers_.end())
		{
			for (auto&& log : it->second)
				if (log->get_level() >= level)
					log->write(log_content);
		}
		else
		{
			std::shared_ptr<ConsoleLogger> c_log = std::make_shared<ConsoleLogger>();
			std::vector< std::shared_ptr<ILogHandler>> vec_log;
			vec_log.push_back(c_log);
			log_handlers_.insert(std::pair<std::string, std::vector<std::shared_ptr<ILogHandler>>>("general", vec_log));
			if (c_log->get_level() >= level)
				c_log->write(log_content);
		}
	}
	static void set_general_log(std::shared_ptr<ILogHandler> log_handler)
	{
		std::vector< std::shared_ptr<ILogHandler>> vec_log;
		vec_log.push_back(log_handler);
			log_handlers_.insert(std::pair<std::string, std::vector<std::shared_ptr<ILogHandler>>>("general", vec_log));
	}
	static void add_log_handler(std::string unique_id, std::shared_ptr<ILogHandler> log_handler)
	{
		std::map<std::string, std::vector<std::shared_ptr<ILogHandler>>>::iterator it = log_handlers_.find(unique_id);
		if (it != log_handlers_.end())
		{
			it->second.push_back(log_handler);
		}
		else
		{
			std::vector< std::shared_ptr<ILogHandler>> vec_log;
			vec_log.push_back(std::move(log_handler));
			log_handlers_.insert(std::pair<std::string, std::vector< std::shared_ptr<ILogHandler>>>(unique_id, vec_log));
		}
	}
	static std::shared_ptr<ILogHandler> get_log_properties(std::string unique_id, std::string log_id)
	{
		std::map<std::string, std::vector<std::shared_ptr<ILogHandler>>>::iterator it = log_handlers_.find(unique_id);
		if (it != log_handlers_.end())
		{
			bool found = false;
			for (auto&& log : it->second)
				if (log->get_id() == log_id)
					return log;
		}
		return nullptr;
	}
	static bool is_logger_exist(std::string logger_id)
	{
		std::map<std::string, std::vector<std::shared_ptr<ILogHandler>>>::iterator it = log_handlers_.find(logger_id);
		if (it != log_handlers_.end())
			return true;
		return false;
	}


private:
	Debugger()
	{
	}
	static Logger_Map log_handlers_;
	static bool init_;
	//static std::unique_ptr<ILogHandler> gerenal_logger_;
}; 

static std::string get_now()
{
	time_t now = time(0);

	// convert now to string form
	char* dt = ctime(&now);

	std::string str_time(dt, strlen(dt));
	return dt;
}

static std::string get_level(Logger_Level level)
{
	if (level == Logger_Level::Fault)
		return "Fault";
	else if (level == Logger_Level::Information)
		return "Info";
	else if (level == Logger_Level::Critical)
		return "Critical";
	else if (level == Logger_Level::Warning)
		return "Warning";
	else if (level == Logger_Level::Debuging)
		return "Debugging";
	else if (level == Logger_Level::Tracing)
		return "Trace";
	return "Unknown";
}
//static std::ostream& operator<<(std::ostream& os, int x, int y, int z)
//{
//	return os << "{ x=" << x << ", y=" << y << ", z=" << z << " }";
//}
static std::string get_str_thread_id()
{
	size_t thread_id = std::hash<std::thread::id>{}(std::this_thread::get_id());
	std::string str_id = std::to_string(thread_id);
	return str_id;
}

static std::string remove(std::string str, char value)
{
	std::vector<char> result;
	std::for_each(str.begin(), str.end(), [&](const char& input)
		{
			if (input != value)
				result.push_back(input);
		}
	);
	std::string str_result(result.begin(), result.end());
	return str_result;
}


// base case
static std::string format_str(const char* s)
{
	std::string output = "";
	while (*s)
	{
		if (*s == '%')
		{
			if (*(s + 1) != '%')
				++s;
			else
				return s;//throw std::runtime_error("invalid format string: missing arguments");
		}
		output.append(1, s[0]);
		s++;
	}
	return output;
}

// recursive
template<typename T, typename... Args>
static std::string format_str(const char* s, T value, Args... args)
{
	std::stringstream st;
	while (*s)
	{
		if (*s == '%')
		{
			if (*(s + 1) != '%')
			{
				// pretend to parse the format: only works on 2-character format strings ( %d, %f, etc ); fails with %5.4f
				s += 2;
				// print the value
				if (value != 0)
					st << value;
				else
					st << ' ';
				// called even when *s is 0 but does nothing in that case (and ignores extra arguments)

				return format_str(st.str().c_str(), args...);
			}

			++s;
		}

		st << *s++;
	}
	return st.str();
}


// base case
static std::wstring format_str(const wchar_t* s)
{
	std::wstring output = L"";
	int index = 0;
	while (*s)
	{
		if (*s == L'%')
		{
			if (*(s + 1) != L'%')
				++s;
			else
				return s;//throw std::runtime_error("invalid format string: missing arguments");
		}
		output.append(1, s[index]);
		s++;
	}
	return output;
}

// recursive
template<typename T, typename... Args>
static std::wstring format_str(const wchar_t* s, T value, Args... args)
{
	std::stringstream st;
	while (*s)
	{
		if (*s == '%')
		{
			if (*(s + 1) != '%')
			{
				// pretend to parse the format: only works on 2-character format strings ( %d, %f, etc ); fails with %5.4f
				s += 2;
				// print the value
				if (value != 0)
					st << value;
				else
					st << ' ';

				st << s;
				return format_str(st.str().c_str(), args...);
			}

			++s;
		}

		st << *s++;
	}
	return s;
}
static inline std::string create_log_format(int level, std::string user_data, const char* file, const char* func, unsigned int line)
{
	/*output
[time] [level] [file:] [func:] [line:] [threadid:] [output]

*/
	std::string full_path = std::string(file);
	std::string base_filename = full_path.substr(full_path.find_last_of("/\\") + 1);
	std::string str_time = "[" + get_now() + "]";
	std::string str_level = "[" + get_level(static_cast<Logger_Level>(level)) + "]";
	std::string str_file = "[" + base_filename + "]";
	std::string str_func = "[" + std::string(func) + "]";
	std::string str_line = "[" + std::to_string(line) + "]";
	std::string str_thread_id = "[" + get_str_thread_id() + "]";
	if (user_data.size() > 0)
		user_data = "[" + user_data + "]";
	std::string str_output = str_time + " " + str_level + " " + " " + str_file + " " + str_func + " " + str_line + " " + str_thread_id + " " + user_data;

	str_output = remove(str_output, '\r');
	str_output = remove(str_output, '\n');
	return  str_output;
}

static inline void Log(int level, std::string log_content, const char* file, const char* func, unsigned int line)
{
	std::string log_format = create_log_format(level, log_content, file, func, line);
	Debugger::log(log_format.c_str(), static_cast<Logger_Level>(level));
}
static inline void Log(std::string logger_id, int level, std::string log_content, const char* file, const char* func, unsigned int line)
{
	std::string log_format = create_log_format(level, log_content, file, func, line);
	Debugger::log(logger_id, log_format.c_str(), static_cast<Logger_Level>(level));
}

bool Debugger::init_ = false;
Logger_Map Debugger::log_handlers_ = Logger_Map();