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

	start_directory = new QLineEdit();
	setupLayout->addWidget( start_directory );

	auto *button_search_dir = new QPushButton( u8"Search Directory");
	connect( button_search_dir, SIGNAL(pressed()), this, SLOT(selectDirectory()) );
	setupLayout->addWidget( button_search_dir );

	setupLayout->addWidget( new QLabel( u8"Files:" ) );

	search_file_pattern = new DescComboQt();
	setupLayout->addWidget( search_file_pattern );

	setupLayout->addWidget( new QLabel( u8"Search criterias:" ) );
	setupLayout->addWidget( new QCheckBox( u8"Case sensitive" ) );
	setupLayout->addWidget( new QCheckBox( u8"Regular expresssion" ) );

	setupLayout->addWidget( new HSeparatorQt() );

	setupLayout->addWidget( new QLabel( u8"Search:" ) );
	setupLayout->addWidget( new QLineEdit() );
	setupLayout->addWidget( new QPushButton( u8"Start" ) );

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
		search_file_pattern->addItem( QString::fromStdWString(it->descr), QVariant::fromValue(&it->entry) );
	}
}

void SearchWinQt::selectDirectory()
{
	auto text = QFileDialog::getExistingDirectory( this,
									   u8"Choose the directory to start",
									   start_directory->text() );

	start_directory->setText( text );
}
