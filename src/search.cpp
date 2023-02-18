#include "search.h"
#include "string_utils.h"
#include <FXMemMap.h>
#include <format.h>
#include <DetectLocale.h>
#include <debug.h>
#include <utf8_util.h>

using namespace Tools;

Search::Search( Config & config_ )
  : config( config_ )
{
  config.mt_status->set(0);
}

Search::~Search()
{

}

FXint Search::run()
{
  config.mt_running->set(true);
  start_time = FXThread::time();
  files.clear();
  pattern_list.clear();

  if( find_files( config.path ) )
	{
	  if( config.icase ) {
		lsearch = config.search;
		usearch = config.search;

		lsearch.lower();
		usearch.upper();
	  }

	  config.mt_status_max->set(files.size());
	  int count = 0;
	  for( file_list::iterator it = files.begin(); it != files.end() && !config.mt_stop->get(); it++ )
		{
		  // DEBUG( wformat( L"Searching in file: '%s'", DetectLocale::in2w(it->text()) ) );
		  do_search( *it );

		  count++;
		  config.mt_status->set(count);
		}
	}

  config.mt_runtime->set(FXThread::time()-start_time);
  config.mt_running->set(false);

#if FOX_MAJOR >= 1 && FOX_MINOR >= 7
  delete this;
#endif
  return 1;
}

bool Search::find_files( const FXString &path )
{
  if( config.mt_stop->get() )
	return false;

  // printf( "%s\n", path.text() );
  // DEBUG( wformat(L"path: '%s'", DetectLocale::in2w( path.text() ) ) );

  FXString *list = NULL;

  if( !FXDir::listFiles( list, path ) )
    {
	  return false;
    }

  if( FXStat::isLink( path ) &&  FXStat::isDirectory( path ) ) {
	  return false;
  }
  
  FXString *list_back = list;
  
  for( ; *list != FXString::null ; list++ )
    {
	  if( FXStat::isDirectory( format( "%s%s%s", path.text(), PATHSEPSTRING, list->text()
												  ).c_str() ) )
        {
		  if( *list == ".." || *list == "." )
			continue;

		  
		  find_files( format( "%s%s%s", path.text(), PATHSEPSTRING, list->text() ).c_str() );
		  continue;
        }
	  
	  if( match_file_type( *list ) ) {
	      files.push_back( format( "%s%s%s", path.text(), PATHSEPSTRING, list->text() ).c_str() );
      }
    }

    // cleanup
    delete [] list_back;

    return true;
}

bool Search::match_file_type( const FXString & file )
{
#if FOX_MAJOR >= 1 && FOX_MINOR >= 7
  bool ret = FXPath::match( file, config.pattern, FXPath::NoEscape|FXPath::PathName|FXPath::CaseFold );
#else
  bool ret = FXPath::match( config.pattern, file, FILEMATCH_NOESCAPE|FILEMATCH_FILE_NAME|FILEMATCH_CASEFOLD );
#endif
  if( !ret ) {
	  DEBUG( wformat(L"file: '%s' didn't matched pattern '%s'",
					 DetectLocale::in2w( file.text() ),
					 DetectLocale::in2w( config.pattern.text() ) ) );
  }

  return ret;
}

void Search::do_search( const FXString & file )
{
  FXMemMap map;

  char *base = nullptr;

#if FOX_MAJOR >= 1 && FOX_MINOR >= 7
  base = static_cast<char*>(map.openMap( file ));
#else
  base = static_cast<char*>(map.mapFile( file ));
#endif

  if( base == NULL ) {
	  DEBUG( wformat( L"cannot open file: '%s'", Utf8Util::utf8toWString(file.text()) ) );
	  return;
  }

  do_simple_search( base, map.length(), file );
}

void Search::do_simple_search( char *s, long length, const FXString & file )
{
  long pstart = 0;
  long linecount = 1;
  long last_line = -1;

  do
	{
	  pstart = find( s, pstart, length, linecount );
	  
	  if( pstart > 0 ) 
		{
		  if( last_line != linecount ) {
			/* jede Zeile nur einmal */
			// printf( "%s:%ld\n", file.text(), linecount );
			config.mt_result->access().push_back(Result( file, linecount, pstart ));
			config.mt_result->free();
		  }

		  last_line = linecount;

		  pstart += config.search.length();
		}

	} while( pstart > 0 && pstart < length );
}

int Search::find( char *s, long pstart, long length, long &linecount )
{
  if( pstart < 0 )
	return -1;

  if( pstart >= length )
	return -1;

  if( config.search.empty() )
	return -1;

  long pos = 0;

  do {
	if( s[pstart] == '\n' )
	  linecount++;

	if( config.icase ) {
	  if( s[pstart] != lsearch[pos] && s[pstart] != usearch[pos] )
		{
		  pstart++;
		  pos=0;
		} else {
		pstart++;
		pos++;
	  }
	} else {
	  if( s[pstart] != config.search[pos] )
		{
		  pstart++;
		  pos=0;
		} else {
		pstart++;
		pos++;
	  }
	} /* else */	
  } while( pstart < length && pos < config.search.length() );

  if( pstart < length && pos == config.search.length() )
	{
	  return pstart - config.search.length();
	}

  return -1;
}

