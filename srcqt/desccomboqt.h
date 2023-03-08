/*
 * desccomboqt.h
 *
 *  Created on: 08.03.2023
 *      Author: martin
 */

#ifndef SRCQT_DESCCOMBOQT_H_
#define SRCQT_DESCCOMBOQT_H_

#include <qcombobox.h>

class DescComboQt : public QComboBox
{
	Q_OBJECT
private:

public:
	explicit DescComboQt(QWidget *parent = 0);

private slots:
	void setFilePatternAsText(int index);
};

#endif /* SRCQT_DESCCOMBOQT_H_ */
