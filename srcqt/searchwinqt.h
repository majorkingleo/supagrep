/*
 * searchwinqt.h
 *
 *  Created on: 28.02.2023
 *      Author: martin
 */

#ifndef SRCQT_SEARCHWINQT_H_
#define SRCQT_SEARCHWINQT_H_

#include <qwidget.h>
#include <qlineedit.h>

class DescComboQt;
class MainWindowQt;

class SearchWinQt : public QWidget
{
	Q_OBJECT
private:

	MainWindowQt *main;

	QLineEdit *start_directory;
	DescComboQt *search_file_pattern;

public:
	explicit SearchWinQt( MainWindowQt *main, QWidget *parent = 0);

private slots:
	void selectDirectory();
};


#endif /* SRCQT_SEARCHWINQT_H_ */
