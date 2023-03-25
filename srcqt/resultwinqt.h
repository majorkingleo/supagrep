/*
 * resultwinqt.h
 *
 *  Created on: 12.03.2023
 *      Author: martin
 */
#include <qlistwidget.h>
#include <search.h>
#include <ResultWinCommon.h>

#ifndef SRCQT_RESULTWINQT_H_
#define SRCQT_RESULTWINQT_H_

class MainWindowQt;

class ResultWinQt : public QListWidget, public ResultWinCommon
{
	Q_OBJECT

private:
	struct Cmd
	{
		QAction *action;
		std::wstring name;
		std::wstring exec;
		std::wstring open_cmd;

		Cmd(	const std::wstring & name_,
				const std::wstring & exec_,
				const std::wstring & open_cmd_ )
		: action(0),
		  name( name_ ),
		  exec( exec_ ),
		  open_cmd( open_cmd_ )
		{}
	};

private:
	MainWindowQt *main;
	QAction *actionOpenWidthDefaultApp;
	QAction *actionCopyFileNameToClipboard;
	std::list<Cmd> cmds;

public:
	ResultWinQt( MainWindowQt *main, QWidget *parent = 0 );

	void append( const std::wstring & path, bool use_icon, Search::ResultEntry *address ) override;
	void append( const std::vector<std::wstring> & paths, bool use_icon, Search::ResultEntry *address ) override;
	void clear() override;

protected:
	std::wstring highLightKeyWord( const std::wstring & line ) override;

	std::wstring hightLightFileNameAndLine( const std::wstring & file_name, long line_number ) override;

	void contextMenuEvent(QContextMenuEvent *event) override;

	Search::Result getCurrentSelecteResult();

	void addCmd( const Cmd & cmd );

	std::wstring wLC( const std::wstring & s );
	const char* LC( const char * msg );

private slots:
	void openWidthDefaultApp();
	void copyFileNameToClipboard();
	void openWidthCmd();
};



#endif /* SRCQT_RESULTWINQT_H_ */
