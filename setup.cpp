#include "setup.h"
#include "lang.h"

Setup::Setup( Lang * lang )
{
  /* default file patterns */

  config.file_pattern.push_back( ConfigPair( "*.c,*.cpp,*.cc", 
											 lang->LC("C and C++ Source files") ) );
  config.file_pattern.push_back( ConfigPair( "*.h,*.hh", 
											 lang->LC("C and C++ Header files" ) ) );
  config.file_pattern.push_back( ConfigPair( "*.h,*.hh,*.c,*.cc,*.cpp", 
											 lang->LC("C and C++ Source and Header files") ) );
  config.file_pattern.push_back( ConfigPair( "*.rc", 
											 lang->LC("RC Files") ) );

}
