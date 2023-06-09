/**
 * class for markup with HTML entities
 * @author Copyright (c) 2023 Martin Oberzalek
 */
#include "HighLightKeyWord.h"
#include "debug.h"
#include <string_utils.h>

using namespace Tools;

class HtmlOrPlainText
{
public:
	struct HTML
	{
		std::wstring text;
	};

	struct PLAIN
	{
		std::wstring text;
	};

private:
	std::wstring plain_text;
	std::wstring html_text;
	bool html_text_encoded;

public:
	HtmlOrPlainText( const PLAIN & plain )
	: plain_text( plain.text ),
	  html_text_encoded( false )
	{}

	HtmlOrPlainText( const HTML & html )
	: html_text( html.text ),
	  html_text_encoded( true )
	{}

	const std::wstring & getPlainText() const {
		return plain_text;
	}

	std::wstring & getPlainText() {
		return plain_text;
	}

	std::wstring & getHtmlText()
	{
		if( !html_text_encoded ) {
			html_text = encodeHtmlEntities( plain_text );
			html_text_encoded = true;
		}

		return html_text;
	}

private:
};

std::wstring encodeHtmlEntities( const std::wstring & line )
{
	auto res = substitude( line, L"<", L"&lt;" );
	res = substitude( res, L">", L"&gt;" );

	return res;
}

std::wstring highLightKeyWord( const std::wstring & line, const std::wstring & search_term, bool ignore_case )
{
	// DEBUG( line );

	using POSITIONS = std::vector<std::wstring::size_type>;
	POSITIONS positions;

	if( ignore_case ) {
		const std::wstring search_term_upper_case = toupper( search_term );
		const std::wstring line_upper_case = toupper( line );

		positions = find_all_of<std::vector<std::wstring::size_type>>( line_upper_case, search_term_upper_case );
	} else {
		positions = find_all_of<std::vector<std::wstring::size_type>>( line, search_term );
	}

	std::wstring result_line = line;

	std::wstring::size_type pos_offset = 0;
	static const std::wstring HTML_B_OPEN = L"<b>";
	static const std::wstring HTML_B_CLOSE = L"</b>";

	std::vector<HtmlOrPlainText> parts;
	std::wstring post;

	for( auto pos : positions ) {
		pos -= pos_offset;
		std::wstring pre = result_line.substr( 0, pos );
		std::wstring word = result_line.substr( pos, search_term.size() );
		post = result_line.substr( pos + search_term.size() );

		parts.push_back( HtmlOrPlainText(HtmlOrPlainText::PLAIN{ pre }) );
		parts.push_back( HtmlOrPlainText(HtmlOrPlainText::HTML{ HTML_B_OPEN }) );
		parts.push_back( HtmlOrPlainText(HtmlOrPlainText::PLAIN{ word }) );
		parts.push_back( HtmlOrPlainText(HtmlOrPlainText::HTML{ HTML_B_CLOSE }) );

		result_line = post;
		pos_offset += pos + word.size();
		// DEBUG( result_line );
	}

	parts.push_back( HtmlOrPlainText(HtmlOrPlainText::PLAIN{ post }) );

	result_line.clear();

	for( auto & part : parts ) {
		result_line += part.getHtmlText();
	}

	// DEBUG( result_line );

	return result_line;
}


