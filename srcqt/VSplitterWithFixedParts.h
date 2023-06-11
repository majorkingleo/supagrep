/**
 * class for splitter widget.
 * The first element wont stretch when enlarged
 * Works only with 2 widgets
 *
 * based on a horizontal splitter of Jesse Ray Adkins [https://stackoverflow.com/users/2888563/jesse-ray-adkins]
 * https://stackoverflow.com/a/19416352/20079418
 *
 * @author Copyright (c) 2023 Martin Oberzalek
 */

#ifndef SRCQT_VSPLITTERWITHFIXEDPARTS_H_
#define SRCQT_VSPLITTERWITHFIXEDPARTS_H_

#include <QSplitter>

class VSplitterWithFixedParts : public QSplitter
{
	Q_OBJECT;

public:
	explicit VSplitterWithFixedParts(QWidget* parent = nullptr);

	void resizeEvent(QResizeEvent *event);
};



#endif /* SRCQT_HSPLITTERWITHFIXEDPARTS_H_ */
