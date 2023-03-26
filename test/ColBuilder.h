/*
 * ColBuilder.h
 *
 *  Created on: 27.01.2023
 *      Author: Martin Oberzalek <oberzalek@gmx.at>
 */

#ifndef TEST_COL_BUILDER_H
#define TEST_COL_BUILDER_H

#include <vector>
#include <string>

/*
 * ASCII art class for displaying data in rows and cols
 */
class ColBuilder
{
	std::vector<std::string> col_headers;
	std::vector< std::vector<std::string> > cols;
	unsigned margin_left;
	unsigned margin_top;
	unsigned fill_bar_to_width_of;

public:
	ColBuilder(unsigned margin_left_ = 0, unsigned margin_top_ = 0 )
	: col_headers(), cols(),
	  margin_left(margin_left_),
	  margin_top(margin_top_),
	  fill_bar_to_width_of(0)
	{}

	void setFillBarToWidthOf( unsigned width )
	{
		fill_bar_to_width_of = width;
	}

	/**
	 * returns the index of the col
	 */
	int addCol( const std::string & name );

	void addColData( int idx, const std::string & data );

	void addColData( const std::string & name, const std::string & data );

	bool haveCol( const std::string & name ) const;

	/**
	 * returns -1 on error
	 */
	int getColByName( const std::string & name ) const;

	int getNumOfCols() const
	{
		return col_headers.size();
	}

	int getMaxNumOfRows() const;

	int getColWidth( int idx ) const;

	std::string toString() const;

protected:
	static std::string fill_leading( std::string s, const std::string fill_sign, unsigned int len );
	static std::string strip_escape_sequences( const std::string & str );
	static unsigned int count_visible_size( const std::string & str );

};

#endif  /* TEST_COL_BUILDER_H */
