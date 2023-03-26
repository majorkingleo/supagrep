/*
 * ColBuilder.cc
 *
 *  Created on: 27.01.2023
 *      Author: Martin Oberzalek <oberzalek@gmx.at>
 */

#include <sstream>
#include <iostream>
#include "ColBuilder.h"


int ColBuilder::addCol( const std::string & name )
{
	int idx = getColByName( name );

	if( idx >= 0 )
		return idx;

	col_headers.push_back( name );
	cols.resize( col_headers.size() );

	return static_cast<int>(col_headers.size()-1);
}

void ColBuilder::addColData( int idx, const std::string & data )
{
	cols.at(idx).push_back( data );
}

void ColBuilder::addColData( const std::string & name, const std::string & data )
{
	int idx = getColByName( name );

	addColData( idx, data );
}

bool ColBuilder::haveCol( const std::string & name ) const
{
	bool found = false;

	for( unsigned i = 0; i < col_headers.size(); i++ )
	{
		if( col_headers[i] == name )
		{
			found = true;
			break;
		}
	}

	return found;
}

/**
 * returns -1 on error
 */
int ColBuilder::getColByName( const std::string & name ) const
{
	for( unsigned i = 0; i < col_headers.size(); i++ ) {
		if( col_headers[i] == name ) {
			return i;
		}
	}

	return -1;
}

int ColBuilder::getMaxNumOfRows() const
{
	int max_rows = 0;

	for( unsigned i = 0; i < cols.size(); i++ )
	{
		if( max_rows < static_cast<int>(cols[i].size()) )
			max_rows = cols[i].size();
	}

	return max_rows;
}

std::string ColBuilder::strip_escape_sequences( const std::string & str )
{
	std::string::size_type start_pos = str.find("\033[");

	if( start_pos == std::string::npos ) {
		return str;
	}

	std::string::size_type end_pos = str.find( "m", start_pos );

	if( end_pos == std::string::npos ) {
		return str;
	}

	std::string s;

	if( start_pos != 0 ) {
		s = str.substr( 0, start_pos );
	}

	s += str.substr( end_pos + 1 );

	std::string::size_type end_of_sequence = s.find( "\033[0m" );

	if( end_of_sequence == std::string::npos ) {
		return s;
	}

	s = s.substr( 0, end_of_sequence );

	return strip_escape_sequences( s );
}

unsigned int ColBuilder::count_visible_size( const std::string & str )
{
	std::string s = strip_escape_sequences( str );
	return s.size();
}

int ColBuilder::getColWidth( int idx ) const
{
	int max_width = 0;

	for( unsigned i = 0; i < cols.at(idx).size(); i++ ) {

		unsigned col_size = count_visible_size( cols.at(idx)[i] );

		if( max_width < static_cast<int>(col_size) ) {
			max_width = col_size;
		}
	}

	if( max_width < static_cast<int>(col_headers.at(idx).size()) ) {
		max_width = col_headers.at(idx).size();
	}

	return max_width;
}

std::string ColBuilder::toString() const
{
	std::stringstream str;

	std::string header;

	// build Header
	for( unsigned i = 0; i < col_headers.size(); i++ ) {
		if( i > 0 ) {
			header += ' ';
		}

		header += fill_leading( col_headers[i], " ", getColWidth(i));
	}

	if( margin_top ) {
		str << fill_leading( "", "\n", margin_top );
	}

	if( margin_left ) {
		str << fill_leading( "", " ", margin_left );
	}

	str << header;
	str << "\n";

	if( margin_left ) {
		str << fill_leading( "", " ", margin_left );
	}

	unsigned width = header.size();

	if( width < fill_bar_to_width_of ) {
		width = fill_bar_to_width_of - margin_left;
	}

	str << fill_leading( "", "_", width);
	str << "\n";

	unsigned max_rows = getMaxNumOfRows();

	for( unsigned i = 0; i < max_rows; i++  )
	{
		if( margin_left ) {
			str << fill_leading( "", " ", margin_left );
		}

		for( unsigned j = 0; j < cols.size(); j++ )
		{
			if( j > 0 ) {
				str << ' ';
			}

			if( cols.at(j).size() <= i ) {
				str << fill_leading( "", " ",  getColWidth(j));
			} else {
				str << fill_leading( cols.at(j).at(i) , " ",  getColWidth(j));
			}
		}

		str << '\n';
	}

	return str.str();
}

std::string ColBuilder::fill_leading( std::string s, const std::string fill_sign, unsigned int len )
{
	s.reserve(len);

	while( s.size() < len ) {
		s = fill_sign + s;
	}

	return s;
}

