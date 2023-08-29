/**
 * class for search text data in files paralell
 * @author Copyright (c) 2023 Martin Oberzalek
 */
#include "SearchWorker.h"
#include <thread>
#include <CpputilsDebug.h>
#include <format.h>
#include <chrono>

using namespace std::chrono_literals;
using namespace Tools;

SearchWorker::SearchWorker( SearchWorkerMain & main_ )
: main( main_ )
{

}

void SearchWorker::run()
{
	for( std::optional<std::shared_ptr<SearchWorkerMain::Data>> o_data = main.pop();
	     main.should_i_wait_for_additional_data() || o_data;
	     o_data = main.pop() ) {

		if( o_data ) {
			read_file.read_file( (*o_data)->file.string(), (*o_data)->data );
			main.found_result(*o_data);
		} else {
			std::this_thread::sleep_for(100ms);
		}
	}
}

SearchWorkerMain::~SearchWorkerMain()
{
	stop();
}

void SearchWorkerMain::stop()
{
	finished_adding_data();
	for( auto thread : workers ) {
		thread->join();
		delete thread;
	}

	workers.clear();
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
		auto thread = new std::thread([]( auto main ) {
			SearchWorker sw(*main);
			sw.run();
		}, this);

		workers.push_back( thread );

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
