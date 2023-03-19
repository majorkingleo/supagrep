/*
 * resultwinqt.cc
 *
 *  Created on: 12.03.2023
 *      Author: martin
 */
#include "resultwinqt.h"
#include "mainqt.h"

ResultWinQt::ResultWinQt( MainWindowQt *main_, QWidget *parent )
 : QListWidget( parent ),
   ResultWinCommon(),
   main( main_ )
{

}

void ResultWinQt::appendItem( Search::Result & result, const std::wstring & path )
{
	addItem( QString::fromStdWString( path ) );
}


