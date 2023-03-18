#ifndef GETLINE_H
#define GETLINE_H

#include <string>

std::wstring getline( const std::wstring & s, std::wstring::size_type pos );

std::wstring::size_type get_pos_for_line( const std::wstring & content, int line );

std::string diff_lines( const std::string & orig, std::string & modded );
std::wstring diff_lines( const std::wstring & orig, std::wstring & modded );

std::wstring get_whole_line( const std::wstring & s, std::wstring::size_type pos );

#endif
