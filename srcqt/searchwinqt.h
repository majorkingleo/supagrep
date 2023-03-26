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
class QTimer;
class ResultWinQt;
class QSpinBox;

class SearchWinQt : public QWidget
{
	Q_OBJECT
private:

	MainWindowQt *main;

	QComboBox 	*cb_start_directory;
	DescComboQt *cb_search_file_pattern;
	QLineEdit 	*ef_search_term;
	QCheckBox 	*cx_icase;
	QCheckBox 	*cx_regex;
	QPushButton *bt_search;
	QTimer 		*timer;
	QPushButton	*bt_search_dir;
	ResultWinQt *result;
	QSpinBox	*bt_number_of_lines;

	Search::MTAccess<bool> mt_running;
	Search::MTAccess<int>  mt_status;
	Search::MTAccess<int>  mt_status_max;
	Search::MTAccess<bool> mt_stop;
	Search::MTAccess<std::list<Search::Result> > mt_result;
	Search::MTAccess<std::chrono::milliseconds> mt_runtime;

	std::shared_ptr<Search::Config> config;
	int tabidx = 0;

public:
	explicit SearchWinQt( MainWindowQt *main, QWidget *parent = 0);
	~SearchWinQt();

	void startwith( const Search::Config & conf );

	 void setTabIdx( int idx ) {
		 tabidx = idx;
	 }

	 std::wstring wLC( const std::wstring & s );
	 const char* LC( const char * msg );

signals:
	void StartSearchNow();

private slots:
	void selectDirectory();
	void onSearch();
	void onTimeout();
	void onVisibleLinesChanged( int value );
};


#endif /* SRCQT_SEARCHWINQT_H_ */
