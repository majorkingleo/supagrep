/*
 * mainqt.cc
 *
 *  Created on: 28.02.2023
 *      Author: martin
 */
#include "mainqt.h"
#include <QtWidgets>
#include "searchwinqt.h"
#include <arg.h>
#include <iostream>
#include <debug.h>
#include "OutDebug.h"

using namespace Tools;

MainWindowQt::MainWindowQt( int argc, char **argv, QWidget *parent)
: QMainWindow( parent ),
  lang(),
  setup( &lang )
{
	QAction *actionNewSearch;
	QAction *actionQuit;

	actionNewSearch = new QAction(this);
	actionNewSearch->setObjectName( u8"actionNewSearch" );
	actionNewSearch->setText( u8"New Search" );

	actionQuit = new QAction(this);
	actionQuit->setObjectName( u8"actionQuit" );
	actionQuit->setText( u8"Quit") ;

	QMenuBar *menuBar = new QMenuBar(this);
	QMenu *menuGame = new QMenu(menuBar);
	menuGame->setTitle( u8"Program" );
	setMenuBar(menuBar);

	menuBar->addAction(menuGame->menuAction());

	menuGame->addAction(actionNewSearch);
	menuGame->addAction(actionQuit);

	new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_T), this, SLOT(newSearch()));

	connect(actionQuit, SIGNAL (triggered()), QApplication::instance(), SLOT (quit()));
	connect(actionNewSearch, SIGNAL (triggered()), this, SLOT (newSearch()));


    auto *f = new QFrame();
    setCentralWidget(f);

    tabs = new QTabWidget();
    tabs->setTabsClosable(true);
    tabs->setMovable(true);

    connect( tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeSearch(int)) );

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(tabs);

    f->setLayout(mainLayout);

    if( argc > 1 ) {
    	Search::Config conf = Search::getConfFromCommandLine( argc, argv );

    	if( !conf.search.empty() ) {
    		newSearch( &conf );
    	} else {
    		newSearch();
    	}
    } else {
    	newSearch();
    }
}

void MainWindowQt::newSearch()
{
	newSearch( nullptr );
}

void MainWindowQt::newSearch( const Search::Config *conf )
{
	static int idx;
	idx++;
	SearchWinQt *w = new SearchWinQt(this);
	int tabidx = tabs->addTab( w, QString(u8"Search %1").arg(idx) );
	tabs->setCurrentIndex( tabidx );
	w->setTabIdx( tabidx );

	if( conf ) {
		w->startwith( *conf );
	}
}

void MainWindowQt::closeSearch(int idx)
{
	tabs->removeTab( idx );
}

void MainWindowQt::setTabTitle( int idx, const QString & text )
{
	tabs->setTabText( idx, text );
}

std::wstring MainWindowQt::wLC( const std::wstring & s )
{
	return lang.wLC( s );
}

const char* MainWindowQt::LC( const char * msg )
{
	return lang.LC( msg );
}

static void usage( const std::string & prog )
{
  std::cerr << "usage: "
			<< prog << " SEARCH PATTERN\n";
}


int main(int argc, char **argv)
{
	setlocale( LC_ALL, "" );

	Arg::Arg arg( argc, argv );

	arg.addPrefix( "-" );
	arg.addPrefix( "--" );

	Arg::OptionChain oc_info;
	arg.addChainR( &oc_info );
	oc_info.setMinMatch( 1 );
	oc_info.setContinueOnMatch( false );
	oc_info.setContinueOnFail( true );

	Arg::FlagOption o_help( "h" );
	o_help.addName( "help" );
	o_help.setDescription( "Show this page" );
	oc_info.addOptionR( &o_help );

	Arg::FlagOption o_debug("debug");
	o_debug.setDescription("print debugging messages");
	o_debug.setRequired(false);
	arg.addOptionR( &o_debug );

	arg.parse();

	if( o_help.getState() )
	{
		usage(argv[0]);
		std::cout << arg.getHelp(5,20,30, 80 ) << std::endl;
		return 0;
	}

	if( o_debug.getState() )
	{
		Tools::x_debug = new OutDebug();
	}

	QApplication app (argc, argv);

	MainWindowQt mainwindow( argc, argv);

	mainwindow.resize(800,300);
	mainwindow.move(200,200);
	mainwindow.show();

	return app.exec();
}
