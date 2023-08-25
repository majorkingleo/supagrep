/**
 * class for printing debugging messages async to stdout
 * @author Copyright (c) 2023 Martin Oberzalek
 */
#include "AsyncOutDebug.h"
#include <filesystem>

using namespace Tools;
using namespace AsyncOut;

AsyncOut::Debug::Debug( ColoredOutput::Color color )
: OutDebug(color)
{

}

void AsyncOut::Debug::add( const char *file, unsigned line, const char *function, const std::string & s )
{
	distribute( Data{file,line,function, s, color, prefix} );
}

void AsyncOut::Debug::add( const char *file, unsigned line, const char *function, const std::wstring & s )
{
	distribute( Data{file,line,function, s, color, prefix} );
}

void AsyncOut::Logger::deliver( const value_type & msg )
{
	std::lock_guard<std::mutex> ml(m_messages);
	messages.push_back( msg );
	m_worktodo.unlock();
}

void AsyncOut::Logger::wait()
{
	if( m_worktodo.try_lock() ) {
		// ok locked, now lock again
	}
	m_worktodo.lock();
}

std::list<AsyncOut::Logger::value_type> AsyncOut::Logger::popAll()
{
	std::lock_guard<std::mutex> ml(m_messages);
	std::list<value_type> ret( std::move_iterator(std::begin(messages)),
				               std::move_iterator(std::end(messages)) );
	messages.clear();
	return ret;
}

void AsyncOut::Logger::log()
{
	auto all_messages = popAll();

	for( const auto & m : all_messages ) {
		if( print_line_and_file_info ) {
			std::string file_name = std::filesystem::path(m.file).filename();
			std::cout << color_output( m.color, file_name );
			std::cout << ':' << m.line
					  << " ";
		}

		if( !prefix.empty() ) {
			 std::cout << color_output( m.color, DetectLocale::w2out(m.prefix) );
			 std::cout << " ";
		}

		std::string message;

		if( const auto str_ptr (std::get_if<std::string>(&m.message)); str_ptr) {
			message = *str_ptr;
		} else {
			message = DetectLocale::w2out(std::get<std::wstring>(m.message));
		}

		std::cout << message << '\n';
	}
}

