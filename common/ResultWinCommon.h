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

public:
	  ResultWinCommon();

	 std::wstring getLineAtPos( const std::filesystem::path & file, long pos, int lines );
};

#endif /* COMMON_RESULTWINCOMMON_H_ */
