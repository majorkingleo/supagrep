/*
 * $Log: string_utils.cpp,v $
 * Revision 1.3  2008/06/22 12:50:20  wamas
 * improvement
 *
 * Revision 1.2  2006/11/23 16:46:27  wamas
 * MOBERZA -Wshadow dazugeschalten und ausgebessert
 *
 * Revision 1.1.1.1  2006/03/17 19:49:15  wamas
 * own tools reponsitority
 *
 * Revision 1.2  2006/03/09 00:48:27  wamas
 * Added CVS Log Info
 *
 */

#include "string_utils.h"
#include "format.h"
#include <cctype>

namespace Tools {

std::string toupper( std::string s )
{
  for( unsigned int i = 0; i < s.size(); ++i )
    s[i] = std::toupper( s[i] );

  return s;
}

bool is_int( const std::string &s )
{
  if( s.empty() )
    return false;

  for( unsigned int i = 0; i < s.size(); ++i )
    {
      switch( s[i] )
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	  break;

	default:
	  return false;
	}
    }

  return true;
}

/*
  Re: Trim Funktion für Strings
  Von: Hubert Schmid <h.schmid-usenet@gmx.de>
  Datum:  Sonntag, 10. Oktober 2004 14:13:35
  Gruppen:  de.comp.lang.iso-c++
  Message-ID:  <87acuug49c.fsf@dent.z42.de>
*/
std::string strip( const std::string& str, const std::string& what )
{
    std::string::size_type p = str.find_first_not_of(what);
    
    if( p == std::string::npos )
    {
	return std::string();

    } else {

	std::string::size_type q = str.find_last_not_of(what);

	return std::string(str, p, q - p + 1);
    }
}

std::string text_right_format( std::string s, unsigned int max_size, unsigned int spaces )
{
  std::string space;
  
  for( unsigned int i = 0; i < spaces; ++i )
    space += ' ';
  
  unsigned int i;
  
  for( i = max_size; i >= 0 && s[i] != ' '; --i );
  
  std::string ss = s.substr( i + 1 );
  s = s.substr( 0, i );
 
  bool has_new_line = false;
 
  while( !ss.empty() )
    {
      for( i = max_size - spaces; i < ss.size() && i >= 0 && ss[i] != ' '; --i );
      
      s += '\n';
      s += space;      

      s += ss.substr( 0, i );
      
      has_new_line = true;

      if( i >= ss.size() )
	break;
      
      ss = ss.substr( i + 1 );
    }

  if( has_new_line )
    s +='\n';

  return s;
}

std::vector<std::string> split_simple( std::string str, std::string sep, int max )
{
  str = strip( str, sep );

  std::string::size_type start = 0, last = 0;
  int count = 0;

  std::vector<std::string> sl;

  while( true )
    {
      if( max > 0 )
	count++;

      if( count >= max && max > 0 )
	{
	  sl.push_back( str.substr( last ) );
	  break;
	}


      start = str.find_first_of( sep, last );

      if( start == std::string::npos )
	{
	  sl.push_back( str.substr( last ) );
	  break;
	}

      sl.push_back( str.substr( last, start - last ) );

      last = start + 1;
    }

  return sl;
}

std::vector<std::string> split_string( std::string str, std::string sep, int max  )
{
  std::string::size_type start = 0, last = 0;
  int count = 0;

  std::vector<std::string> sl;

  while( true )
    {
      if( max > 0 )
	count++;

      if( count >= max && max > 0 )
	{
	  sl.push_back( str.substr( last ) );
	  break;
	}


      start = str.find( sep, last );

      if( start == std::string::npos )
	{
	  sl.push_back( str.substr( last ) );
	  break;
	}

      sl.push_back( str.substr( last, start - last ) );

      last = start + sep.size();
    }

  return sl;
}
/*
bool s2x( const std::string &s )
{
  if( s == "0" || toupper( s ) == "FALSE" )
    return false;
  
  if( s == "1" || toupper( s ) == "TRUE" )
    return true;
  
  out( "Warning: illegal value for boolean state: %s\n", s );

  return false;
}
*/

bool s2bool( const std::string &s )
{
    if( s == "1" || toupper( s ) == "TRUE" )
	return true;

    return false;
}

std::string x2s( bool b )
{
  if( b )
    return "TRUE";

  return "FALSE";
}

std::string substitude( std::string str, std::string what, std::string with )
{
  std::string::size_type pos=0;

  if( what.empty() )
      return str;

  for(;;)
    {
      pos = str.find( what, pos );
      if( pos == std::string::npos )
        break;

      if( with.empty() )
	{
	  std::string s = str.substr( 0, pos + what.size() );
	  s = str.substr( pos + what.size() );
	  str = s;
	  continue;
	}
      else
	{
	  str.replace( pos, what.size(), with );
	}

      pos += (what.size() > with.size() ? what.size() : with.size() );
    }
  return str;
}

std::string prepand( std::string str, std::string what, std::string prefix )
{
    std::string::size_type pos = 0, start1 = 0, start2 = 0, start = 0;

    for(;;)
    {
	pos = str.find( what, pos );
	
	if( pos == std::string::npos )
	    break;

	start1 = str.rfind( '\"', pos );
	start2 = str.rfind( '=', pos );              

	if( start1 == std::string::npos && start2 != std::string::npos )
	    start = start2;
	else if( start2 == std::string::npos && start1 != std::string::npos )
	    start = start1;
	else if( start1 == std::string::npos && start2 == std::string::npos )	    
	    break;
	else if( start1 > start2 )
	    start = start1;
	else
	    start = start2;

	str.insert( start + 1, prefix );
	pos += prefix.size();
	pos++;
    }

    return str;
}

std::string bin_encode( const std::string &s )
{
    std::string ret;

    for( unsigned i = 0; i < s.size(); i++ )
    {
	ret += format( "%03d", static_cast<int>( s[i] ) );
    }

    return ret;
}

std::string bin_decode( const std::string &s )
{
    std::string ret;

    for( unsigned i = 0; i < s.size(); i += 3 )
    {
	std::string ss = s.substr( i, 3 );
	int x = s2x<int>( ss );
	ret += static_cast<char>(x);
    }

    return ret;
}

std::string group_thousand( const std::string &s, unsigned digit, const std::string &sep )
{
    if( s.empty() )
	return std::string();

    std::string ret;       

    for( int i = static_cast<int>(s.size()) - 1, count = 0; i >= 0; i--, count++ )
    {
	if( count > 0 && count % digit == 0 )
	    ret += sep;
	  
	ret += s[i];
    }

    std::string sret;

    sret.resize( ret.size() );

    for( int i = static_cast<int>(ret.size()) - 1, count = 0; i >= 0; i--, count++ )
    {
	sret[count] = ret[i];
    }

    return sret;
}

std::string escape( const std::string &s )
{
    return substitude( s, "'", "\\'" );
}

struct Pair
{
  std::string::size_type start;
  std::string::size_type end;

  Pair( std::string::size_type start_,   std::string::size_type end_ )
    : start( start_ ) , end( end_ )
  {}
};

static bool is_escaped( const std::string &s, std::string::size_type start )
{
  bool escaped = false;

  if( start > 0 )
    {
      if( s[start-1] == '\\' )
	{
	  escaped = true;

	  if( start > 1 )
	    {
	      if( s[start-2] == '\\' )
		{
		  escaped = false;
		} 
	      }
	}	
    }

  return escaped;
}

static std::vector<Pair> find_exclusive( const std::string &s )
{
  std::vector<Pair> ex;

  std::string::size_type pos = 0, start = 0, end = 0;

  while( true )
    {

      /* find starting pos */
      while( true )
	{
	  start = s.find( '"', pos ); 
	  
	  if( start == std::string::npos )
	    return ex;
	  
	  /* is the " escaped? */
	  if( is_escaped( s, start ) )
	    {
	      pos = start + 1;
	      continue;
	    }

	  break;
	}

      // find second "
      pos = start + 1;
      while( true )
	{	  
	  end = s.find( '"', pos );
	  
	  if( end == std::string::npos )
	    return ex;

	  if( is_escaped( s, end ) )
	    {
	      pos = end + 1;
	      continue;
	    }

	  break;
	}
      
      ex.push_back( Pair( start, end ) );
      pos = end + 1;
    }
}

static bool is_exclusive( const std::string &s, const std::vector<Pair> &exclude, std::string::size_type pos1 )
{
  bool exclusive = false;
  for( unsigned i = 0; i < exclude.size(); i++ )
    {
      if( exclude[i].start < pos1 &&
	  exclude[i].end > pos1 )
        {
	  exclusive = true;
	  break;
	}
    }

  return exclusive;
}

/* splits
       hello "my name is" 
   correct up into 'hello' and '"my name is"'
*/
std::vector<std::string> split_safe( const std::string &s, const std::string &sep )
{
  std::vector<Pair> exclude = find_exclusive( s );

  std::string::size_type pos1 = 0, pos2 = 0;

  std::vector<std::string> sl;

  while( true )
    {
      pos1 = s.find_first_of( sep, pos1 );
      
      if( pos1 == std::string::npos )
	{
	  sl.push_back( s.substr( pos2 ) );
	  return sl;
	}
      
      if( is_exclusive( s, exclude, pos1 ) )
	{
	  pos1++;
	  continue;
	}

      sl.push_back( s.substr( pos2, pos1 - pos2 ) );
      pos2 = pos1 + 1;
      pos1++;
    }

  return sl;
}

} // namespace Tools
