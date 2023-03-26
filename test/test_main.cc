#include <memory>
#include <ColoredOutput.h>
#include <debug.h>
#include "ColBuilder.h"
#include <arg.h>
#include <OutDebug.h>
#include <getline.h>

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

	virtual bool run( const std::string & fac, void *tid ) = 0;

	const std::string & getName() const {
		return name;
	}

	bool throwsException() const {
		return throws_exception;
	}
};

class TestCase
{

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

		test_cases.push_back( std::make_shared<TestCaseCppTpaLoad1>() );

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

				if( test->run( fac, tid ) ) {
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

	} catch( std::exception & error ) {
		std::cout << error.what() << std::endl;
		return 1;
	}

	return 0;
}

