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
#include <QClipboard>
#include <QGuiApplication>
#include <debug.h>
#include <QStandardPaths>
#include <QProcess>
#include <string_utils.h>
#include <HighLightKeyWord.h>

using namespace Tools;

ResultWinQt::ResultWinQt( MainWindowQt *main_, QWidget *parent )
 : QListWidget( parent ),
   ResultWinCommon(),
   WdgCommon(main_),
   highlight_keyword( false )
{
	actionOpenWidthDefaultApp = new QAction(this);
	actionOpenWidthDefaultApp->setObjectName( u8"actionOpenWidthDefaultApp" );
	actionOpenWidthDefaultApp->setText( u8"Open with default App" );

	actionCopyFileNameToClipboard = new QAction(this);
	actionCopyFileNameToClipboard->setObjectName( u8"actionCopyFileNameToClipboard" );
	actionCopyFileNameToClipboard->setText( u8"Copy Filename" );

	addCmd( Cmd( wformat( wLC( L"Open File with %s" ), wLC( L"GVim" ) ),
			L"gvim", L"+%d %s" ) );
	addCmd( Cmd( wformat( wLC( L"Open File with %s" ), wLC( L"KVim" ) ),
			L"kvim", L"+%d %s" ) );
	addCmd( Cmd( wformat( wLC( L"Open File with %s" ), wLC( L"Emacs" ) ),
			L"emacs", L"+%d %s" ) );
	addCmd( Cmd( wformat( wLC( L"Open File with %s" ), wLC( L"Emacs Client" ) ),
			L"emacsclient", L"+%d %s" ) );
	addCmd( Cmd( wformat( wLC( L"Open File with %s" ), wLC( L"XEmacs" ) ),
			L"xemacs", L"+%d %s" ) );
	addCmd( Cmd( wformat( wLC( L"Open File with %s" ), wLC( L"XEmacs Client" ) ),
			L"xemacsclient", L"+%d %s" ) );
	addCmd( Cmd( wformat( wLC( L"Open File with %s" ), wLC( L"Kate" ) ),
			L"kate", L"--line %d %s" ) );
	addCmd( Cmd( wformat( wLC( L"Open File with %s" ), wLC( L"Adie" ) ),
			L"adie", L"--line %d %s" ) );
	addCmd( Cmd( wformat( wLC( L"Open File with %s" ), wLC( L"Adie" ) ),
			L"adie.exe", L"--line %d %s" ) );
	addCmd( Cmd( wformat( wLC( L"Open File with %s" ), wLC( L"Kwrite" ) ),
			L"kwrite", L"--line %d %s" ) );
	addCmd( Cmd( wformat( wLC( L"Open File with %s" ), wLC( L"WinVi" ) ),
			L"Winvi32.exe", L"+%d %s" ) );
	addCmd( Cmd( wformat( wLC( L"Open File with %s" ), wLC( L"Notepad++" ) ),
			L"notepad++.exe", L"-n%d %s" ) );

	addCmd( Cmd( wformat( wLC( L"Open File with %s" ), wLC( L"Vi" ) ),
			L"vi.exe", L"+%d %s" ) );


	addCmd( Cmd( wformat( wLC( L"Start %s" ), wLC( L"XTerm" ) ),
			L"xterm" ) );

	addCmd( Cmd( wformat( wLC( L"Start %s" ), wLC( L"Konsole" ) ),
			L"konsole" ) );

	addCmd( Cmd( wformat( wLC( L"Start %s" ), wLC( L"Gnome Terminal" ) ),
			L"gnome-terminal" ) );

	addCmd( Cmd( wformat( wLC( L"Start %s" ), wLC( L"Console" ) ),
			L"cmd.exe" ) );

	addCmd( Cmd( wformat( wLC( L"Open File with %s" ), wLC( L"Eclipse" ) ),
			L"eclipse.exe", L"--launcher.openFile %s:%d" ) );

	addCmd( Cmd( wformat( wLC( L"Start %s" ), wLC( L"Notepad" ) ),
			L"notepad.exe" ) );

	// DEBUG( wformat( L"notpaged.exe: %s", QStandardPaths::findExecutable("notepad" ).toStdWString() ) );

	connect(actionOpenWidthDefaultApp, SIGNAL (triggered()), this, SLOT (openWidthDefaultApp()));
	connect(actionCopyFileNameToClipboard, SIGNAL (triggered()), this, SLOT (copyFileNameToClipboard()));
}

void ResultWinQt::addCmd( const Cmd & cmd )
{
  if( QStandardPaths::findExecutable( QString::fromStdWString( cmd.exec ) ).isEmpty() ) {
	  return;
  }

  cmds.push_back( cmd );
  Cmd *cmd_ptr = &(*cmds.rbegin());

  QAction *action = new QAction( this );
  cmd_ptr->action = action;
  action->setObjectName( QString::fromStdWString(cmd_ptr->exec) );
  action->setText( QString::fromStdWString( cmd_ptr->name ) );

  // DEBUG( wformat( L"adding editor app: %s", cmd_ptr->name ) );

  action->setData( QVariant::fromValue( cmd_ptr ) );
  connect(action, SIGNAL (triggered()), this, SLOT (openWidthCmd()));
};


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
	if( line_number >= 0 ) {
		return  wformat( L"<small>%s:%ld</small>", file_name, line_number );
	}

	return file_name;
}


std::wstring ResultWinQt::highLightKeyWord( const std::wstring & line )
{
	if( highlight_keyword ) {
		return ::highLightKeyWord( line, config->search, config->icase );
	}

	return encodeHtmlEntities(line);
}

void ResultWinQt::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(actionCopyFileNameToClipboard);
    menu.addAction(actionOpenWidthDefaultApp);

    for( Cmd & cmd : cmds ) {
    	menu.addAction( cmd.action );
    }

    menu.exec(event->globalPos());
}

void ResultWinQt::openWidthDefaultApp()
{
  Search::Result result = getCurrentSelectedResult();

#ifdef WIN32
  ShellExecuteW( NULL, L"open", result.file.wstring().c_str(), L"", L"", SW_SHOWNORMAL );
#else
# warning TODO on Unix
#endif
}


Search::Result ResultWinQt::getCurrentSelectedResult()
{
	auto item = currentItem();
	QVariant var = item->data( Qt::UserRole );
	Search::ResultEntry *res = var.value<Search::ResultEntry*>();

	return res->result;
}

void ResultWinQt::copyFileNameToClipboard()
{
	Search::Result result = getCurrentSelectedResult();

	QClipboard *clipboard = QGuiApplication::clipboard();
	clipboard->setText( QString::fromStdWString( result.file.wstring() ) );
}

void ResultWinQt::openWidthCmd()
{
	QAction *qaction = qobject_cast<QAction*>(sender());
	QVariant var = qaction->data();
	Cmd *cmd = var.value<Cmd*>();

	Search::Result result = getCurrentSelectedResult();

	std::vector<std::wstring> sArgs = split_and_strip_simple( cmd->open_cmd );

	QStringList args;
	std::wstring debug_args;

	for( std::wstring & s : sArgs ) {
		s = substitude( s, L"%d", std::to_wstring( result.line) );
		s = substitude( s, L"%s", result.file.wstring() );

		debug_args += s;
		args << QString::fromStdWString( s );
	}

	DEBUG( wformat( L"Open with: %s %s", cmd->exec, debug_args ) );

	QProcess::startDetached( QString::fromStdWString( cmd->exec ), args );
}

void ResultWinQt::setVisibleLines( int vl )
{
  view_lines = vl;
  QListWidget::clear();

  for( result_list::iterator it = results.begin();
	   it != results.end(); it++ )
	{
	  appendResult( it->result, it->path, false, &(*it) );
	}
}

void ResultWinQt::hightLightKeyword( int state )
{
  bool highlight_keyword_new = false;
  if( state == Qt::Checked ) {
	  highlight_keyword_new = true;
  }

  if( highlight_keyword == highlight_keyword_new ) {
	  return;
  }

  highlight_keyword = highlight_keyword_new;

  QListWidget::clear();

  for( result_list::iterator it = results.begin();
	   it != results.end(); it++ )
	{
	  appendResult( it->result, it->path, false, &(*it) );
	}
}
