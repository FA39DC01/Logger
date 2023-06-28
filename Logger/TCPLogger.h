#pragma once
 
#include "ILog_meta_data.h"
#include "Debugger.h"
#include "..//Communication//Messages/SetLogProperties.h"
#include "..//Peripheral/TCPClient.h" 

class TCPLogger : public ILogHandler
{
public:
	TCPLogger(std::string id, Logger_Level level, std::string ipaddress, unsigned int port);
	TCPLogger();
	// Inherited via ILogHandler
	virtual void enable() override;
	virtual void disable() override;
	virtual void write(const char* data) override;
	virtual Logger_Type get_type() const override;
	virtual std::string get_id() const override;
	virtual Logger_Level get_level() const override;
	// Inherited via ILogHandler
	virtual void set_properties(std::shared_ptr<ILogProperties> properties) override;
	virtual bool is_enabled() const override;

private:
	std::shared_ptr<ILogProperties> properties_;
	std::string id_;
	Logger_Level level_;
	std::string ipaddress_;
	unsigned int port_;
	bool enabled_;
	std::shared_ptr<Module::Peripheral::TCPClient> client_;
	 
};

