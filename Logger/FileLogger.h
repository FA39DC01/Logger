#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include "ILog_meta_data.h"
#include "..//Communication/Messages/SetLogProperties.h"
class FileLogger : public ILogHandler
{
public:
	FileLogger(std::string id, Logger_Level level, std::wstring file_path)
	{
		file_path_ = file_path;
		id_ = id;
		enable_ = true;
		level_ = level;
	}
	FileLogger();
	// Inherited via ILogHandler
	virtual Logger_Type get_type() const override;
	virtual void write(const char* data) override;
	virtual void enable() override; 
	virtual void disable() override;  
	virtual std::string get_id() const override;
	virtual Logger_Level get_level() const override;
	virtual bool is_enabled() const override;
	virtual void set_properties(std::shared_ptr<ILogProperties> properties) override;

private:

	std::mutex f_mtx;
	std::wstring file_path_;
	bool enable_;
	std::string id_;
	Logger_Level level_; 

};