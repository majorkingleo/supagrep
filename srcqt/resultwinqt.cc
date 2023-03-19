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

void ResultWinQt::append( const std::wstring & path, bool use_icon, void *address )
{
	QListWidgetItem *item = new QListWidgetItem( QString::fromStdWString( path ) );
	item->setData( Qt::UserRole, QVariant::fromValue( address ) );
	addItem( item );
}

void ResultWinQt::append( const std::vector<std::wstring> & paths, bool use_icon, void *address )
{
	for( const auto & path : paths ) {
		QListWidgetItem *item = new QListWidgetItem( QString::fromStdWString( path ) );
		item->setData( Qt::UserRole, QVariant::fromValue( address ) );
		addItem( item );
	}
}

void ResultWinQt::clear()
{
	ResultWinCommon::clear();
	QListWidget::clear();
}
