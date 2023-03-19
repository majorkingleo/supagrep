/*
 * ResultWinCommon.cc
 *
 *  Created on: 19.03.2023
 *      Author: martin
 */
#include <debug.h>
#include "ResultWinCommon.h"
#include <read_file.h>
#include <getline.h>

using namespace Tools;

ResultWinCommon::ResultWinCommon()
: results()
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

	DEBUG( wformat( L"Line: '%s'", line ) );

	return line;
}

