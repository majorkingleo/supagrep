#ifndef GETLINE_H
#define GETLINE_H

#include <string>
#include <ranges>
#include <iostream>
#include <CpputilsDebug.h>
#include <format.h>

std::wstring getline( const std::wstring & s, std::wstring::size_type pos );

std::wstring::size_type get_pos_for_line( const std::wstring & content, int line );

std::string diff_lines( const std::string & orig, std::string & modded );
std::wstring diff_lines( const std::wstring & orig, std::wstring & modded );

std::wstring get_whole_line( const std::wstring & s, std::wstring::size_type pos );

std::wstring get_lines_before_line_at_pos( const std::wstring & s, std::wstring::size_type pos, int lines );
std::wstring get_lines_after_line_at_pos( const std::wstring & s, std::wstring::size_type pos, int lines );


template <class view>
auto find_next_x_elements( const view & s, const wchar_t what, std::wstring::size_type pos, int count )
{
	if( s[pos] == what ) {
		CPPDEBUG( Tools::wformat( L"found '%c' incrementing pos %d", what, pos ));
		pos++;
	}

	auto p = s.begin() + pos;

	for( ; p != s.end() && count > 0; ) {
		auto pp = std::ranges::find( p, s.end(), what );

		CPPDEBUG( Tools::wformat( L"found: '%s' count: %d", std::wstring( p, pp ), count ) );

		if( pp == s.end() ) {
			CPPDEBUG( Tools::wformat( L"pp == end" ) );
			return s.end();
		}

		p = ++pp;
		// std::wcout << L"p: " << *p << std::endl;
		count--;
	}

	return p;
}

template <class view>
auto find_prev_x_elements( const view & s, const wchar_t what, std::wstring::size_type pos, int count )
{
	std::ranges::reverse_view vs(s);
	auto p = find_next_x_elements( vs, what, s.size() - pos - 1, count );

	auto rbegin = vs.begin() + s.size() - pos - 1;
	std::wstring search_buf( vs.begin() + s.size() - pos - 1, vs.end() );
	std::wstring data_result( rbegin, p );
	CPPDEBUG( Tools::wformat( L"s: '%s' w: '%s' distance: %d",
						   search_buf,
						   data_result,
						   std::ranges::distance( p, vs.end() )) );

	// std::wcout << L"s.begin(): " << *s.begin() << L" distance: " << std::ranges::distance( vs.begin(), p ) << std::endl;
	// std::wcout << L"end: " << *(s.begin() + std::ranges::distance( vs.begin(), p )) << std::endl;
	auto end = s.begin() + std::ranges::distance( p, vs.end() );
	return end;
}

#endif
