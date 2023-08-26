/**
 * class for printing debugging messages async to stdout
 * @author Copyright (c) 2023 Martin Oberzalek
 */
#pragma once
#include <OutDebug.h>
#include <ColoredOutput.h>
#include <FastDelivery.h>
#include <variant>
#include <mutex>

namespace AsyncOut {

struct Data
{
	const char *file;
	unsigned line;
	const char *function;
	std::variant<std::string,std::wstring> message;
	Tools::ColoredOutput::Color color;
	std::wstring prefix;
};

class Logger : public Tools::FastDelivery::PublisherNode<Data>, public Tools::OutDebug
{
	std::list<value_type> messages;
	std::mutex m_messages;
	std::recursive_mutex m_worktodo;
public:

	// called async from Debug publisher.
	void deliver( const value_type & msg );

	void log();

	// wait for data
	void wait();

protected:
	std::list<value_type> popAll();
};

class Debug : public Tools::OutDebug, public Tools::FastDelivery::Publisher<Data,Logger>
{
public:
	Debug( Tools::ColoredOutput::Color color = Tools::ColoredOutput::BRIGHT_YELLOW );

	void add( const char *file, unsigned line, const char *function, const std::string & s ) override;
	void add( const char *file, unsigned line, const char *function, const std::wstring & s ) override;
};

} // namespace AsyncOut

