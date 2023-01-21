/*
 * $Log: format.cpp,v $
 * Revision 1.1.1.1  2006/03/17 19:49:16  wamas
 * own tools reponsitority
 *
 * Revision 1.2  2006/03/09 00:48:27  wamas
 * Added CVS Log Info
 *
 */

#include "format.h"

#ifndef NFORMAT

using namespace Tools;

void Format::CFormat::reset()
{
  valid = false;
  adjust = RIGHT;
  special = false;
  precision = 6;
  precision_explicit = false;
  zero = false;
  sign = false;
  width = 0;
  internal = false;
  setupper = false;
  grouping = false;
  conversion = false;
  base = DEC;
  floating = FIXED;
  showbase = false;
  strlength = 0;
}

int Format::skip_atoi( std::string s, ST start, ST& pos )
{
  pos = start;
  ST len = s.size();
  
  while( (pos < len) && isdigit( s[pos] ) )
    pos++;
  
  return atoi( s.substr( start, start-pos ).c_str() );
}

void Format::CFormat::set( std::ostream& out )
{
  if( !valid )
    {
      return;
    }

/*
  printf( "valid: %d\n", valid );
  printf( "adjust: %d\n", adjust );
  printf( "special: %d\n", special );
  printf( "precision: %d\n", precision );
  printf( "precision_explicit: %d\n", precision_explicit );
  printf( "zero: %d\n", zero );
  printf( "sign: %d\n", sign );
  printf( "width: %d\n", width );
  printf( "internal: %d\n", internal );
  printf( "setupper: %d\n", setupper );
  printf( "grouping: %d\n", grouping );
  printf( "conversion: %d\n", conversion );
  printf( "base: %d\n", base );
  printf( "floating: %d\n", floating );
  printf( "showbase: %d\n", showbase );
  printf( "strlength: %d\n", strlength );
*/

  if( base == HEX && special && showbase && zero ) 
  {
      // without this correction:
      // printf( "[%#08x]", 0x42 ) => [0x000042]
      // fromat( "[%#08x]", 0x42 ) => [00000x42]

      showbase = false;
      out << '0' << ( setupper ? 'X' : 'x' );
      width -= 2;
  }

  if( base == HEX && special && showbase && strlength )
  {
      /*
	 sprintf( buffer, "[%#8.3x]", 0x42 ) => [   0x042]
      */

      showbase = false;
      
      if( width )
      {
	  for( int i = 0; i + strlength + 2 + 1 < width; ++i )
	      out << ' ';
	  width = 0;
      }

      out << '0' << ( setupper ? 'X' : 'x' );
      for( int i = 0; i + strlength < precision; ++i )
	  out << '0';
  }  

  if( adjust == LEFT && zero )
  {
/*
      sprintf( buffer, "[%-#08x]", 0x42 ); => [0x42    ]
      not => [0x420000]
*/
      zero = false;
  }

  switch( adjust )
    {
    case LEFT: out.setf( std::ios::left, std::ios::adjustfield ); break;
    case RIGHT: out.setf( std::ios::right, std::ios::adjustfield ); break;
    }

  if( zero ) out << std::setfill('0');
  else out << std::setfill( ' ' );
    
  if( sign ) out.setf( std::ios::showpos );
    else out.unsetf( std::ios::showpos );

  if( internal ) 
    out.setf( std::ios::internal, std::ios::adjustfield );
  
  switch( base )
    {
    case OCT: out.setf( std::ios::oct, std::ios::basefield ); break;
    case DEC: out.setf( std::ios::dec, std::ios::basefield ); break;
    case HEX: out.setf( std::ios::hex, std::ios::basefield ); break;
    }

  if( setupper ) out.setf( std::ios::uppercase );
  else out.unsetf( std::ios::uppercase );

  switch( floating )
    {
    case FIXED: out.setf( std::ios::fixed, std::ios::floatfield ); break;
    case SCIENTIFIC: out.setf( std::ios::scientific, std::ios::floatfield ); break;
    }

  if( showbase )
    out.setf( std::ios::showbase );
  else
    out.unsetf( std::ios::showbase );

  out << std::setw( width );
  out << std::setprecision( precision );
}

#endif
