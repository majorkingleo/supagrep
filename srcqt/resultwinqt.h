/*
 * resultwinqt.h
 *
 *  Created on: 12.03.2023
 *      Author: martin
 */
#include <qlistwidget.h>

#ifndef SRCQT_RESULTWINQT_H_
#define SRCQT_RESULTWINQT_H_

class MainWindowQt;

class ResultWinQt : public QListWidget
{
	Q_OBJECT

private:
	MainWindowQt *main;

public:
	ResultWinQt( MainWindowQt *main, QWidget *parent = 0 );

};



#endif /* SRCQT_RESULTWINQT_H_ */