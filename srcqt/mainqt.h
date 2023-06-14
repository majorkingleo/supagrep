/*
 * mainqt.h
 *
 *  Created on: 28.02.2023
 *      Author: martin
 */

#ifndef SRCQT_MAINQT_H_
#define SRCQT_MAINQT_H_

#include <qmainwindow.h>
#include <qtabwidget.h>
#include "setup.h"
#include "lang.h"
#include "search.h"

class MainWindowQt : public QMainWindow
{
	 Q_OBJECT
private:
	 static constexpr const char* CONFIG_MAIN_WIN_GEOMETRY = "MainWin/Geometry";

	 QTabWidget *tabs;

	  Lang lang;
	  Setup setup;

public:
	 explicit MainWindowQt( int argc, char **argv, QWidget *parent = 0);
	 ~MainWindowQt();

	 Setup & getSetup() { return setup; }

	 void setTabTitle( int idx, const QString & text );

	 std::wstring wLC( const std::wstring & s );
	 const char* LC( const char * msg );
	 QString wLCQ( const std::wstring & s );

private slots:
	 void newSearch();
	 void closeSearch(int idx);
	 void settingsEditors();
	 void settingsFont();

private:
	 void newSearch( const Search::Config *conf );
};

#endif /* SRCQT_MAINQT_H_ */
