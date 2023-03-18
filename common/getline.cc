#include "getline.h"
#include <vector>
#include <string_utils.h>

using namespace Tools;

std::wstring getline( const std::wstring & s, std::wstring::size_type pos )
{
  std::wstring::size_type end = s.find( L'\n', pos );

  return s.substr( pos, end - pos );
}


std::wstring::size_type get_pos_for_line( const std::wstring & content, int line )
{
	std::wstring::size_type pos = 0;

	for( int count = 1; count < line; count++ )
	{
		pos = content.find(L'\n', pos );

		if( pos == std::string::npos )
			return pos;

		pos++;
	}

	return pos;
}


std::string diff_lines( const std::string & orig, std::string & modded )
{
  std::vector<std::string> sl_orig, sl_modded;

  sl_orig = split_simple( orig, "\n" );
  sl_modded = split_simple( modded, "\n" );

  std::string res;

  for( unsigned i = 0; i < sl_orig.size() && i < sl_modded.size(); i++ )
	{
	  if( sl_orig[i] != sl_modded[i] )
		{
		  if( !res.empty() )
			res += '\n';

		  res += "\t" + strip( sl_orig[i] ) + " => " + strip( sl_modded[i] );
		}
	}

  return res;
}

std::wstring diff_lines( const std::wstring & orig, std::wstring & modded )
{
  std::vector<std::wstring> sl_orig, sl_modded;

  sl_orig = split_simple( orig, L"\n" );
  sl_modded = split_simple( modded, L"\n" );

  std::wstring res;

  for( unsigned i = 0; i < sl_orig.size() && i < sl_modded.size(); i++ )
	{
	  if( sl_orig[i] != sl_modded[i] )
		{
		  if( !res.empty() )
			res += '\n';

		  res += L"\t" + strip( sl_orig[i] ) + L" => " + strip( sl_modded[i] );
		}
	}

  return res;
}

std::wstring get_whole_line( const std::wstring & s, std::wstring::size_type pos )
{
    if( pos == std::wstring::npos ) {
        return std::wstring();
    }

    std::wstring::size_type ppos = s.rfind( '\n', pos );

    if( ppos == std::string::npos ) {
        ppos = 0;
    } else {
        ppos++;
    }

    std::wstring::size_type p = s.find( '\n', ppos );
    std::wstring ret = s.substr( ppos, p - ppos );

    // std::cout << "ppos: " << ppos << " p: " << p << " >" << ret << "< " << std::endl;

    return ret;
}


