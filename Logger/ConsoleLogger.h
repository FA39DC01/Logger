#pragma once
#include <string>
#include <iostream>
#include <mutex>

#include "ILogHandler.h" 

class ConsoleLogger : public ILogHandler
{
public:
	ConsoleLogger() { id_ = "", is_enabled_ = true; level_ = Logger_Level::ALL; }

	// Inherited via ILogHandler
	virtual void enable() override
	{
		is_enabled_ = true;
	}

	virtual void disable() override
	{
		is_enabled_ = false;
	}

	virtual bool is_enabled() const override
	{
		return is_enabled_;
	}

	virtual void write(const char* data) override
	{
		printf(data);
	}

	virtual Logger_Type get_type() const override
	{
		return Logger_Type::Console;
	}
	virtual std::string get_id() const override
	{
		return id_;
	}
	virtual Logger_Level get_level() const override
	{
		return level_;
	}

private:
	bool is_enabled_;
	std::string id_;
	Logger_Level level_;
};

