/**
 * class for splitter widget.
 * The first element wont stretch when enlarged
 * @author Copyright (c) 2023 Martin Oberzalek
 */

#include "VSplitterWithFixedParts.h"
#include <QResizeEvent>


VSplitterWithFixedParts::VSplitterWithFixedParts(QWidget* parent)
: QSplitter(parent)
{

}

// based on a horizontal splitter of Jesse Ray Adkins [https://stackoverflow.com/users/2888563/jesse-ray-adkins]
// https://stackoverflow.com/a/19416352/20079418

void VSplitterWithFixedParts::resizeEvent(QResizeEvent *event)
{
	/* The first resizeEvent is -1 for height and width, because it was
       invisible before. */
	if (event->oldSize().width() != -1) {
		int diff;
		QList<int> previousSizes = sizes();
		QSplitter::resizeEvent(event);
		QList<int> newSizes = sizes();
		/* The bottom widget is the fixed one, so find out if that is to
           grow or shrink. */
		diff = newSizes[1] - previousSizes[1];
		if (diff > 0) {
			/* Keep the left from growing by giving the size to the right. */
			newSizes[0] -= diff;
			newSizes[1] += diff;
		}
		else {
			/* Steal size from the right to keep it from shrinking. */
			newSizes[0] += diff;
			newSizes[1] -= diff;
		}
		setSizes(newSizes);
	}
	else
	{
		QSplitter::resizeEvent(event);
	}
}
