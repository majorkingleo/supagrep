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

class MainWindowQt : public QMainWindow
{
	 Q_OBJECT
private:
	 QTabWidget *tabs;

public:
	 explicit MainWindowQt(QWidget *parent = 0);

private slots:
	 void newSearch();
	 void closeSearch(int idx);
};

#endif /* SRCQT_MAINQT_H_ */
