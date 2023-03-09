/*
 * searchwinqt.cc
 *
 *  Created on: 28.02.2023
 *      Author: martin
 */
#include "searchwinqt.h"
#include "separatorqt.h"
#include "desccomboqt.h"
#include <QtWidgets>
#include "mainqt.h"
#include <format.h>
#include <debug.h>

using namespace Tools;

SearchWinQt::SearchWinQt( MainWindowQt *main_, QWidget *parent )
: main( main_ ),
  QWidget(parent)
{
	QFrame *setupFrame = new QFrame();
	//setupFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	QVBoxLayout *setupLayout = new QVBoxLayout();
	setupFrame->setLayout( setupLayout );
	setupLayout->addWidget( new QLabel( u8"Path:" ) );

	cb_start_directory = new QComboBox();
	cb_start_directory->setEditable( true );
	setupLayout->addWidget( cb_start_directory );

	auto *bt_search_dir = new QPushButton( u8"Search Directory");
	connect( bt_search_dir, SIGNAL(pressed()), this, SLOT(selectDirectory()) );
	setupLayout->addWidget( bt_search_dir );

	setupLayout->addWidget( new QLabel( u8"Files:" ) );

	cb_search_file_pattern = new DescComboQt();
	setupLayout->addWidget( cb_search_file_pattern );

	setupLayout->addWidget( new QLabel( u8"Search criterias:" ) );

	cx_icase = new QCheckBox( u8"Case sensitive" );
	setupLayout->addWidget( cx_icase );

	cx_regex = new QCheckBox( u8"Regular expresssion" );
	setupLayout->addWidget( cx_regex );

	setupLayout->addWidget( new HSeparatorQt() );

	setupLayout->addWidget( new QLabel( u8"Search:" ) );

	ef_search_term = new QLineEdit();
	setupLayout->addWidget( ef_search_term );

	bt_search = new QPushButton( u8"Start" );
	connect( bt_search, SIGNAL(pressed()), this, SLOT(onSearch()) );
	connect( this, SIGNAL(StartSearchNow()), this, SLOT(onSearch()) );
	setupLayout->addWidget( bt_search );

	setupLayout->addSpacing(10);

	setupLayout->addWidget( new HSeparatorQt() );
	setupLayout->addWidget( new QLabel( u8"Display options:" ) );

	setupLayout->addWidget( new QCheckBox( u8"Highlight" ) );
	setupLayout->addWidget( new HSeparatorQt() );

	setupLayout->addWidget( new QLabel( u8"State:" ) );
	setupLayout->addWidget( new QProgressBar() );

	QSplitter *splitter = new QSplitter();
	splitter->addWidget( setupFrame );

	QTextEdit *edit = new QTextEdit();
	splitter->addWidget( edit );

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
}
