/**
 * class for search text data in files paralell
 * @author Copyright (c) 2023 Martin Oberzalek
 */
#include "SearchWorker.h"
#include <thread>
#include <CpputilsDebug.h>
#include <format.h>

using namespace Tools;

SearchWorker::SearchWorker( SearchWorkerMain & main_ )
: main( main_ )
{

}

void SearchWorker::run()
{
	for( std::optional<std::shared_ptr<SearchWorkerMain::Data>> o_data = main.pop();
	     o_data;
	     o_data = main.pop() ) {
		read_file.read_file( (*o_data)->file.string(), (*o_data)->data );
		main.found_result(*o_data);
	}
}


void SearchWorkerMain::add( std::shared_ptr<Data> data )
{
	std::lock_guard<std::mutex> m(m_queue);
	queue.push_back(data);
	input_queue_counter++;
}

std::optional<std::shared_ptr<SearchWorkerMain::Data>> SearchWorkerMain::pop()
{
	std::optional<std::shared_ptr<Data>> ret;

	std::lock_guard<std::mutex> m(m_queue);
	if( queue.empty() ) {
		return ret;
	}

	ret = queue.front();
	queue.pop_front();
	return ret;
}

void SearchWorkerMain::found_result( std::shared_ptr<Data> data )
{
	std::lock_guard<std::mutex> m(m_finished_queue);
	finished_queue.push_back(data);
	finished_queue_counter++;
}

void SearchWorkerMain::run_workers()
{
	for( unsigned i = 0; i < std::thread::hardware_concurrency(); i++ ) {
		std::thread([]( auto main ) {
			SearchWorker sw(*main);
			sw.run();
		}, this).detach();
		DEBUG( wformat( L"started worker thread %d", i+1 ) );
	}
}

std::optional<std::shared_ptr<SearchWorkerMain::Data>> SearchWorkerMain::pop_result()
{
	std::optional<std::shared_ptr<Data>> ret;

	std::lock_guard<std::mutex> m(m_finished_queue);
	if( finished_queue.empty() ) {
		return ret;
	}

	ret = finished_queue.front();
	finished_queue.pop_front();
	return ret;
}
