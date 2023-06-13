#include "search.h"
#include "string_utils.h"
#include <format.h>
#include <DetectLocale.h>
#include <debug.h>
#include <utf8_util.h>
#include <filesystem>
#include "read_file.h"
#include <string_utils.h>
#include <unistd.h>

using namespace Tools;
using namespace std::chrono_literals;

Search::Search( std::shared_ptr<Config> config_ )
  : config( config_ )
{
  DEBUG( wformat( L"Search with config: %s", config ) );
  config->mt_status->set(0);
}

void Search::run()
{
  config->mt_running->set(true);
  start_time = std::chrono::system_clock::now();
  files.clear();
  pattern_list.clear();
  regex_pattern_list.clear();

  if( find_files( config->path ) )
	{
	  if( config->icase ) {
		lsearch = tolower(config->search);
	  }

	  config->mt_status_max->set(files.size());

	  auto duration_file_search = std::chrono::system_clock::now().time_since_epoch()
									  - start_time.time_since_epoch();
	  double seconds = duration_file_search / 1.0s;

	  DEBUG( wformat( L"Total files '%d' duration: %3.3lf sec",
			  	  	    files.size(),
						seconds ) );

	  int count = 0;
	  for( file_list::iterator it = files.begin(); it != files.end() && !config->mt_stop->get(); it++ ) {

		  count++;
		  DEBUG( wformat( L"%d Searching in file: '%s'", count, it->filename().wstring() ) );

		  do_search( *it );

		  config->mt_status->set(count);
		}
	}

  auto duration = std::chrono::system_clock::now().time_since_epoch() - start_time.time_since_epoch();
  config->mt_runtime->set(std::chrono::duration_cast<std::chrono::milliseconds>(duration));
  config->mt_running->set(false);

  DEBUG( L"search thread done" );
}

bool Search::find_files( const std::filesystem::path & path )
{
  namespace fs = std::filesystem;

  if( config->mt_stop->get() ) {
	return false;
  }

  // printf( "%s\n", path.text() );
  // DEBUG( wformat(L"path: '%s'", DetectLocale::in2w( path.text() ) ) );

  if( !std::filesystem::is_directory(path) ) {
	  return false;
  }
  
  // avoid recursion
  if( std::filesystem::is_symlink(path) ) {
	  return false;
  }
#if 0
#ifdef _WIN32
  if( _waccess(path.c_str(), 04 ) != 0 ) {
	  return false;
  }
#else
  if( access(path.c_str(), R_OK, W_OK | X_OK ) != 0 ) {
	  return false;
  }
#endif
#endif

#if 1
  try {
	  for( const auto & entry : fs::directory_iterator(path) ) {
		  if( fs::is_directory(entry.status()) ) {
			  find_files( fs::path(entry) );
		  }

		  if( match_file_type( entry.path().filename() ) ) {
			  files.push_back( entry.path() );
		  }
	  }
  } catch( const std::filesystem::filesystem_error & error ) {
	  // It seems that we cannot fix this by using _waccess() simple opening these
	  // dead symlinks fails

	  /*  Error accessing file or directory: "C:/Users/Martin Oberzalek\\SendTo".
	   *   Error: filesystem error: directory iterator cannot open directory:
	   *   Invalid argument [C:/Users/Martin Oberzalek\SendTo]
	   */
	  DEBUG( wformat( L"Error accessing file or directory: %s. Error: %s", path, error.what()) );
	  return false;
  }
#else
  try {
	  for (const fs::directory_entry & entry
			  : fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied ) ) {
		  if( config->mt_stop->get() ) {
			  break;
		  }

		  if( match_file_type( entry.path().filename() ) ) {
			  files.push_back( entry.path() );
		  }
	  }
  } catch( const std::filesystem::filesystem_error & error ) {
	  // It seems that we cannot fix this by using _waccess() simple opening these
	  // dead symlinks fails

	  /*  Error accessing file or directory: "C:/Users/Martin Oberzalek\\SendTo".
	   *   Error: filesystem error: directory iterator cannot open directory:
	   *   Invalid argument [C:/Users/Martin Oberzalek\SendTo]
	   */
	  DEBUG( wformat( L"Error accessing file or directory: %s. Error: %s", path, error.what()) );
	  return false;
  }
#endif
  return true;
}

std::vector<std::wregex> Search::build_pattern_list( const std::wstring & pattern )
{
	std::vector<std::wstring> sl = split_and_strip_simple( pattern, L",;" );
	std::vector<std::wregex> ret;

	for( auto & str : sl ) {
		if( str.empty() ) {
			continue;
		}

		str = substitude( str, L".", L"\\." );
		str = substitude( str, L"*", L".*" );

		ret.push_back( std::wregex(str, std::regex_constants::icase) );
	}

	return ret;
}

bool Search::match_file_type( const std::filesystem::path & file )
{
  std::wstring file_name = file.wstring();

  if( regex_pattern_list.empty() ) {
	  regex_pattern_list = build_pattern_list( config->pattern );
  }

  for( std::wregex & regex : regex_pattern_list ) {

	  bool ret = std::regex_match( file_name, regex );

	  if( ret ) {
		  return true;
	  }
  }

  return false;
}

void Search::do_search( const std::filesystem::path & file )
{
  std::wstring content;
  ReadFile read_file;

  if( config->search.empty() ) {

	  if( config->icase ) {

		  const std::wstring lfile = tolower( file.filename().wstring() );

		  if( lfile.find(lsearch) != std::wstring::npos ) {
			  config->mt_result->access().push_back(Result( file ));
			  config->mt_result->free();
		  }

	  } else {

		  if( file.filename().wstring().find(config->search) != std::wstring::npos ) {
			  config->mt_result->access().push_back(Result( file ));
			  config->mt_result->free();
		  }
	  }


  } else {

	  if( !read_file.read_file( file.string(), content ) ) {
		  DEBUG( wformat( L"cannot open file: '%s'", file.wstring() ) );
		  return;
	  }

	  if( config->icase ) {
		  DEBUG( lsearch );
		  do_simple_search( tolower(content), lsearch, file );
	  } else {
		  do_simple_search( content, config->search, file );
	  }
  }
}

void Search::do_simple_search( const std::wstring & s,
							   const std::wstring & search_term,
							   const std::filesystem::path & file )
{
	std::wstring::size_type pos = 0;
	unsigned line_count = 0;

	do {
		std::wstring::size_type pos_linebreak = s.find( L'\n', pos );
		std::wstring::size_type pos_keyword = s.find( search_term, pos );

		if( pos_keyword == std::wstring::npos ) {
			break;
		}

		if( pos_linebreak != std::wstring::npos ) {
			line_count++;
			pos = pos_linebreak+1;
		}

		if( pos_keyword < pos ) {
			config->mt_result->access().push_back(Result( file, line_count, pos_keyword ));
			config->mt_result->free();
		}

	} while( pos != std::string::npos );
}

Search::Config Search::getConfFromCommandLine( int argc, const char *const*argv )
{
	Search::Config conf;

	std::list<std::wstring> args;

	for( int i = 1; i < argc; i++ ) {
		args.push_back( DetectLocale::in2w( argv[i] ) );
	}
	auto it_first = args.begin();

#ifdef WIN32
	if( icase_cmp( *it_first, L"qc" ) ) {
		conf.pattern = L"*.c,*.cpp,*.cc";
		args.erase(it_first);
	} else if( icase_cmp( *it_first, L"qh" ) ) {
		conf.pattern = L"*.h,*.hh,*.hpp";
		args.erase(it_first);
	} else if( icase_cmp( *it_first, L"qch" ) ) {
		conf.pattern = L"*.h,*.hh,*.c,*.cpp,*.cc";
		args.erase(it_first);
	} else if( icase_cmp( *it_first, L"qrc" ) ) {
		conf.pattern = L"*.rc";
		args.erase(it_first);
	}
#endif

	auto erase_empty_args = []( auto & arg ) {
		return arg.empty();
	};
	std::erase_if( args, erase_empty_args );


	auto arg_ignore_case = [&conf]( auto & arg ) {
		if( arg == L"-i" ) {
			conf.icase = true;
			return true;
		}
		return false;
	};
	std::erase_if( args, arg_ignore_case );


	auto clear_all_other_args = [&conf]( auto & arg ) {
		return arg.starts_with( L"-" );
	};

	std::erase_if( args, clear_all_other_args );

	auto unescape_minus = [&conf]( auto & arg ) {
		if( arg.starts_with( LR"(\-)" ) ) {
			arg = arg.substr( 1 );
		}
	};

	std::for_each( args.begin(), args.end(), unescape_minus );


	if( !args.empty() ) {
		it_first = args.begin();
		conf.search = it_first->c_str();

		args.erase( it_first );
	}

	for( auto & arg : args ){
		if( conf.pattern.empty() ) {
			conf.pattern = arg.c_str();
		} else {
			conf.pattern += ',' + arg.c_str();
		}
	}

	conf.path = std::filesystem::current_path().wstring();

	return conf;
}

std::wostream & operator<<( std::wostream & out, const Search::Config & config )
{
	/*
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
	*/
	out << L"path: " << config.path << L"\n"
		<< L"pattern: " << config.pattern << L"\n"
		<< L"search: " << config.search;
/*
	if( config.mt_running->get() ) {
		out << L"running\n";
	}

	out << "status: " << config.mt_status->get() << " of " << config.mt_status_max->get() << "\n";

	if( config.mt_stop->get() ) {
		out << L"stopping\n";
	}

	out << L"runtime: " << config.mt_runtime.get();
	*/
	return out;
}

std::wostream & operator<<( std::wostream & out, const Search::Result & result )
{
	return out << result.file.wstring()
			   << L"{ line: " << result.line << L", pos: " << result.pos << L"}";
}
