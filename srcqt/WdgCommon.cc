/*
 * WdgCommon.cc
 *
 *  Created on: 30.03.2023
 *      Author: martin
 */

#include "WdgCommon.h"
#include "mainqt.h"

WdgCommon::WdgCommon( MainWindowQt *main_ )
: main( main_ )
{

}

std::wstring WdgCommon::wLC( const std::wstring & s )
{
	return main->wLC( s );
}

const char* WdgCommon::LC( const char  *msg )
{
	return main->LC( msg );
}

QString WdgCommon::wLCQ( const std::wstring & s )
{
	return main->wLCQ( s );
}
