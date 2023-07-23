#include <memory>
#include <ColoredOutput.h>
#include <CpputilsDebug.h>
#include "ColBuilder.h"
#include <arg.h>
#include <OutDebug.h>
#include <getline.h>
#include <functional>
#include <string_utils.h>
#include <HighLightKeyWord.h>

using namespace Tools;

class TestCaseBase
{
protected:
	std::string name;
	bool throws_exception;

public:
	TestCaseBase( const std::string & name_, bool throws_exception_ = false )
	: name( name_ ),
	  throws_exception( throws_exception_ )
	{}

	virtual ~TestCaseBase() {}

	virtual bool run() = 0;

	const std::string & getName() const {
		return name;
	}

	bool throwsException() const {
		return throws_exception;
	}
};

class TestCaseFindBase : public TestCaseBase
{
protected:
	const std::wstring testdata;

public:
	TestCaseFindBase( const std::string & name )
	: TestCaseBase( name ),
	  testdata (L"aaaa 1\n"
	 			 "bbbb 2\n"
	 			 "cccc 3\n"
	 			 "dddd 4\n"
	 			 "eeee 5\n"
	 			 "ffff 6\n" )
	{

	}

};

class TestCaseFindNext1 : public TestCaseFindBase
{
public:
	TestCaseFindNext1()
	: TestCaseFindBase( "find_next_x_elements() first line" )
	{}

	bool run() override
	{
		auto p = find_next_x_elements( testdata, L'\n', 0, 1 );

		std::wstring data_result( testdata.begin(), p );

		DEBUG( Tools::wformat( L"Result: '%s'", data_result ) );

		if( data_result == L"aaaa 1\n" ) {
			return true;
		}

		return false;
	}
};

class TestCaseFindNext2 : public TestCaseFindBase
{
public:
	TestCaseFindNext2()
	: TestCaseFindBase( "find_next_x_elements() first 2 lines" )
	{}

	bool run() override
	{
		auto p = find_next_x_elements( testdata, L'\n', 0, 2 );

		std::wstring data_result( testdata.begin(), p );

		DEBUG( Tools::wformat( L"Result: '%s'", data_result ) );

		if( data_result == L"aaaa 1\n"
							"bbbb 2\n" ) {
			return true;
		}

		return false;
	}
};

class TestCaseFindNext3 : public TestCaseFindBase
{
public:
	TestCaseFindNext3()
	: TestCaseFindBase( "find_prev_x_elements() first line" )
	{}

	bool run() override
	{
		auto p = find_prev_x_elements( testdata, L'\n', 14, 3 );

		std::wstring data_result( p, testdata.begin() + 14 );

		DEBUG( Tools::wformat( L"Result: '%s'", data_result ) );

		if( data_result == L"aaaa 1\n"
							"bbbb 2\n" ) {
			return true;
		}

		return false;
	}
};

class TestCaseGetLinesBefore1 : public TestCaseFindBase
{
public:
	TestCaseGetLinesBefore1()
	: TestCaseFindBase( "get_lines_before_line_at_pos(1)" )
	{}

	bool run() override
	{
		std::wstring data_result = get_lines_before_line_at_pos( testdata, 9, 3 );

		DEBUG( Tools::wformat( L"Result: '%s'", data_result ) );

		if( data_result == L"aaaa 1\n" ) {
			return true;
		}

		return false;
	}
};

class TestCaseGetLinesBefore2 : public TestCaseFindBase
{
public:
	TestCaseGetLinesBefore2()
	: TestCaseFindBase( "get_lines_before_line_at_pos(2)" )
	{}

	bool run() override
	{
		std::wstring data_result = get_lines_before_line_at_pos( testdata, 13, 1 );

		DEBUG( Tools::wformat( L"Result: '%s'", data_result ) );

		if( data_result == L"aaaa 1\n" ) {
			return true;
		}

		return false;
	}
};


class TestCaseGetLinesBefore3 : public TestCaseFindBase
{
public:
	TestCaseGetLinesBefore3()
	: TestCaseFindBase( "get_lines_before_line_at_pos(3)" )
	{}

	bool run() override
	{
		std::wstring data_result = get_lines_before_line_at_pos( testdata, 6, 1 );

		DEBUG( Tools::wformat( L"Result: '%s'", data_result ) );

		if( data_result.empty() ) {
			return true;
		}

		return false;
	}
};

class TestCaseGetLinesBefore4 : public TestCaseFindBase
{
public:
	TestCaseGetLinesBefore4()
	: TestCaseFindBase( "get_lines_before_line_at_pos(4)" )
	{}

	bool run() override
	{
		std::wstring data_result = get_lines_before_line_at_pos( testdata, 0, 1 );

		DEBUG( Tools::wformat( L"Result: '%s'", data_result ) );

		if( data_result.empty() ) {
			return true;
		}

		return false;
	}
};

class TestCaseGetLinesBefore5 : public TestCaseFindBase
{
public:
	TestCaseGetLinesBefore5()
	: TestCaseFindBase( "get_lines_before_line_at_pos(5)" )
	{}

	bool run() override
	{
		std::wstring data_result = get_lines_before_line_at_pos( testdata, 100, 1 );

		DEBUG( Tools::wformat( L"Result: '%s'", data_result ) );

		if( data_result == L"eeee 5\n" ) {
			return true;
		}

		return false;
	}
};

class TestCaseGetLinesAfter1 : public TestCaseFindBase
{
public:
	TestCaseGetLinesAfter1()
	: TestCaseFindBase( "get_lines_after_line_at_pos(1)" )
	{}

	bool run() override
	{
		std::wstring data_result = get_lines_after_line_at_pos( testdata, 0, 1 );

		DEBUG( Tools::wformat( L"Result: '%s'", data_result ) );

		if( data_result == L"bbbb 2\n" ) {
			return true;
		}

		return false;
	}
};

class TestCaseGetLinesAfter2 : public TestCaseFindBase
{
public:
	TestCaseGetLinesAfter2()
	: TestCaseFindBase( "get_lines_after_line_at_pos(2)" )
	{}

	bool run() override
	{
		std::wstring data_result = get_lines_after_line_at_pos( testdata, 3, 1 );

		DEBUG( Tools::wformat( L"Result: '%s'", data_result ) );

		if( data_result == L"bbbb 2\n" ) {
			return true;
		}

		return false;
	}
};


class TestCaseGetLinesAfter3 : public TestCaseFindBase
{
public:
	TestCaseGetLinesAfter3()
	: TestCaseFindBase( "get_lines_after_line_at_pos(3)" )
	{}

	bool run() override
	{
		std::wstring data_result = get_lines_after_line_at_pos( testdata, 6, 1 );

		DEBUG( Tools::wformat( L"Result: '%s'", data_result ) );

		if( data_result == L"bbbb 2\n" ) {
			return true;
		}

		return false;
	}
};


class TestCaseGetLinesAfter4 : public TestCaseFindBase
{
public:
	TestCaseGetLinesAfter4()
	: TestCaseFindBase( "get_lines_after_line_at_pos(4)" )
	{}

	bool run() override
	{
		std::wstring data_result = get_lines_after_line_at_pos( testdata, 7, 2 );

		DEBUG( Tools::wformat( L"Result: '%s'", data_result ) );

		if( data_result == L"cccc 3\n"
							"dddd 4\n") {
			return true;
		}

		return false;
	}
};

class TestCaseFindAllOfBase : public TestCaseBase
{
protected:
	const std::wstring testdata;

public:
	TestCaseFindAllOfBase( const std::string & name )
	: TestCaseBase( name ),
	  testdata (L"Hi I am Hi Boost Hi Library")
	{

	}

	template<class container>
	std::wstring v2s( const container & data ) {
		return IterableToFormattedWString( data );
	}
};


class TestCaseFindAllOf1 : public TestCaseFindAllOfBase
{
public:
	TestCaseFindAllOf1()
	: TestCaseFindAllOfBase( "find_all_of(1)" )
	{}

	bool run() override
	{
		std::vector<std::wstring::size_type> positions;
		find_all_of( testdata, L"Hi",
				[&positions]( auto pos ) {
					positions.push_back( pos );
					return true;
				} );

		std::wstring data_result = v2s( positions );
		DEBUG( Tools::wformat( L"Result: '%s'", data_result ) );

		if( data_result == L"0, 8, 17") {
			return true;
		}

		auto data2 = find_all_of<std::vector<std::wstring::size_type>>( testdata, L"Hi");

		if( data2 == positions ) {
			return true;
		}

		return false;
	}
};

class TestCaseHighLightKeyWord1 : public TestCaseFindBase
{
public:
	TestCaseHighLightKeyWord1()
	: TestCaseFindBase( "HighLightKeyWord(1)" )
	{}

	bool run() override
	{
		std::wstring data_result = highLightKeyWord( L"#include <debug.h>", L"debug" );

		DEBUG( Tools::wformat( L"Result: '%s'", data_result ) );

		if( data_result == L"#include &lt;<b>debug</b>.h&gt;" ) {
			return true;
		}

		return false;
	}
};

class TestCaseHighLightKeyWord2 : public TestCaseFindBase
{
public:
	TestCaseHighLightKeyWord2()
	: TestCaseFindBase( "HighLightKeyWord(2)" )
	{}

	bool run() override
	{
		std::wstring data_result = highLightKeyWord( L"#include <debug.h>", L"Debug", true );

		DEBUG( Tools::wformat( L"Result: '%s'", data_result ) );

		if( data_result == L"#include &lt;<b>debug</b>.h&gt;" ) {
			return true;
		}

		return false;
	}
};

class TestCaseHighLightKeyWord3 : public TestCaseFindBase
{
public:
	TestCaseHighLightKeyWord3()
	: TestCaseFindBase( "HighLightKeyWord(3)" )
	{}

	bool run() override
	{
		std::wstring data_result = highLightKeyWord( L"#include <debug.h> // include debug", L"debug" );

		DEBUG( Tools::wformat( L"Result: '%s'", data_result ) );

		if( data_result == L"#include &lt;<b>debug</b>.h&gt; // include <b>debug</b>" ) {
			return true;
		}

		return false;
	}
};

class TestCaseHighLightKeyWord4 : public TestCaseFindBase
{
public:
	TestCaseHighLightKeyWord4()
	: TestCaseFindBase( "HighLightKeyWord(4)" )
	{}

	bool run() override
	{
		std::wstring data_result = highLightKeyWord( L"debug", L"debug" );

		DEBUG( Tools::wformat( L"Result: '%s'", data_result ) );

		if( data_result == L"<b>debug</b>" ) {
			return true;
		}

		return false;
	}
};

int main( int argc, char **argv )
{
	ColoredOutput co;

	Arg::Arg arg( argc, argv );
	arg.addPrefix( "-" );
	arg.addPrefix( "--" );

	Arg::OptionChain oc_info;
	arg.addChainR(&oc_info);
	oc_info.setMinMatch(1);
	oc_info.setContinueOnMatch( false );
	oc_info.setContinueOnFail( true );

	Arg::FlagOption o_help( "help" );
	o_help.setDescription( "Show this page" );
	oc_info.addOptionR( &o_help );

	Arg::FlagOption o_debug("d");
	o_debug.addName( "debug" );
	o_debug.setDescription("print debugging messages");
	o_debug.setRequired(false);
	arg.addOptionR( &o_debug );

	if( !arg.parse() )
	{
		std::cout << arg.getHelp(5,20,30, 80 ) << std::endl;
		return 1;
	}

	if( o_debug.getState() )
	{
		Tools::x_debug = new OutDebug();
	}

	if( o_help.getState() ) {
		std::cout << arg.getHelp(5,20,30, 80 ) << std::endl;
		return 1;
	}

	try {
		std::vector<std::shared_ptr<TestCaseBase>> test_cases;

		test_cases.push_back( std::make_shared<TestCaseFindNext1>() );
		test_cases.push_back( std::make_shared<TestCaseFindNext2>() );
		test_cases.push_back( std::make_shared<TestCaseFindNext3>() );
		test_cases.push_back( std::make_shared<TestCaseGetLinesBefore1>() );
		test_cases.push_back( std::make_shared<TestCaseGetLinesBefore2>() );
		test_cases.push_back( std::make_shared<TestCaseGetLinesBefore3>() );
		test_cases.push_back( std::make_shared<TestCaseGetLinesBefore4>() );
		test_cases.push_back( std::make_shared<TestCaseGetLinesBefore5>() );

		test_cases.push_back( std::make_shared<TestCaseGetLinesAfter1>() );
		test_cases.push_back( std::make_shared<TestCaseGetLinesAfter2>() );
		test_cases.push_back( std::make_shared<TestCaseGetLinesAfter3>() );
		test_cases.push_back( std::make_shared<TestCaseGetLinesAfter4>() );

		test_cases.push_back( std::make_shared<TestCaseFindAllOf1>() );

		test_cases.push_back( std::make_shared<TestCaseHighLightKeyWord1>() );
		test_cases.push_back( std::make_shared<TestCaseHighLightKeyWord2>() );
		test_cases.push_back( std::make_shared<TestCaseHighLightKeyWord3>() );
		test_cases.push_back( std::make_shared<TestCaseHighLightKeyWord4>() );

		ColBuilder col;

		const int COL_IDX 		= col.addCol( "Idx" );
		const int COL_NAME 		= col.addCol( "Test" );
		const int COL_RESULT    = col.addCol( "Result" );
		const int COL_EXPTECTED = col.addCol( "Expected" );
		const int COL_TEST_RES	= col.addCol( "Test Result" );

		unsigned idx = 0;

		for( auto & test : test_cases ) {

			idx++;

			DEBUG( format( "run test: %s", test->getName() ) );

			col.addColData( COL_IDX, format( "% 2d", idx ) );
			col.addColData( COL_NAME, test->getName() );

			std::string result;
			std::string expected_result = "true";
			std::string test_result;

			try {

				if( test->throwsException() ) {
					 expected_result = "exception";
				}

				if( test->run() ) {
					result = "true";
				} else {
					result = "false";
				}

			} catch( const std::exception & error ) {
				result = "exception";
				DEBUG( format( "Error: %s", error.what() ));
			} catch( ... ) {
				result = "exception";
				DEBUG( "Error" );
			}

			if( result != expected_result ) {
				test_result = co.color_output( ColoredOutput::RED, "failed" );
			} else {
				test_result = co.color_output( ColoredOutput::GREEN, "succeeded" );
			}

			col.addColData( COL_RESULT, result );
			col.addColData( COL_EXPTECTED, expected_result );
			col.addColData( COL_TEST_RES, test_result );

		}

		std::cout << col.toString() << std::endl;

	} catch( const std::exception & error ) {
		std::cout << "Error: " << error.what() << std::endl;
		return 1;
	}

	return 0;
}

