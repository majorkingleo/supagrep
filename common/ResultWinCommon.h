/*
 * ResultWinCommon.h
 *
 *  Created on: 19.03.2023
 *      Author: martin
 */

#ifndef COMMON_RESULTWINCOMMON_H_
#define COMMON_RESULTWINCOMMON_H_

#include <string>
#include <filesystem>
#include "search.h"

class ResultWinCommon
{
protected:
	using Entry = Search::ResultEntry;
	typedef std::list<Entry> result_list;
	result_list results;
	int view_lines;

public:
	ResultWinCommon();
	virtual ~ResultWinCommon();

	std::wstring getLineAtPos( const std::filesystem::path & file, long pos, int lines );
	void appendResult( const Search::Result & result, const std::wstring & path, bool do_append = true, void *address = NULL );

	virtual void append( const std::wstring & path, bool use_icon, void *address ) = 0;
	virtual void append( const std::vector<std::wstring> & paths, bool use_icon, void *address ) = 0;

	virtual void clear();

protected:
	virtual std::wstring hightLightFileNameAndLine( const std::wstring & file_name, long line_number );

	virtual std::wstring highLightKeyWord( const std::wstring & line ) {
		return line;
	}
};

#endif /* COMMON_RESULTWINCOMMON_H_ */
