#ifndef SEARCH_H
#define SEARCH_H

#include <fx.h>

#include <list>
#include <vector>
#include <string>
#include <mutex>

class Search : public FXThread
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
	FXString file;
	long line;
	long pos;

	Result( const FXString & file_, long line_, long pos_ )
	  : file( file_ ), line( line_ ), pos( pos_ )
	{}
  };

  struct Config
  {
	bool regex;
	bool icase;
	FXString pattern;
	FXString path;
	FXString search;
	MTAccess<bool> *mt_running;
	MTAccess<int> *mt_status;
	MTAccess<int> *mt_status_max;
	MTAccess<bool> *mt_stop;
	MTAccess<std::list<Result> > *mt_result;
	MTAccess<FXTime> *mt_runtime;

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
  typedef std::list<FXString> file_list;  
  file_list files;
  std::vector<std::string> pattern_list;
  FXTime start_time;
  FXString lsearch;
  FXString usearch;

 public:
  Search( Config & config );
  virtual ~Search();

  FXint run();

 protected:
  bool find_files( const FXString &path );
  bool match_file_type( const FXString &file );
  void do_search( const FXString & file );
  void do_simple_search( char *s, long length, const FXString & file );
  int find( char *s, long start, long lenght, long &linecount );
};

#endif
