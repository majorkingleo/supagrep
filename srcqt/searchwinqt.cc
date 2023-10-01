/*
 * searchwinqt.cc
 *
 *  Created on: 28.02.2023
 *      Author: martin
 */
#include "searchwinqt.h"
#include "separatorqt.h"
#include "desccomboqt.h"
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QFileDialog>
#include <QTimer>
#include <QSplitter>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QLabel>
#include <thread>
#include "mainqt.h"
#include <format.h>
#include <CpputilsDebug.h>
#include "resultwinqt.h"
#include "ResentEntriesComboQt.h"
#include <QSettings>
#include "VSplitterWithFixedParts.h"

using namespace Tools;

SearchWinQt::SearchWinQt( MainWindowQt *main_, QWidget *parent )
: QWidget(parent),
  WdgCommon(main_),
  config(0),
  tabidx(0)
{
	QSettings settings;
	QFrame *setupFrame = new QFrame();
	//setupFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	QVBoxLayout *setupLayout = new QVBoxLayout();
	setupFrame->setLayout( setupLayout );
	setupLayout->addWidget( new QLabel( wLCQ(L"Path:") ) );

	cb_start_directory = new ResentEntriesComboQt( L"cb_start_directory" );
	cb_start_directory->setEditable( true );
	// connect( cb_start_directory, SIGNAL( returnPressed() ),  this, SLOT(onSearch()) );
	setupLayout->addWidget( cb_start_directory );

	bt_search_dir = new QPushButton( wLCQ( L"Search Directory") );
	connect( bt_search_dir, SIGNAL(pressed()), this, SLOT(selectDirectory()) );
	setupLayout->addWidget( bt_search_dir );

	setupLayout->addWidget( new QLabel( wLCQ(L"Files:") ) );

	cb_search_file_pattern = new DescComboQt();
	connect( cb_search_file_pattern, SIGNAL( returnPressed() ),  this, SLOT(onSearch()) );
	setupLayout->addWidget( cb_search_file_pattern );

	setupLayout->addWidget( new QLabel( wLCQ( L"Search criterias:")  ) );

	cx_icase = new QCheckBox( wLCQ( L"Case Insensitive" ) );
	setupLayout->addWidget( cx_icase );

	cx_regex = new QCheckBox( wLCQ( L"Regular Expresssion")  );
	setupLayout->addWidget( cx_regex );

	setupLayout->addWidget( new HSeparatorQt() );

	setupLayout->addWidget( new QLabel( wLCQ( L"Search:") ) );

	ef_search_term = new ResentEntriesComboQt( L"ef_search_term" );
	connect( ef_search_term, SIGNAL( returnPressed() ),  this, SLOT(onSearch()) );
	setupLayout->addWidget( ef_search_term );

	bt_search = new QPushButton( wLCQ( L"Start" ) );
	connect( bt_search, SIGNAL(pressed()), this, SLOT(onSearch()) );
	connect( this, SIGNAL(StartSearchNow()), this, SLOT(onSearch()) );
	setupLayout->addWidget( bt_search );

	setupLayout->addSpacing(10);

	setupLayout->addWidget( new HSeparatorQt() );
	setupLayout->addWidget( new QLabel( wLCQ( L"Result Options:") ) );

	setupLayout->addWidget( new QLabel( QString::fromStdWString(wLC( L"Number of lines\naround the keyword" ) )) );
	bt_number_of_lines = new QSpinBox();
	bt_number_of_lines->setMinimum( 1 );
	bt_number_of_lines->setMaximum( 99 );
	bt_number_of_lines->setSingleStep( 2 );
	connect( bt_number_of_lines, SIGNAL( valueChanged(int) ), this, SLOT(onVisibleLinesChanged(int)) );
	setupLayout->addWidget( bt_number_of_lines );

	setupLayout->addWidget( new HSeparatorQt() );

	cx_highlight = new QCheckBox( wLCQ( L"Highlight Keyword" )  );
	setupLayout->addWidget( cx_highlight );

	setupLayout->addWidget( new HSeparatorQt() );

	setupLayout->addWidget( new QLabel( wLCQ( L"State:" ) ) );
	pb_progress = new QProgressBar();
	setupLayout->addWidget( pb_progress );

	l_runtime = new QLabel();
	l_runtime->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	setupLayout->addWidget( l_runtime );

	splitter = new VSplitterWithFixedParts();
	splitter->addWidget( setupFrame );

	result = new ResultWinQt( main );
	splitter->addWidget( result );
	splitter->setCollapsible(0, false);
	splitter->setCollapsible(1, false);
	splitter->restoreState(settings.value(CONFIG_SEARCH_WIN_SPLITTER_SIZE).toByteArray());

	connect( cx_highlight, SIGNAL( stateChanged(int) ), result, SLOT( hightLightKeyword(int) ) );
	cx_highlight->setCheckState( Qt::Checked );

	QHBoxLayout *mainLayout = new QHBoxLayout();
	mainLayout->addWidget( splitter );
	setLayout(mainLayout);

	/* add File pattern */
	for( Setup::config_pair_list_it it = main->getSetup().config.file_pattern.begin();
			it != main->getSetup().config.file_pattern.end();
			it++ )
	{
		CPPDEBUG( wformat( L"descr: '%s'; pattern: '%s'", it->descr, it->entry) );
		cb_search_file_pattern->addItem( QString::fromStdWString(it->descr), QVariant::fromValue(&it->entry) );
	}

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
	timer->start(300);
}

SearchWinQt::~SearchWinQt()
{
	timer->stop();
	QSettings settings;
	settings.setValue(CONFIG_SEARCH_WIN_SPLITTER_SIZE,splitter->saveState());
}

void SearchWinQt::selectDirectory()
{
	auto text = QFileDialog::getExistingDirectory( this,
									   u8"Choose the directory to start",
									   cb_start_directory->currentText() );

	cb_start_directory->setEditText( text );
	cb_start_directory->insertItemAtFirst( text );
}

void SearchWinQt::startwith( const Search::Config & conf )
{
	ef_search_term->lineEdit()->setText( QString::fromStdWString(conf.search) );
	cx_icase->setCheckState( conf.icase ? Qt::Checked : Qt::Unchecked );
	cx_regex->setCheckState( conf.regex ? Qt::Checked : Qt::Unchecked );
	cb_search_file_pattern->setEditText( QString::fromStdWString( conf.pattern ) );
	cb_start_directory->setEditText( QString::fromStdWString( conf.path ) );
	cb_start_directory->insertItemAtFirst( QString::fromStdWString( conf.path ) );

	emit StartSearchNow();
}

void SearchWinQt::onSearch()
{
	CPPDEBUG( "onSearch()" );

	if( config ) {
		if( mt_running.get() == true ) {
			mt_stop.set( true );
			return;
		}
	}

	config = std::make_shared<Search::Config>();

	mt_result.access().clear();
	mt_result.free();

	mt_running.set(false);
	mt_status.set(0);
	mt_status.clear();
	mt_stop.set(false);
	mt_status_max.set(0);
	mt_status_max.clear();

	config->mt_running = &mt_running;
	config->mt_status = &mt_status;
	config->mt_status_max = &mt_status_max;
	config->mt_stop = &mt_stop;
	config->mt_result = &mt_result;
	config->mt_runtime = &mt_runtime;

	if( cb_start_directory->currentText().isEmpty() ) {

		auto path = QString::fromStdWString(std::filesystem::current_path().wstring());

		cb_start_directory->setEditText( path );
		cb_start_directory->insertItemAtFirst( path );
	}

	if( cb_search_file_pattern->currentText().isEmpty() ) {
		cb_search_file_pattern->setCurrentIndex(2);
	}

	config->regex = cx_regex->checkState() == Qt::Checked ? true : false;
	config->icase = cx_icase->checkState() == Qt::Checked ? true : false;
	config->pattern = cb_search_file_pattern->currentText().toStdWString();
	config->path =  cb_start_directory->currentText().toStdWString();
	config->search =  ef_search_term->lineEdit()->text().toStdWString();

	result->setConfig( config );
	result->clear();

	std::thread search_thread( [this]{
		Search s( config );
		mt_running.set(true);
		s.run();
	} );

	search_thread.detach();

	main->setTabTitle( tabidx, QString::fromStdWString(config->search) );
}


void SearchWinQt::onTimeout()
{
	if( mt_running.get() ) {
		cb_start_directory->setEnabled(false);
		cb_search_file_pattern->setEnabled(false);
		cx_regex->setEnabled(false);
		cx_icase->setEnabled(false);
		bt_search_dir->setEnabled(false);
		ef_search_term->setEnabled(false);

		bt_search->setText( u8"Stop" );

	} else {
		cb_start_directory->setEnabled(true);
		cb_search_file_pattern->setEnabled(true);
		cx_regex->setEnabled(true);
		cx_icase->setEnabled(true);
		bt_search_dir->setEnabled(true);
		ef_search_term->setEnabled(true);

		bt_search->setText( u8"Go" );


		if( mt_status_max.changed() ) {
			pb_progress->setMaximum(mt_status_max.getAndClear());
			pb_progress->setValue(mt_status_max.getAndClear());
		}
	}

	if( mt_status_max.changed() ) {
		pb_progress->setMaximum(mt_status_max.getAndClear());
	}

	if( mt_status.changed() ) {
		pb_progress->setValue(mt_status.getAndClear());
	}

	if( mt_result.changed() ) {
		CPPDEBUG( L"trying to get result" );
		std::list<Search::Result> & l = mt_result.access();
		CPPDEBUG( wformat( L"got access appending %d data", l.size() ) );

		for( std::list<Search::Result>::iterator it = l.begin(); it != l.end(); it++ ) {
			result->appendResult( *it, config->path );
			CPPDEBUG( wformat( L"Result: %s", it->file.wstring() ) );
		}

		l.clear();

		mt_result.clear_and_free();
	}

	if( mt_runtime.changed() ) {
		using namespace std::chrono_literals;

		auto runtime = mt_runtime.getAndClear();
		double seconds = runtime / 1.0s;
		CPPDEBUG( format( "%3.3lf sec", seconds ) );
		l_runtime->setText( QString::fromStdWString( wformat( wLC(L"%3.3lf sec"), seconds ) ) );
	}
}

void SearchWinQt::onVisibleLinesChanged( int value )
{
	result->setVisibleLines( value );
}

