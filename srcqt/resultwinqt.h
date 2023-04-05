/*
 * resultwinqt.h
 *
 *  Created on: 12.03.2023
 *      Author: martin
 */
#ifndef SRCQT_RESULTWINQT_H_
#define SRCQT_RESULTWINQT_H_

#include <qlistwidget.h>
#include <search.h>
#include <ResultWinCommon.h>
#include "WdgCommon.h"

class MainWindowQt;

class ResultWinQt : public QListWidget, public ResultWinCommon, public WdgCommon
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
				const std::wstring & open_cmd_ = std::wstring() )
		: action(0),
		  name( name_ ),
		  exec( exec_ ),
		  open_cmd( open_cmd_ )
		{}
	};

private:
	QAction *actionOpenWidthDefaultApp;
	QAction *actionCopyFileNameToClipboard;
	std::list<Cmd> cmds;
	bool highlight_keyword;
	std::shared_ptr<Search::Config> config;

public:
	ResultWinQt( MainWindowQt *main, QWidget *parent = 0 );

	void append( const std::wstring & path, bool use_icon, Search::ResultEntry *address ) override;
	void append( const std::vector<std::wstring> & paths, bool use_icon, Search::ResultEntry *address ) override;
	void clear() override;

	void setVisibleLines( int vl );
	void setConfig( std::shared_ptr<Search::Config> config_ ) {
		config = config_;
	}

protected:
	std::wstring highLightKeyWord( const std::wstring & line ) override;

	std::wstring hightLightFileNameAndLine( const std::wstring & file_name, long line_number ) override;

	void contextMenuEvent(QContextMenuEvent *event) override;

	Search::Result getCurrentSelectedResult();

	void addCmd( const Cmd & cmd );

public slots:
	void hightLightKeyword( int state );


private slots:
	void openWidthDefaultApp();
	void copyFileNameToClipboard();
	void openWidthCmd();
};



#endif /* SRCQT_RESULTWINQT_H_ */
