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
#include <search.h>

class DescComboQt;
class MainWindowQt;
class QCheckBox;
class QComboBox;
class QPushButton;

class SearchWinQt : public QWidget
{
	Q_OBJECT
private:

	MainWindowQt *main;

	QComboBox *cb_start_directory;
	DescComboQt *cb_search_file_pattern;
	QLineEdit *ef_search_term;
	QCheckBox *cx_icase;
	QCheckBox *cx_regex;
	QPushButton *bt_search;

public:
	explicit SearchWinQt( MainWindowQt *main, QWidget *parent = 0);

	void startwith( const Search::Config & conf );

signals:
	void StartSearchNow();

private slots:
	void selectDirectory();
	void onSearch();
};


#endif /* SRCQT_SEARCHWINQT_H_ */
