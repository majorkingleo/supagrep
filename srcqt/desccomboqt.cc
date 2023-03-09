/*
 * desccomboqt.cc
 *
 *  Created on: 08.03.2023
 *      Author: martin
 */
#include "desccomboqt.h"
#include "debug.h"
#include <format.h>

using namespace Tools;

DescComboQt::DescComboQt(QWidget *parent)
: QComboBox(parent)
{
	setEditable(true);
	connect( this, SIGNAL(activated(int)), this, SLOT(setFilePatternAsText(int)) );
	connect( this, SIGNAL(currentIndexChanged(int)),this,SLOT(setFilePatternAsText(int)));
}

void DescComboQt::setFilePatternAsText(int index)
{
	DEBUG( wformat(L"activated: %d", index) );

	if( index >= 0 ) {
		QVariant qv = itemData( index );
		std::wstring *s = qv.value<std::wstring*>();

		if( s ) {
			DEBUG( wformat( L"search Text: %s", *s ) );
			setEditText( QString::fromStdWString(*s) );
		}
	}
}

