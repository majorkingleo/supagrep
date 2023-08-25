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
#include <CpputilsDebug.h>
#include "OutDebug.h"
#include <getline.h>
#include <QSettings>
#include "AsyncOutDebug.h"
#include <chrono>

using namespace std::chrono_literals;
using namespace Tools;

MainWindowQt::MainWindowQt( int argc, char **argv, QWidget *parent)
: QMainWindow( parent ),
  lang(),
  setup( &lang )
{
	QSettings settings;
	QAction *actionNewSearch;
	QAction *actionQuit;

	actionNewSearch = new QAction(this);
	actionNewSearch->setObjectName( u8"actionNewSearch" );
	actionNewSearch->setText( wLCQ(L"&New Search") );

	actionQuit = new QAction(this);
	actionQuit->setObjectName( u8"actionQuit" );
	actionQuit->setText( wLCQ(L"&Quit") ) ;

	QMenuBar *menuBar = new QMenuBar(this);
	QMenu *menuProgram = new QMenu(menuBar);
	menuProgram->setTitle( wLCQ(L"&Program") );
	setMenuBar(menuBar);

	menuBar->addAction(menuProgram->menuAction());

	menuProgram->addAction(actionNewSearch);
	menuProgram->addAction(actionQuit);


	QMenu *menuSettings = new QMenu(menuBar);
	menuSettings->setTitle( wLCQ(L"&Settings") );

	menuBar->addAction(menuSettings->menuAction());


	QAction *actionEditors = new QAction(this);
	actionEditors->setObjectName( u8"actionEditors" );
	actionEditors->setText( wLCQ(L"&Editors") ) ;

	menuSettings->addAction(actionEditors);


	QAction *actionFont = new QAction(this);
	actionFont->setObjectName( u8"actionFont" );
	actionFont->setText( wLCQ(L"&Font") ) ;

	menuSettings->addAction(actionFont);

	new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_T), this, SLOT(newSearch()));

	connect(actionQuit, SIGNAL (triggered()), QApplication::instance(), SLOT (quit()));
	connect(actionNewSearch, SIGNAL (triggered()), this, SLOT (newSearch()));
	connect(actionEditors, SIGNAL (triggered()), this, SLOT (settingsEditors()));
	connect(actionFont, SIGNAL (triggered()), this, SLOT (settingsFont()));


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

    restoreGeometry(settings.value(CONFIG_MAIN_WIN_GEOMETRY).toByteArray());
}

MainWindowQt::~MainWindowQt()
{
	QSettings settings;
	settings.setValue(CONFIG_MAIN_WIN_GEOMETRY, saveGeometry() );
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

QString MainWindowQt::wLCQ( const std::wstring & s )
{
	return QString::fromStdWString( lang.wLC( s ) );
}

void MainWindowQt::settingsEditors()
{

}

void MainWindowQt::settingsFont()
{

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
#ifdef _WIN32
		AllocConsole();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif
		// Tools::x_debug = new OutDebug();

		auto log_frontend = new AsyncOut::Debug();

		Tools::x_debug = log_frontend;

		std::thread( []( auto log_frontend ) {
			AsyncOut::Logger backend;
			log_frontend->subscribe(&backend);

			while( true ) {
				backend.log();
				backend.wait();
				//std::this_thread::sleep_for(200ms);
			}
		}, log_frontend ).detach();
	}

	QApplication app (argc, argv);

	auto font = app.font();
	font.setPointSize(10);
	app.setFont(font);

	QCoreApplication::setOrganizationName("RedeyeLabs");
	QCoreApplication::setOrganizationDomain("borger.co.at");
	QCoreApplication::setApplicationName("Supagrep");

	MainWindowQt mainwindow( argc, argv);

	//mainwindow.move(200,200);
	mainwindow.show();

	int ret = 0;

	try {
		ret = app.exec();

	} catch( const std::exception & error ) {
		std::wcerr << L"Error: " << error.what() << std::endl;
#ifdef _WIN32
		system("pause");
#endif
	} catch( ... ) {
		std::wcerr << L"UnknownError" << std::endl;
#ifdef _WIN32
		system("pause");
#endif
	}

	return ret;
}
