#ifndef LANG_H
#define LANG_H

#include <vector>
#include <fx.h>

class Lang
{
 protected:
  struct StaticText
  {
	const char *lang;
	const char *orig;
	const char *result;
  };

  std::vector<StaticText> messages;
  
  static StaticText TransMsg[];

  const char* cl;

 public:
  Lang();

  const char* translate( const char * msg );
  const char* LC( const char * msg ) { return translate( msg ); }
};

#endif
