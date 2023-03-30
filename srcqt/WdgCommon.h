/*
 * WdgCommon.h
 *
 *  Created on: 30.03.2023
 *      Author: martin
 */

#ifndef SRCQT_WDGCOMMON_H_
#define SRCQT_WDGCOMMON_H_

#include <QString>

class MainWindowQt;

class WdgCommon
{
protected:
	MainWindowQt *main;

public:
	WdgCommon( MainWindowQt *main );

public:

	 std::wstring wLC( const std::wstring & s );
	 const char* LC( const char * msg );
	 QString wLCQ( const std::wstring & s );
};



#endif /* SRCQT_WDGCOMMON_H_ */
