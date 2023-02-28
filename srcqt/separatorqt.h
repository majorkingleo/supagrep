/*
 * separatorqt.h
 *
 *  Created on: 28.02.2023
 *      Author: martin
 */

#ifndef SRCQT_SEPARATORQT_H_
#define SRCQT_SEPARATORQT_H_

#include <qframe.h>

class HSeparatorQt : public QFrame
{
	Q_OBJECT

public:
	explicit HSeparatorQt(QWidget *parent = 0);
};

#endif /* SRCQT_SEPARATORQT_H_ */
