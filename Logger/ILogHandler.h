#pragma once
#include <string>
enum class Logger_Level
{
	Invalid = -1,
	/*
	OFF=
		This log level does not log anything. This OFF level is used to turn off logging and is the greatest possible rank. With this log level, nothing gets logged at all.
	*/
	OFF = 0x00,
	/*
	Critical=
	Critical means that the application is about to stop a serious problem or corruption from happening. The Critical level of logging shows that the application’s situation is catastrophic, such that an important function is not working. For example, you can use Critical log level if the application is unable to connect to the data store, or cannot start cmd.exe or powershell.exe
	*/
	Critical = 0x01,
	/*
	Fault = Unlike the Critical logging level, error does not mean your application is aborting. Instead, there is just an inability to access a service or a file. This Fault shows a failure of something important in your application. This log level is used when a severe issue is stopping functions within the application from operating efficiently. Most of the time, the application will continue to run, but eventually, it will need to be addressed.
	*/
	Fault = 0x02,
	/*
	Warning = The WARN log level is used when you have detected an unexpected application problem. This means you are not quite sure whether the problem will recur or remain. You may not notice any harm to your application at this point. This issue is usually a situation that stops specific processes from running. Yet it does not mean that the application has been harmed. In fact, the code should continue to work as usual. You should eventually check these warnings just in case the problem reoccurs.
	*/
	Warning = 0x04,
	/*
	Information = Information messages are like the normal behavior of applications. They state what happened. For example, if a particular service stopped or started or you added something to the database. These entries are nothing to worry about during usual operations. The information logged using the Information log is usually informative, and it does not necessarily require you to follow up on it.
	*/
	Information = 0x08,
	/*
	Debuging =	With Debuging, you are giving diagnostic information in a detailed manner. It is verbose and has more information than you would need when using the application. Debuging logging level is used to fetch information needed to diagnose, troubleshoot, or test an application. This ensures a smooth running application.
	*/
	Debuging = 0x10,
	/*
	Tracing = The Tracing log level captures all the details about the behavior of the application. It is mostly diagnostic and is more granular and finer than Debuging log level. This log level is used in situations where you need to see what happened in your application or what happened in the third-party libraries used. You can use the Tracing log level to query parameters in the code or interpret the algorithm’s steps.
	*/
	Tracing = 0x20,
	/*
	ALL = This log level logs any logging levels that are defined. It logs everything and includes custom logging levels as well. It is the combination of all other logging levels.
	*/
	ALL = 0x030
};

enum class Logger_Type
{
	Invalid = 0x00,
	Console = 0x01,
	Network = 0x02,
	File = 0x03,
};

class ILogProperties
{
public:

	virtual Logger_Type get_type() const = 0;
	virtual std::string get_id() const = 0;
	virtual Logger_Level get_level() const = 0;
	virtual bool is_active() const = 0;
	virtual void load_properties(std::wstring payload) = 0;
	~ILogProperties() {}
};


class ILogHandler
{
public:
	virtual void enable() = 0;
	virtual void disable() = 0;
	virtual bool is_enabled() const = 0;
	virtual void write(const char* data) = 0;
	virtual Logger_Type get_type() const = 0;
	virtual std::string get_id() const = 0;
	virtual Logger_Level get_level() const = 0;
	virtual	~ILogHandler() {};
};
