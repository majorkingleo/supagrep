/**
 * class for markup with HTML entities
 * @author Copyright (c) 2023 Martin Oberzalek
 */
#ifndef COMMON_HIGHLIGHTKEYWORD_H_
#define COMMON_HIGHLIGHTKEYWORD_H_

#include <string>

std::wstring highLightKeyWord( const std::wstring & line,
							   const std::wstring & search_term,
							   bool ignore_case = false );


std::wstring encodeHtmlEntities( const std::wstring & line );

#endif /* COMMON_HIGHLIGHTKEYWORD_H_ */
