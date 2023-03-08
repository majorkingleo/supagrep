#ifndef SEARCH_H
#define SEARCH_H

#include <fx.h>

#include <list>
#include <vector>
#include <string>
#include <mutex>
#include <chrono>
#include <filesystem>
#include <regex>

class Search
{
 public:
  template<class T> class MTAccess
	{
	private:
	  std::mutex mt;
	  using slock = std::lock_guard<std::mutex>;
	  T value;
	  bool ch;

	public:
	  MTAccess( const T &value_ )
		: value( value ), ch( false )
		{}
		
	  MTAccess(): ch(false) {}

	  void set( const T &value_ )
	  { 
		slock ml(mt);
		value = value_;
		ch = true;
	  }

	  T get() 
	  {
		slock ml(mt);
		T t = value;
		return t;
	  }

	  T getAndClear()
	  {
		slock ml(mt);
		T t = value;
		ch = false;
		return t;
	  }

	  void clear()
	  {
		slock ml(mt);
		ch = false;
	  }

	  bool changed()
	  {
		bool ret;

		slock ml(mt);
		ret = ch;

		return ret;
	  }

	  T & access()
		{
		  mt.lock();
		  ch = true;
		  return value;
		}

	  void free()
	  {
		mt.unlock();
	  }

	  void clear_and_free()
	  {
		ch = false;
		mt.unlock();
	  }
	};

  struct Result
  {
	std::filesystem::path file;
	long line;
	long pos;

	Result( const std::filesystem::path & file_, long line_, long pos_ )
	  : file( file_ ), line( line_ ), pos( pos_ )
	{}
  };

  struct Config
  {
	bool regex;
	bool icase;
	std::wstring pattern;
	std::wstring path;
	std::wstring search;
	MTAccess<bool> *mt_running;
	MTAccess<int> *mt_status;
	MTAccess<int> *mt_status_max;
	MTAccess<bool> *mt_stop;
	MTAccess<std::list<Result> > *mt_result;
	MTAccess<std::chrono::milliseconds> *mt_runtime;

	Config()
	  : regex( false ),
		icase ( false ),
		mt_running( 0 ),
		mt_status( 0 ),
		mt_status_max( 0 ),
		mt_stop( 0 ),
		mt_result( 0 ),
		mt_runtime( 0 )
	{}
  };

 protected:
  Config & config;
  typedef std::list<std::filesystem::path> file_list;
  file_list files;
  std::vector<std::string> pattern_list;
  std::chrono::time_point<std::chrono::system_clock> start_time;
  std::wstring lsearch;

 public:
  Search( Config & config );

  void run();

 protected:
  bool find_files( const std::filesystem::path & path );
  bool match_file_type( const std::filesystem::path & file );
  void do_search( const std::filesystem::path & file );
  void do_simple_search( const std::wstring & s,
						 const std::wstring & search_term,
						 const std::filesystem::path & file );

  std::vector<std::wregex> build_pattern_list( const std::wstring & pattern );
};

#endif
