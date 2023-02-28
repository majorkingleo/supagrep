/*
 * separatorqt.cc
 *
 *  Created on: 28.02.2023
 *      Author: martin
 */
#include "separatorqt.h"

HSeparatorQt::HSeparatorQt( QWidget *parent )
: QFrame( parent )
{
	setFrameShape(QFrame::HLine);
	setFrameShadow(QFrame::Sunken);
}



