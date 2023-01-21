#ifndef DEBUG_H
#define DEBUG_H

#ifdef NDEBUG
# define DEBUG( out )
#else

#include "format.h"

#define DEBUG( x ) if( x_debug ) { x_debug->add(  __FILE__, __LINE__, __PRETTY_FUNCTION__, x ); }

namespace Tools {

class Debug
{
 public:
  Debug();
  virtual ~Debug();
  
  virtual void add( const char *file, unsigned line, const char *function, const std::string & s ) = 0;
};

extern Debug *x_debug;

}

#endif

#endif
