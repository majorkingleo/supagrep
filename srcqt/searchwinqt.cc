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
#include <debug.h>
#include "resultwinqt.h"

using namespace Tools;

SearchWinQt::SearchWinQt( MainWindowQt *main_, QWidget *parent )
: main( main_ ),
  QWidget(parent),
  config(0),
  tabidx(0)
{
	QFrame *setupFrame = new QFrame();
	//setupFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	QVBoxLayout *setupLayout = new QVBoxLayout();
	setupFrame->setLayout( setupLayout );
	setupLayout->addWidget( new QLabel( u8"Path:" ) );

	cb_start_directory = new QComboBox();
	cb_start_directory->setEditable( true );
	// connect( cb_start_directory, SIGNAL( returnPressed() ),  this, SLOT(onSearch()) );
	setupLayout->addWidget( cb_start_directory );

	bt_search_dir = new QPushButton( u8"Search Directory");
	connect( bt_search_dir, SIGNAL(pressed()), this, SLOT(selectDirectory()) );
	setupLayout->addWidget( bt_search_dir );

	setupLayout->addWidget( new QLabel( u8"Files:" ) );

	cb_search_file_pattern = new DescComboQt();
	connect( cb_search_file_pattern, SIGNAL( returnPressed() ),  this, SLOT(onSearch()) );
	setupLayout->addWidget( cb_search_file_pattern );

	setupLayout->addWidget( new QLabel( u8"Search criterias:" ) );

	cx_icase = new QCheckBox( u8"Case sensitive" );
	setupLayout->addWidget( cx_icase );

	cx_regex = new QCheckBox( u8"Regular expresssion" );
	setupLayout->addWidget( cx_regex );

	setupLayout->addWidget( new HSeparatorQt() );

	setupLayout->addWidget( new QLabel( u8"Search:" ) );

	ef_search_term = new QLineEdit();
	connect( ef_search_term, SIGNAL( returnPressed() ),  this, SLOT(onSearch()) );
	setupLayout->addWidget( ef_search_term );

	bt_search = new QPushButton( u8"Start" );
	connect( bt_search, SIGNAL(pressed()), this, SLOT(onSearch()) );
	connect( this, SIGNAL(StartSearchNow()), this, SLOT(onSearch()) );
	setupLayout->addWidget( bt_search );

	setupLayout->addSpacing(10);

	setupLayout->addWidget( new HSeparatorQt() );
	setupLayout->addWidget( new QLabel( u8"Display options:" ) );

	setupLayout->addWidget( new QLabel( QString::fromStdWString(wLC( L"Number of lines\naround the keyword" ) )) );
	bt_number_of_lines = new QSpinBox();
	bt_number_of_lines->setMinimum( 1 );
	bt_number_of_lines->setMaximum( 99 );
	bt_number_of_lines->setSingleStep( 2 );
	connect( bt_number_of_lines, SIGNAL( valueChanged(int) ), this, SLOT(onVisibleLinesChanged(int)) );
	setupLayout->addWidget( bt_number_of_lines );

	setupLayout->addWidget( new HSeparatorQt() );

	setupLayout->addWidget( new QCheckBox( u8"Highlight" ) );
	setupLayout->addWidget( new HSeparatorQt() );

	setupLayout->addWidget( new QLabel( u8"State:" ) );
	setupLayout->addWidget( new QProgressBar() );

	QSplitter *splitter = new QSplitter();
	splitter->addWidget( setupFrame );

	result = new ResultWinQt( main );
	splitter->addWidget( result );

	QHBoxLayout *mainLayout = new QHBoxLayout();
	mainLayout->addWidget( splitter );
	setLayout(mainLayout);

	/* add File pattern */
	for( Setup::config_pair_list_it it = main->getSetup().config.file_pattern.begin();
			it != main->getSetup().config.file_pattern.end();
			it++ )
	{
		DEBUG( wformat( L"descr: '%s'; pattern: '%s'", it->descr, it->entry) );
		cb_search_file_pattern->addItem( QString::fromStdWString(it->descr), QVariant::fromValue(&it->entry) );
	}

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
	timer->start(300);
}

SearchWinQt::~SearchWinQt()
{
	timer->stop();
}

void SearchWinQt::selectDirectory()
{
	auto text = QFileDialog::getExistingDirectory( this,
									   u8"Choose the directory to start",
									   cb_start_directory->currentText() );

	cb_start_directory->setEditText( text );
}

void SearchWinQt::startwith( const Search::Config & conf )
{
	ef_search_term->setText( QString::fromStdWString(conf.search) );
	cx_icase->setCheckState( conf.icase ? Qt::Checked : Qt::Unchecked );
	cx_regex->setCheckState( conf.regex ? Qt::Checked : Qt::Unchecked );
	cb_search_file_pattern->setEditText( QString::fromStdWString( conf.pattern ) );
	cb_start_directory->setEditText( QString::fromStdWString( conf.path ) );
	cb_start_directory->insertItem( 0, QString::fromStdWString( conf.path ) );

	emit StartSearchNow();
}

void SearchWinQt::onSearch()
{
	DEBUG( "onSearch()" );

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
		cb_start_directory->setEditText( QString::fromStdWString(std::filesystem::current_path().wstring()) );
	}

	if( cb_search_file_pattern->currentText().isEmpty() ) {
		cb_search_file_pattern->setCurrentIndex(2);
	}

	config->regex = cx_regex->checkState() == Qt::Checked ? true : false;
	config->icase = cx_icase->checkState() == Qt::Checked ? true : false;
	config->pattern = cb_search_file_pattern->currentText().toStdWString();
	config->path =  cb_start_directory->currentText().toStdWString();
	config->search =  ef_search_term->text().toStdWString();

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
	// DEBUG( "onTimeout()" );

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

		/*
	pb_state->update();

	pb_state->setTotal(mt_status_max.getAndClear());
	pb_state->setProgress(mt_status_max.getAndClear());
		 */
		// DEBUG( format( "done: max: %d", mt_status_max.getAndClear()) );
	}

	if( mt_status_max.changed() )
	{
		// DEBUG( format( "Max Status: '%d'", mt_status.get() ) );
		// pb_state->setTotal(mt_status_max.getAndClear());
	}

	if( mt_status.changed() )
	{
		// DEBUG( format( "Status: '%d'", mt_status.get() ) );
		// pb_state->setProgress(mt_status.getAndClear());
	}

	if( mt_result.changed() )
	{
		std::list<Search::Result> & l = mt_result.access();

		for( std::list<Search::Result>::iterator it = l.begin(); it != l.end(); it++ )
		{
			result->appendResult( *it, config->path );
			DEBUG( wformat( L"Result: %s", it->file.wstring() ) );
		}

		l.clear();

		mt_result.clear_and_free();
	}

	if( mt_runtime.changed() )
	{
		using namespace std::chrono_literals;

		auto runtime = mt_runtime.getAndClear();
		double seconds = runtime / 1.0s;
		DEBUG( format( "%3.3lf sec", seconds ) );
		// l_runtime->setText( format( "%3.3lf sec", seconds ).c_str() );
	}
}

std::wstring SearchWinQt::wLC( const std::wstring & s )
{
	return main->wLC( s );
}

const char* SearchWinQt::LC( const char  *msg )
{
	return main->LC( msg );
}

void SearchWinQt::onVisibleLinesChanged( int value )
{
	result->setVisibleLines( value );
}
