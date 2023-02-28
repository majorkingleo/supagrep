/*
 * searchwinqt.cc
 *
 *  Created on: 28.02.2023
 *      Author: martin
 */
#include "searchwinqt.h"
#include "separatorqt.h"
#include <QtWidgets>

SearchWinQt::SearchWinQt( QWidget *parent )
: QWidget(parent)
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

	search_file_pattern = new QLineEdit();
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
}

void SearchWinQt::selectDirectory()
{
	auto text = QFileDialog::getExistingDirectory( this,
									   u8"Choose the directory to start",
									   start_directory->text() );

	start_directory->setText( text );
}
