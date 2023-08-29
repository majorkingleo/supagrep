/**
 * class for search text data in files paralell
 * @author Copyright (c) 2023 Martin Oberzalek
 */
#pragma once

#include <string>
#include <mutex>
#include <filesystem>
#include <list>
#include <optional>
#include <read_file.h>
#include <memory>

class SearchWorkerMain;

class SearchWorker
{
	SearchWorkerMain & main;
	ReadFile read_file;
public:
	SearchWorker( SearchWorkerMain & main );

	void run();
};

class SearchWorkerMain
{
public:
	struct Data
	{
		std::filesystem::path file;
		std::wstring search_term;
		std::wstring data;
	};

private:
	std::mutex m_queue;
	std::mutex m_finished_queue;
	std::list<std::shared_ptr<Data>> queue;
	std::list<std::shared_ptr<Data>> finished_queue;
	unsigned input_queue_counter = 0;
	unsigned finished_queue_counter = 0;

public:

	void add( std::shared_ptr<Data> data );
	std::optional<std::shared_ptr<Data>> pop();
	void found_result( std::shared_ptr<Data> data );
	void run_workers();

	std::optional<std::shared_ptr<Data>> pop_result();

	/**
	 * returns true if all data put in queue where preoceeded
	 * and the result queue is empty
	 */
	bool finished() {
		std::lock_guard<std::mutex> mf(m_finished_queue);
		std::lock_guard<std::mutex> m(m_queue);

		if( input_queue_counter == finished_queue_counter ) {
			if( finished_queue.empty() ) {
				return true;
			}
		}

		return false;
	}

	unsigned getFinishedCount() {
		std::lock_guard<std::mutex> mf(m_finished_queue);
		return finished_queue_counter;
	}
};

