#include "setup.h"
#include "lang.h"

Setup::Setup( Lang * lang )
{
  /* default file patterns */

  config.file_pattern.push_back( ConfigPair( L"*.c,*.cpp,*.cc",
											 lang->wLC(L"C and C++ Source files") ) );
  config.file_pattern.push_back( ConfigPair( L"*.h,*.hh,*.hpp",
											 lang->wLC(L"C and C++ Header files" ) ) );
  config.file_pattern.push_back( ConfigPair( L"*.h,*.hh,*.hpp,*.c,*.cc,*.cpp",
											 lang->wLC(L"C and C++ Source and Header files") ) );
  config.file_pattern.push_back( ConfigPair( L"*.rc",
											 lang->wLC(L"RC Files") ) );

}
