/*
 * resultwinqt.cc
 *
 *  Created on: 12.03.2023
 *      Author: martin
 */
#include "resultwinqt.h"
#include "mainqt.h"
#include <format.h>
#include <qlabel.h>

using namespace Tools;

ResultWinQt::ResultWinQt( MainWindowQt *main_, QWidget *parent )
 : QListWidget( parent ),
   ResultWinCommon(),
   main( main_ )
{

}

void ResultWinQt::append( const std::wstring & path, bool use_icon, void *address )
{
	QLabel *label = new QLabel( QString::fromStdWString( path ) );
	QListWidgetItem *item = new QListWidgetItem();
	item->setData( Qt::UserRole, QVariant::fromValue( address ) );
	addItem( item );
	setItemWidget( item, label );
}

void ResultWinQt::append( const std::vector<std::wstring> & paths, bool use_icon, void *address )
{
	for( const auto & path : paths ) {
		append( path, use_icon, address );
	}
}

void ResultWinQt::clear()
{
	ResultWinCommon::clear();
	QListWidget::clear();
}

std::wstring ResultWinQt::hightLightFileNameAndLine( const std::wstring & file_name, long line_number )
{
	return  wformat( L"<small>%s:%ld</small>", file_name, line_number );
}
