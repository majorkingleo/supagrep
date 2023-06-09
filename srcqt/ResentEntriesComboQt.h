/**
 * Combobox that automatically saves last entries
 * @author Copyright (c) 2023 Martin Oberzalek
 */
#ifndef SRCQT_RESENTENTRIESCOMBOQT_H_
#define SRCQT_RESENTENTRIESCOMBOQT_H_

#include <QComboBox>

class ResentEntriesComboQt : public QComboBox
{
	Q_OBJECT
private:
	std::wstring name;
	unsigned max_entries;

public:
	explicit ResentEntriesComboQt(const std::wstring & name, unsigned max_entries = 10, QWidget *parent = 0);

	void insertItemAtFirst( const QString &text);

private:

	std::wstring getKeyName( int index );
	void load();
};


#endif /* SRCQT_RESENTENTRIESCOMBOQT_H_ */
