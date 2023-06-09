/**
 * Combobox that automatically saves last entries
 * @author Copyright (c) 2023 Martin Oberzalek
 */
#include "ResentEntriesComboQt.h"
#include "debug.h"
#include <QSettings>
#include <set>
#include "utf8_util.h"

using namespace Tools;

ResentEntriesComboQt::ResentEntriesComboQt(const std::wstring & name_, unsigned max_entries_, QWidget *parent)
: QComboBox(parent),
  name(name_),
  max_entries(max_entries_)
{

	load();
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
		settings.setValue( getKeyName(count++), val );
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
		auto val = settings.value( getKeyName(i), QString() ).toString();

		if( val.isEmpty() ) {
			continue;
		}

		if( s_entries.count(val) > 0 ) {
			continue;
		}

		// DEBUG( wformat( L"Loading %s=%s", getKeyName(i),  val.toStdWString() ) );

		s_entries.insert(val);

		QComboBox::insertItem( i, val );
	}
}
