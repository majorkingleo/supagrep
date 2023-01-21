/*
 * $Log: string_utils.h,v $
 * Revision 1.4  2008/02/28 17:05:22  wamas
 * Bugfixes in Stringutils
 *
 * Revision 1.3  2008/02/20 09:30:20  wamas
 * rewrote format for porting code to gccs -fstrict-aliasing
 * and make it on M$ compiler work
 *
 * Revision 1.2  2007/08/27 17:22:51  wamas
 * Updated odbc Driver
 *
 * Revision 1.1.1.1  2006/03/17 19:49:16  wamas
 * own tools reponsitority
 *
 * Revision 1.2  2006/03/09 00:48:27  wamas
 * Added CVS Log Info
 *
 */

#ifndef TOOLS_string_utils_h
#define TOOLS_string_utils_h

#include <string>
#include <vector>

#define HAVE_STL_SSTREAM

#if __GNUC__ == 2
#undef HAVE_STL_SSTREAM
#endif

#ifdef HAVE_STL_SSTREAM
#  include <sstream>
#  include <cctype>
#  define strstream stringstream
#  define ENDS
#else
#  include <ctype.h>
#  include <strstream>
#  define ENDS << std::ends;
#  define STRSTREAM
#endif  

namespace Tools {

std::string toupper( std::string s );
std::string strip( const std::string& str, const std::string& what = " \t\n\r" );
bool is_int( const std::string &s );
std::vector<std::string> split_simple( std::string str, std::string seperator = " \t\n", int max = -1 );
std::vector<std::string> split_string( std::string str, std::string seperator, int max = -1 );

inline bool is_bool( bool b ) { return true; }
template<class T> bool is_bool( const T &t ) { return false; }
inline bool get_bool( bool b, bool x ) { return b; }

// dummy function
template<class T> T get_bool( bool b, T x ) { return T(); }


bool s2bool( const std::string &s );

/// convert a string to anything
template <class T> T s2x( const std::string& s )
{
    if( is_bool( T() ) )
    {
	  bool b = s2bool( s );

	  return get_bool(b, T() );
    }

  std::strstream str;
  
  str << s ENDS;
  T t;
  str >> t;

#ifdef STRSTREAM
  str.freeze(0);
#endif

  return t;
}

/// convert a string to anything
template <class T> T s2x( const std::string& s, const T & init )
{
    if( is_bool( T() ) )
    {
	return s2bool( s );
    }

  std::strstream str;
  
  str << s ENDS;
  T t(init);
  str >> t;

#ifdef STRSTREAM
  str.freeze(0);
#endif

  return t;
}


/// converts anything to a string
template<class T>std::string x2s( T what )
{
  std::strstream str;
  
  str << what ENDS;
  std::string s( str.str() );

#ifdef STRSTREAM
  str.freeze(0);
#endif

  return s;
}

std::string x2s( const bool b );

std::string text_right_format( std::string text, unsigned int max_size, unsigned int spaces );

std::string substitude( std::string str, std::string what, std::string with );

std::string prepand( std::string str, std::string what, std::string prefix );

inline bool icase_cmp( const std::string &a, const std::string &b )
{
    if( a.size() != b.size() )
	return false;
    
    if( a == b )
	return true;

    if( toupper( a ) == toupper( b ) )
	return true;

    return false;
}

std::string bin_encode( const std::string &s );
std::string bin_decode( const std::string &s );

std::string group_thousand( const std::string &s, unsigned digit = 3, const std::string &sep = "." );

std::string escape( const std::string &s );

std::vector<std::string> split_safe( const std::string &s, const std::string &sep = " \n\t");

} // namespace Tools

#undef STRSTREAM
#undef strstream
#undef ENDS

#endif
