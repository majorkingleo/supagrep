#ifndef LANG_H
#define LANG_H

#include <vector>
#include <string>

class Lang
{
 protected:
  struct StaticText
  {
	const char8_t *lang;
	const char8_t *orig;
	const char8_t *result;
	std::wstring worig;
	std::wstring wresult;
  };

  std::vector<StaticText> messages;
  
  static StaticText TransMsg[];

  std::u8string cl;

 public:
  Lang();

  const char* translate( const char * msg );
  std::wstring translate( const std::wstring & msg );
  const char* LC( const char * msg ) { return translate( msg ); }
  std::wstring wLC( const std::wstring & msg ) { return translate( msg ); }
};

#endif
