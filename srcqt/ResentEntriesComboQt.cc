/**
 * Combobox that automatically saves last entries
 * @author Copyright (c) 2023 Martin Oberzalek
 */
#include "ResentEntriesComboQt.h"
#include <CpputilsDebug.h>
#include <QSettings>
#include <QLineEdit>
#include <set>
#include "utf8_util.h"
#include <format.h>

using namespace Tools;

ResentEntriesComboQt::ResentEntriesComboQt(const std::wstring & name_, unsigned max_entries_, QWidget *parent)
: QComboBox(parent),
  name(name_),
  max_entries(max_entries_)
{
	load();

	setEditable(true);
	connect( lineEdit(), &QLineEdit::returnPressed, this, &ResentEntriesComboQt::editFinished);
}

ResentEntriesComboQt::~ResentEntriesComboQt()
{
	QSettings settings;

	for( unsigned index = 0; index < count(); index++ ) {
		auto val = itemText(index);
		settings.setValue( QString::fromWCharArray(getKeyName(index).c_str()), val );
	}
}

void ResentEntriesComboQt::insertItemAtFirst( const QString &text)
{
	QSettings settings;

	// move stored entries to higher places
	std::set<QString> s_entries;
	std::list<QString> entries_to_store;

	for( unsigned index = 0; index < count(); index++ ) {
		auto val = itemText(index);

		if( s_entries.count(val) == 0 ) {
			 s_entries.insert( val );
			 entries_to_store.push_back(val);
		}
	}


	if( s_entries.count(text) == 0 ) {
		 s_entries.insert( text );
		 entries_to_store.push_front( text );
	}

	insertItem( 0, text );

	unsigned count = 0;
	for( auto val : entries_to_store ) {
		settings.setValue( QString::fromWCharArray(getKeyName(count++).c_str()), val );
		// DEBUG( wformat( L"%s=%s", getKeyName(count),  val.toStdWString() ) );

		if( count > max_entries ) {
			break;
		}
	}
}

std::wstring ResentEntriesComboQt::getKeyName( int index )
{
	return wformat( L"ResentEntriesComboQt/%s/%d", name, index );
}

void ResentEntriesComboQt::load()
{
	QSettings settings;
	std::set<QString> s_entries;

	for( unsigned i = 0; i < max_entries; i++ )
	{
		auto val = settings.value( QString::fromWCharArray(getKeyName(i).c_str()), QString() ).toString();

		if( val.isEmpty() ) {
			continue;
		}

		if( s_entries.count(val) > 0 ) {
			continue;
		}

		CPPDEBUG( wformat( L"Loading %s=%s", getKeyName(i),  val.toStdWString() ) );

		s_entries.insert(val);

		QComboBox::insertItem( i, val );
	}
}

void ResentEntriesComboQt::editFinished()
{
	CPPDEBUG( L"return pressed" );
	emit returnPressed();
}
