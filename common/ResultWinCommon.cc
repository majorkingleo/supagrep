/*
 * ResultWinCommon.cc
 *
 *  Created on: 19.03.2023
 *      Author: martin
 */
#include <debug.h>
#include "ResultWinCommon.h"
#include <read_file.h>
#include "getline.h"
#include <string_utils.h>

using namespace Tools;

ResultWinCommon::ResultWinCommon()
: results(),
  view_lines(1)
{

}

ResultWinCommon::~ResultWinCommon()
{

}

std::wstring ResultWinCommon::getLineAtPos( const std::filesystem::path & file, long pos, int lines )
{
	ReadFile read;

	std::wstring content;

	if( !read.read_file( file.string(), content ) ) {
		DEBUG( wformat( L"Cannot read file '%s'", file.wstring() ));
		return content;
	}

	std::wstring line = get_whole_line( content, pos );

	int lines_before_and_after = lines / 2;

	if( lines_before_and_after ) {
		line = get_lines_before_line_at_pos( content, pos, lines_before_and_after ) + L'\n' + line;
		line += L'\n' + get_lines_after_line_at_pos( content, pos, lines_before_and_after );
	}

	DEBUG( wformat( L"Line: '%s'", line ) );

	return line;
}

void ResultWinCommon::appendResult( const Search::Result & result, const std::wstring &path, bool do_append, Search::ResultEntry *address )
{
	if( do_append ) {
		results.push_back( Search::ResultEntry( result, path ) );
		address = &(*results.rbegin());
	}

	auto file = result.file;
	std::wstring line = getLineAtPos( result.file, result.pos, view_lines );

	line = substitude( line, L"\t", L" " );
	line = substitude( line, L"\r", L" " );

	std::wstring file_name = file.wstring();

	if( file_name.find( path ) == 0 ) {
		file_name = file_name.substr( path.length() );
	}

	if( line.find( L'\n' ) == std::wstring::npos ) {
		line = strip( line );
		line = highLightKeyWord( line );
		append( hightLightFileNameAndLine( file_name, result.line ) + L" " + line, false, address );
	} else {
		append( hightLightFileNameAndLine( file_name, result.line), true, address );

		std::vector<std::wstring> sl = split_simple( line, L"\n" );

		for( std::wstring & s : sl ) {
			s = highLightKeyWord( s );
		}

		append( sl, false, address );
	}
}

std::wstring ResultWinCommon::hightLightFileNameAndLine( const std::wstring & file_name, long line_number )
{
	return  wformat( L"%s:%ld", file_name, line_number );
}

void ResultWinCommon::clear()
{
	results.clear();
}
