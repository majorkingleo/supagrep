#ifndef SETUP_H
#define SETUP_H

#include <fx.h>
#include <vector>

class Lang;

class Setup
{
 public:
  struct ConfigPair
  {
	FXString entry;
	FXString descr;

	ConfigPair( const FXString & entry_, const FXString & descr_ )
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
