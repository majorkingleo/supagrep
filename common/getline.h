#ifndef GETLINE_H
#define GETLINE_H

#include <string>
#include <ranges>
#include <iostream>

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
	if( s[pos] == L'\n' ) {
		pos++;
	}

	auto p = s.begin() + pos;

	for( ; p != s.end() && count > 0; ) {
		auto pp = std::ranges::find( p, s.end(), L'\n' );

		if( pp == s.end() ) {
			return p;
		}

		p = ++pp;
		std::wcout << L"p: " << *p << std::endl;
		count--;
	}

	return p;
}

template <class view>
auto find_prev_x_elements( const view & s, const wchar_t what, std::wstring::size_type pos, int count )
{
	std::ranges::reverse_view vs(s);
	auto p = find_next_x_elements( vs, what, s.size() - pos, count );

	std::wcout << L"s.begin(): " << *s.begin() << L" distance: " << std::ranges::distance( vs.begin(), p ) << std::endl;
	std::wcout << L"end: " << *(s.begin() + std::ranges::distance( vs.begin(), p )) << std::endl;
	auto end = s.begin() + std::ranges::distance( vs.begin(), p );
	return end;
}

#endif
