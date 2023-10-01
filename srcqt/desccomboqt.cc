/**
 * ComboBox with description
 * @author Copyright (c) 2023 Martin Oberzalek
 */
#include "desccomboqt.h"
#include <CpputilsDebug.h>
#include <format.h>
#include <QLineEdit>

using namespace Tools;

DescComboQt::DescComboQt(QWidget *parent)
: QComboBox(parent)
{
	setEditable(true);
	connect( this, SIGNAL(activated(int)), this, SLOT(setFilePatternAsText(int)) );
	connect( this, SIGNAL(currentIndexChanged(int)),this,SLOT(setFilePatternAsText(int)));
	connect( lineEdit(), SIGNAL(returnPressed()), this,SLOT(editFinished()));
}

void DescComboQt::setFilePatternAsText(int index)
{
	CPPDEBUG( wformat(L"activated: %d", index) );

	if( index >= 0 ) {
		QVariant qv = itemData( index );
		std::wstring *s = qv.value<std::wstring*>();

		if( s ) {
			CPPDEBUG( wformat( L"search Text: %s", *s ) );
			setEditText( QString::fromStdWString(*s) );
		}
	}
}

void DescComboQt::editFinished()
{
	emit returnPressed();
}

