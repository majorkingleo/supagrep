#ifndef SEARCH_H
#define SEARCH_H

#include <fx.h>

#include <list>
#include <vector>
#include <string>

class Search : public FXThread
{
 public:
  template<class T> class MTAccess
	{
	private:
	  FXMutex mt;
	  T value;
	  bool ch;

	public:
	  MTAccess( const T &value_ )
		: value( value ), ch( false )
		{}
		
	  MTAccess(): ch(false) {}

	  void set( const T &value_ )
	  { 
		mt.lock(); 
		value = value_;
		ch = true;
		mt.unlock();
	  }

	  T get() 
	  {
		mt.lock();
		T t = value;
		mt.unlock();
		return t;
	  }

	  void clear()
	  {
		mt.lock();
		ch = false;
		mt.unlock();
	  }

	  bool changed()
	  {
		bool ret;

		mt.lock();
		ret = ch;
		mt.unlock();
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
