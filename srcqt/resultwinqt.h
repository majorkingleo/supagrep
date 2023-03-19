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
	MainWindowQt *main;

public:
	ResultWinQt( MainWindowQt *main, QWidget *parent = 0 );

	void append( const std::wstring & path, bool use_icon, void *address ) override;
	void append( const std::vector<std::wstring> & paths, bool use_icon, void *address ) override;
	void clear() override;

protected:
	std::wstring highLightKeyWord( const std::wstring & line ) override;

	std::wstring hightLightFileNameAndLine( const std::wstring & file_name, long line_number ) override;
};



#endif /* SRCQT_RESULTWINQT_H_ */
