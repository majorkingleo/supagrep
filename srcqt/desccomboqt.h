/*
/**
 * ComboBox with description
 * @author Copyright (c) 2023 Martin Oberzalek
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

signals:
	void returnPressed();

private slots:
	void setFilePatternAsText(int index);
	void editFinished();
};

#endif /* SRCQT_DESCCOMBOQT_H_ */
