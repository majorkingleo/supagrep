/*
 * resultwinqt.cc
 *
 *  Created on: 12.03.2023
 *      Author: martin
 */
#ifdef WIN32
#include <windows.h>
# include <tchar.h>
#endif

#include "resultwinqt.h"
#include "mainqt.h"
#include <format.h>
#include <qlabel.h>
#include <debug.h>
#include <qmenu.h>
#include <QContextMenuEvent>

using namespace Tools;

ResultWinQt::ResultWinQt( MainWindowQt *main_, QWidget *parent )
 : QListWidget( parent ),
   ResultWinCommon(),
   main( main_ )
{
	actionOpenWidthDefaultApp = new QAction(this);
	actionOpenWidthDefaultApp->setObjectName( u8"actionOpenWidthDefaultApp" );
	actionOpenWidthDefaultApp->setText( u8"Open with default App" );

	connect(actionOpenWidthDefaultApp, SIGNAL (triggered()), this, SLOT (openWidthDefaultApp()));
}

void ResultWinQt::append( const std::wstring & path, bool use_icon, Search::ResultEntry *address )
{
	QLabel *label = new QLabel( QString::fromStdWString( path ) );
	QListWidgetItem *item = new QListWidgetItem();
	item->setData( Qt::UserRole, QVariant::fromValue( address ) );
	addItem( item );
	setItemWidget( item, label );
}

void ResultWinQt::append( const std::vector<std::wstring> & paths, bool use_icon, Search::ResultEntry *address )
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

std::wstring ResultWinQt::highLightKeyWord( const std::wstring & line )
{
	return line;
}

void ResultWinQt::contextMenuEvent(QContextMenuEvent *event)
{
	auto item = currentItem();
	QVariant var = item->data( Qt::UserRole );
	Search::ResultEntry *res = var.value<Search::ResultEntry*>();

	DEBUG( wformat( L"%s: Path: %s", __FUNCTION__, res->result.file ) );


    QMenu menu(this);
    //menu.setContextMenuPolicy(Qt::CustomContextMenu);
    menu.addAction(actionOpenWidthDefaultApp);
    menu.exec(event->globalPos());
}

void ResultWinQt::openWidthDefaultApp()
{
  Search::Result result = getCurrentSelecteResult();

#ifdef WIN32
  ShellExecuteW( NULL, L"open", result.file.wstring().c_str(), L"", L"", SW_SHOWNORMAL );
#else
# warning TODO on Unix
#endif
}


Search::Result ResultWinQt::getCurrentSelecteResult()
{
	auto item = currentItem();
	QVariant var = item->data( Qt::UserRole );
	Search::ResultEntry *res = var.value<Search::ResultEntry*>();

	return res->result;
}
