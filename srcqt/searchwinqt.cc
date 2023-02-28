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
	setupLayout->addWidget( new QLineEdit() );
	setupLayout->addWidget( new QPushButton( u8"Search Directory") );

	setupLayout->addWidget( new QLabel( u8"Files:" ) );
	setupLayout->addWidget( new QLineEdit() );

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

