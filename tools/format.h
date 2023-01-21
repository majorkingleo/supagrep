/*
 * $Log: format.h,v $
 * Revision 1.5  2008/08/06 12:24:55  wamas
 * new g++ fixes
 *
 * Revision 1.4  2008/02/20 09:30:20  wamas
 * rewrote format for porting code to gccs -fstrict-aliasing
 * and make it on M$ compiler work
 *
 * Revision 1.3  2007/03/14 21:47:08  wamas
 * nicer cast
 *
 * Revision 1.2  2006/11/23 16:46:26  wamas
 * MOBERZA -Wshadow dazugeschalten und ausgebessert
 *
 * Revision 1.1.1.1  2006/03/17 19:49:16  wamas
 * own tools reponsitority
 *
 * Revision 1.2  2006/03/09 00:48:27  wamas
 * Added CVS Log Info
 *
 */

/*
  Format a C++ library for typesafe string formating in printf style
      (C) 2001 - 2003 by Martin Oberzalek <kingleo@gmx.at>  

   2004-01-01 remerged version 1.1 of Format with xstow's Format

  Examples:
     std::cout << format( "Hello %s, I have $05d$ in my pocket", "world", 5 ) << std::endl;
     std::cout << format( "Do not try this with printf: %s", 10101 ) << std::endl;

  
*/

#ifndef TOOLS_format_h
#define TOOLS_format_h

#include <string>
#include <iomanip>
#include <iostream>

#include "ref.h"

#define HAVE_STL_SSTREAM

#if __GNUC__ == 2
#undef HAVE_STL_SSTREAM
#endif

#ifdef HAVE_STL_SSTREAM
#  include <sstream>
#  include <cctype>
#  define IS_DIGIT( x ) std::isdigit( x )
#else
extern "C" {
#  include <ctype.h>
}
#  include <strstream>
#  define IS_DIGIT( x ) isdigit( x )
#endif  

namespace Tools {

namespace Format {
	class CFormat;
}

inline std::ostream& operator<<( std::ostream& out, Format::CFormat cf );

namespace Format
{
  typedef std::string::size_type ST;
  
  class CFormat
  {    
  public:
    
    enum Adjust
      {
	LEFT,
	RIGHT
      };
    
    enum Base
      {
	OCT,
	DEC,
	HEX
      };
    
    enum Floating
      {
	FIXED,
	SCIENTIFIC
      };
    
    bool valid;
    Adjust adjust;
    bool special;
    bool sign;
    bool grouping; // SUSv2 extension
    bool conversion; // glibc 2.2 extension
    bool zero;
    bool precision_explicit;
    bool internal;
    Base base;
    bool setupper;
    Floating floating;
    bool showbase;
    
    int width;
    int precision;
    int strlength;

    std::string format;

  public:    
    CFormat() { reset(); }
    
    void set( std::ostream& out );
    
  private:    
    void reset();
  };
  
  /****************************************/

  typedef BaseException Error;

  template <class A, class B, class C, class D, class E, class F>
  class Format
  {
  private:
    struct Arg
    {
      bool is_int;
      bool is_string;
    };
    
    Arg args[6];

    std::string format;
    
    A a;
    B b;
    C c;
    D d;
    E e;
    F f;
    
    unsigned int num_of_args;
    
    std::string s;
    
  public:
    Format( const std::string &format, A a, B b, C c, D d, E e, F f, unsigned int num_of_args );
    
    std::string get_string() const { return s; }
    
  private:
    void parse();

    template <class N> bool is_int( N &n ) { return false; }
    bool is_int( int &n ) { return true; }
    bool is_int( unsigned int &n ) { return true; }
    bool is_int( short &n ) { return true; }
    bool is_int( unsigned short ) { return true; }
    bool is_int( unsigned long ) { return true; }
    bool is_int( unsigned long long ) { return true; }
    bool is_int( long long ) { return true; }

    template <class S> bool is_string( S &s_ ) { return false; }
    bool is_string( std::string& s_ ) { return true; }
    bool is_string( const std::string& s_ ) { return true; }
    bool is_string( char* ) { return true; }
    bool is_string( const char* ) { return true; }

    int get_int_arg( unsigned int num );
    void gen_arg_list();
    std::string use_arg( unsigned int i, const CFormat &cf );

    template <class S> std::string x2s( S ss, const CFormat &cf )
    {
#ifdef HAVE_STL_SSTREAM
	std::stringstream str;
	str << cf << ss;
	std::string st = str.str();
	return st;
#else
      std::strstream str;
      str << cf << ss << std::ends;
      std::string st = str.str();
      str.freeze(0);
      return st;
#endif
    }

	template<class T> int get_int( const T &t ) { return 0; }
	int get_int( int n ) { return n; }
    int get_int( unsigned int n ) { return n; }
    int get_int( short n ) { return n; }
    int get_int( unsigned short n ) { return n; }
    int get_int( long long n ) { return n; }
    int get_int( unsigned long long n  ) { return n; }
    int get_int( long n ) { return n; }
    int get_int( unsigned long n ) { return n; }

  };

  int skip_atoi( std::string s, ST start, ST& pos );

} // namespace Format

inline std::ostream& operator<<( std::ostream& out, Format::CFormat cf )
{
  cf.set( out );
  return out;
}

namespace Format { /* M$ Comiler can't handle it otherwiese */

template <class A, class B, class C, class D, class E, class F>
Format<A,B,C,D,E,F>::Format( std::string const &format_, A a_, B b_, C c_, D d_, E e_, F f_, unsigned int num_of_args_ )
  : format( format_ ), a(a_), b(b_), c(c_), d(d_), e(e_), f(f_), num_of_args( num_of_args_ )
{
  if( num_of_args > 6 )
    throw Error( "Number of args out of range" );

  gen_arg_list();
  
  parse();
}

template <class A, class B, class C, class D, class E, class F>
int Format<A,B,C,D,E,F>::get_int_arg( unsigned int num )
{
  if( static_cast<unsigned int>(num) > num_of_args - 1 )
    throw Error( "The arg you wan't to use is out of range" );

  if( num < 0 )
    throw Error( "negativ number for arg number not allowed" );

  if( args[num].is_int ) 
    {
      switch( num )
	{
	case 0: return get_int(a); // I have to cast here cause the compiler
	case 1: return get_int(b); // will make troubles if any of these
	case 2: return get_int(c); // values is not an unsigned int.
	case 3: return get_int(d); // Even if we are sure that
	case 4: return get_int(e); // an unsigned int value will be returned
	case 5: return get_int(f);
	}
    }
  else
    throw Error( "expecting int arg" );

  return 0; // should never be reached
}

template <class A, class B, class C, class D, class E, class F>
void Format<A,B,C,D,E,F>::gen_arg_list()
{
  for( unsigned int i = 0; i < num_of_args; i++ )
    { 
      switch( i )
	{
	case 0: 
	  args[i].is_int = is_int( a ); 
	  args[i].is_string = is_string( a ); 
	  break;
	case 1: 
	  args[i].is_int = is_int( b ); 
	  args[i].is_string = is_string( b ); 
	  break;

	case 2: 
	  args[i].is_int = is_int( c ); 
	  args[i].is_string = is_string( c ); 
	  break;

	case 3: 
	  args[i].is_int = is_int( d ); 
	  args[i].is_string = is_string( d ); 
	  break;

	case 4: 
	  args[i].is_int = is_int( e ); 
	  args[i].is_string = is_string( e ); 
	  break;

	case 5: 
	  args[i].is_int = is_int( f ); 
	  args[i].is_string = is_string( f ); 
	  break;
	}
    }
}

template <class A, class B, class C, class D, class E, class F>
std::string Format<A,B,C,D,E,F>::use_arg( unsigned int i, const CFormat &cf )
{
  if( i > num_of_args || i < 0 )
    throw Error( "out of arg range" );

  switch( i )
    {
    case 0: return x2s( a, cf );
    case 1: return x2s( b, cf );
    case 2: return x2s( c, cf );
    case 3: return x2s( d, cf );
    case 4: return x2s( e, cf );
    case 5: return x2s( f, cf );
    }

  return "";
}

template <class A, class B, class C, class D, class E, class F>
void Format<A,B,C,D,E,F>::parse()
{
  if( format.empty() )
    return;

  unsigned int par = 0;
  unsigned int use_par = 0;
  ST pos = 0;
  ST len = format.size();
  s = "";
  bool had_precision = false;

  while( par < num_of_args && pos < len )
    { // while

      use_par = par;

      if( pos >= len )
	break;

      if( format[pos] != '%' )
	{
	  s += format[pos];
	  pos++;
	  continue;
	}

      // % digit found
      pos++;

      if( !(pos < len ) || (format[pos] == '%') )
	{
	  // %% -> %
	  s += format[pos];
	  pos++; 
	  continue;
	}
  
      // format string found

      ST start = pos - 1;
      CFormat cf;

      // process flags

      while( (pos < len) )
	{
	  bool finished = false;
	  
	  switch( format[pos] )
	    {
	    case '-' : cf.adjust = CFormat::LEFT; break;
	    case '+' : cf.sign = true; break;
	    case ' ' : cf.zero = false; break;
	    case '#' : cf.special = true; break;
	    case '\'': cf.grouping = true; break;
	    case 'I' : cf.conversion = true; break; 
	    case '0' : cf.zero = true; break;
	    default: finished = true; break;
	    }
	  
	  if( finished )
	    break;
	  
	  pos++;
	} //       while( (pos < len) )

      // get argument number
      if( pos < len )
	{
	  // search for the $ digit
	  unsigned int dp = pos;
	  
	  while( dp < len && IS_DIGIT( format[dp] ) ) 
	    dp++;
	  
	  if( dp < len && format[dp] == '$' )
	    {
	      use_par = skip_atoi( format, pos, pos ) - 1;
	      pos = dp + 1;
	    }
	}

      // get field with
      if( pos < len )
	{
	  if( IS_DIGIT( format[pos] ) )
	    cf.width = skip_atoi( format, pos, pos );
	  else if( format[pos] == '*' )
	    {
	      pos++;

	      // search for the $ digit
	      unsigned int dp = pos;
	      
	      while( dp < len && IS_DIGIT( format[dp] ) ) 
		dp++;

	      if( dp < len && format[dp] == '$' )
		{
		  cf.width = get_int_arg( skip_atoi( format, pos, pos ) - 1 );
		  // skip $ sign
		  pos++;
		}
	      else 
		{
		  cf.width = get_int_arg( par );

		  if( use_par == par )
		    use_par++;

		  par++;
		} 

	      if( cf.width < 0 )
		{
		  cf.width *= -1;
		  cf.adjust = CFormat::LEFT;
		}
	    }
	}

      // precision
      if( pos < len )
	{
	  if( format[pos] == '.' )
	    {
	      pos++;
	      if( !(pos < len) )
		return;
	      
	      had_precision = true;

	      if( IS_DIGIT( format[pos] ) )
		  cf.precision = skip_atoi( format, pos, pos );
	      else if( format[pos] == '*' )
		{
		  pos++;
		  
		  
		  // search for the $ digit
		  unsigned int dp = pos;
		  
		  while( dp < len && IS_DIGIT( format[dp] ) ) 
		    dp++;
		  
		  if( dp < len && format[dp] == '$' )
		    {
		      cf.precision = get_int_arg( skip_atoi( format, pos, pos ) - 1 );
		      // skip $ sign
		      pos++;
		    }
		  else 
		    {
		      cf.precision = get_int_arg( par );

		      if( use_par == par )
			use_par++;

		      par++;
		    }
		  
		  if( cf.precision == 0)
		    cf.precision_explicit = true;
		  
		  if( cf.precision < 0 )
		    cf.precision = 0;
		}
	      else
		cf.precision = 0;
	    }
	  
	}
    
      // lenght modifier
      /* 
	 they will be ignored 
	 cause we know the types of the parameter
      */      
      if( (pos < len) )
	{
	  bool hh = false;
	  bool ll = false;
	  bool found = false;
	  
	  switch( format[pos] )
	    {
	    case 'h': hh = true; found = true; break;
	    case 'l': ll = true; found = true; break;
	    case 'L':
	    case 'q':
	    case 'j':
	    case 'z':
	    case 't': found = true; break;
	    default: break;
	    }

	  if(found )
	    {
	      pos++;
	      
	      if( pos < len ) {
		if( hh == true )
		  {
		    if( format[pos] == 'h' ) {
		      pos++;
			}
		  }
		else if( ll == true ) {
		  if( format[pos] == 'l' ) {
		    pos++;
			}
			}
		}
	    }
	}

      // conversion specifier

      if( pos < len )
	{
	  bool invalid = false;
	   switch( format[pos] )
	     {
	     case 'd':
	     case 'i':
	       cf.base = CFormat::DEC;
	       if( cf.zero && (cf.adjust != CFormat::LEFT) )
		 cf.internal = true;
	       break;
	       
	     case 'X': cf.setupper = true;
	     case 'x': 
	       cf.base = CFormat::HEX;
	       if( cf.special )
		 cf.showbase = true;
	       break;
	       
	     case 'o': 
	       cf.base = CFormat::OCT;
	       if( cf.special )
		 cf.showbase = true;
	       break;

	       
	     case 'E': 
	       cf.setupper = true;

	     case 'e': 
	       if( cf.special )
		 cf.sign = true;
	       cf.floating = CFormat::SCIENTIFIC;
	       break;
	       
	     case 'F': // not supported
	     case 'f': 
	       if( cf.special )
		 cf.sign = true;
	       cf.floating = CFormat::FIXED;
	       break;
	       
	     case 's':
	       if( cf.zero ) 
		 cf.zero = false;
	       break;
	      

	       // unsupported modifiers


	     case 'G':
	     case 'g':
	       
	     case 'A':
	     case 'a':
	       
	     case 'c':
	       
	     case 'C':
	     case 'S':
	     case 'P':
	     case 'n': break;
	       
	     default: invalid = true;
	     }

	   if( !invalid )
	     cf.valid = true;
	}  

      if( cf.valid )
	{
	  std::string str;
	  int upar = par;

	  if( use_par != par )
	    upar = use_par;

	  if( cf.base == CFormat::HEX && had_precision && cf.special )
	  {
	      CFormat f2;
	      f2.base = cf.base;
	      std::string ss = use_arg( upar, f2 );
	      cf.strlength = ss.size();
//	      printf( "str: %s\n", s.c_str() );
	  }

	  str = use_arg( upar, cf );

	  // cut string
	  if( had_precision && args[upar].is_string )
	    str = str.substr( 0, cf.precision );

	  s += str;

	  if( use_par == par )
	    par++;
	}
      else
	{
	  // copy the invalid format string
	  for( ST i = start;  i<= pos; i++ )
	    if( i < len )
	      s += format[i];
	}
  
      pos++;

    } // while

  if( pos < len )
    {
      while( pos < len )
	{
	  s += format[pos];
	  pos++;
	}
    }
}

} // namespace Format

template <class A, class B, class C, class D, class E, class F>
inline std::string format( std::string fs, A a, B b, C c, D d, E e, F f )
{
  return Format::Format<A,B,C,D,E,F>( fs, a, b, c, d, e, f, 6).get_string();
}

template <class A, class B, class C, class D, class E>
inline std::string format( std::string fs, A a, B b, C c, D d, E e )
{
  return Format::Format<A,B,C,D,E,char>( fs, a, b, c, d, e, 0, 5).get_string();
}

template <class A, class B, class C, class D>
inline std::string format( std::string fs, A a, B b, C c, D d)
{
  return Format::Format<A,B,C,D,char,char>( fs, a, b, c, d, 0, 0, 4).get_string();
}

template <class A, class B, class C>
inline std::string format( std::string fs, A a, B b, C c )
{
  return Format::Format<A,B,C,char,char,char>( fs, a, b, c, 0, 0, 0, 3).get_string();
}

template <class A, class B>
inline std::string format( std::string fs, A a, B b )
{
  return Format::Format<A,B,char,char,char,char>( fs, a, b, 0, 0, 0, 0, 2).get_string();
}

template <class A>
inline std::string format( std::string fs, A a)
{
  return Format::Format<A,char,char,char,char,char>( fs, a, 0, 0, 0, 0, 0, 1).get_string(); 
} 


namespace Format
{
  template<typename ostream> class PrintF
  {
  private:

    int level;
    int dlevel;

    int dmodule;
    int module;
    

  public:

    ostream &out;
    
  public:

    PrintF( ostream &out_ = std::cout, int module_ = -1, int debug_level_ = -1 ) 
      : level( debug_level_ ), dlevel( debug_level_ ), 
      dmodule( module_), module( module_),  out( out_ ) 
      {}

    void set_debug_level( int dlevel_ ) { dlevel = dlevel_; }
    void set_module( int module_ ) { dmodule = module_; }

    PrintF operator()( int module_ )
      {
	PrintF printf( *this );
	printf.module = module_;

	return printf;
      }

    PrintF operator[]( int level_ )
      {
	PrintF printf( *this );
	printf.level = level_;

	return printf;
      }

    template<typename T> PrintF& operator<<( const T &t ) 
      { 
	if( check() )
	  out << t; 

	return *this; 
      }

    // io manipulator overloading
    PrintF& operator<<(ostream& (*f)(ostream&))
      {
	if( check() )
	  out << f;

	return *this;
      }

    PrintF& operator()( std::string fs )
    {
      if( check() )
	out << fs;

      return *this;
    }


    template<typename A> PrintF& operator()( std::string fs, const A &a )
    {
      if( check() )
	out << format( fs, a );

      return *this;
    }

    template<typename A, typename B> 
    PrintF& operator()( std::string fs, const A &a, const B &b )
    {
      if( check() )
	out << format( fs, a, b );

      return *this;
    }

    template<typename A, typename B, typename C> 
    PrintF& operator()( std::string fs, const A &a, const B &b, const C &c )
    {
      if( check() )
	out << format( fs, a, b, c );

      return *this;
    }

    template<typename A, typename B, typename C, typename D> 
    PrintF& operator()( std::string fs, const A &a, const B &b, const C &c, const D &d )
    {
      if( check() )
	out << format( fs, a, b, c, d );

      return *this;
    }

    template<typename A, typename B, typename C, typename D, typename E> 
    PrintF& operator()( std::string fs, const A &a, const B &b, const C &c, const D &d, const E &e )
    {
      if( check() ) 
	out << format( fs, a, b, c, d, e );

      return *this;
    }

    template<typename A, typename B, typename C, typename D, typename E, typename F> 
    PrintF& operator()( std::string fs, const A &a, const B &b, const C &c, const D &d, const E &e, const F &f )
    {
      if( check() )
	out << format( fs, a, b, c, d, e, f );

      return *this;
    }

    bool check( int module_, int level_ ) const
      {
	if( module_ == dmodule || dmodule == -1 )
	  {
	    if( dlevel == -1 )
	      return true;
	    
	    if( level_ <= dlevel )
	      return true;
	  }

	return false;	
      }

  private:
    
    bool check() const { return check( module, level ); }

  };

}

} // namespace Tools

#undef IS_DIGIT

#endif
