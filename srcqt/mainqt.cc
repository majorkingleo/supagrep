/*
 * mainqt.cc
 *
 *  Created on: 28.02.2023
 *      Author: martin
 */
#include "mainqt.h"
#include <QtWidgets>
#include "searchwinqt.h"

MainWindowQt::MainWindowQt(QWidget *parent)
: QMainWindow( parent )
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

    newSearch();
}

void MainWindowQt::newSearch()
{
	static int idx;
	idx++;
	int tabidx = tabs->addTab( new SearchWinQt(), QString(u8"Search %1").arg(idx) );
	tabs->setCurrentIndex( tabidx );
}

void MainWindowQt::closeSearch(int idx)
{
	tabs->removeTab( idx );
}

int main(int argc, char **argv)
{
	QApplication app (argc, argv);

	MainWindowQt mainwindow;

	mainwindow.resize(300,300);
	mainwindow.move(200,200);
	mainwindow.show();

	return app.exec();
}
