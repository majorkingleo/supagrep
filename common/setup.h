#ifndef SETUP_H
#define SETUP_H

#include <vector>
#include <string>

class Lang;

class Setup
{
 public:
  struct ConfigPair
  {
	std::wstring entry;
	std::wstring descr;

	ConfigPair( const std::wstring & entry_, const std::wstring & descr_ )
	  : entry( entry_ ), descr( descr_ )
	{}
  };
  
  typedef std::vector<ConfigPair> config_pair_list;
  typedef std::vector<ConfigPair>::iterator config_pair_list_it;

  struct Config
  {
	config_pair_list file_pattern;
  } config;

  Setup( Lang * lang );
};

#endif
