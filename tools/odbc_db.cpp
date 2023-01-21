/*
 * $Log: odbc_db.cpp,v $
 * Revision 1.1.1.1  2006/03/17 19:49:16  wamas
 * own tools reponsitority
 *
 * Revision 1.2  2006/03/09 00:48:27  wamas
 * Added CVS Log Info
 *
 */

#include "odbc_db.h"

#ifdef TOOLS_USE_ODBC

#include "format.h"
#include "string_utils.h"

#ifdef WIN32
#  include <windows.h>
#endif

#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

#define H( x ) (SQLHENV*)x
#define C( x ) (SQLHDBC*)x

using namespace Tools;

ODBCDB::ODBCDB()
    : DB(),
      handle(0),
      connection(0)
{
    handle     = new SQLHENV;
    connection = new SQLHDBC;

    SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, H(handle) );
    SQLSetEnvAttr( *H(handle), SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC2, 0 );
    SQLAllocHandle( SQL_HANDLE_DBC, *H(handle), C(connection) );
}

ODBCDB::~ODBCDB()
{
    close();

    SQLFreeHandle( SQL_HANDLE_DBC, *C(connection) );
    SQLFreeHandle( SQL_HANDLE_ENV, *C(handle ) );

    delete C(connection);
    delete H(handle);
}

const char* ODBCDB::error()
{
    return err.c_str();
}

bool ODBCDB::connect( const char *hostname, const char* username, const char* password )
{
    if( SQL_SUCCESS != SQLConnect( *C(connection), 
				   (SQLCHAR*) hostname, SQL_NTS,
				   (SQLCHAR*) username, SQL_NTS,
				   (SQLCHAR*) password, SQL_NTS ) )
    {
	err = "connection failed";
	return false;
    }

    return true;	
}

bool ODBCDB::select_db( const char *db )
{
    return true;
}

std::string<std::vector> ODBCDB::read( const std::string & query )
{
    SQLHSTMT statement;
    SQLRETURN ret;
    SQLINTEGER e;

    ret = SQLAllocHandle( SQL_HANDLE_STMT, *C(connection), &statement );     

    ret = SQLExecDirect( statement, (SQLCHAR*) query.c_str(), SQL_NTS );

    if( ret == SQL_ERROR )
    {
	printf( "here\n" );
	printf( query.c_str() );
	printf( "\n" );
	err = format( "sql error\nquery: %s", query.c_str() ).c_str();
	SQLFreeHandle( SQL_HANDLE_STMT, statement );
	return std::vector<std::string>();
    } 
    else if( ret == SQL_INVALID_HANDLE ) 
    {
	err = "invalid handle";
	SQLFreeHandle( SQL_HANDLE_STMT, statement );
	return std::vector<std::string>();
    }

    SQLSMALLINT cols;

    SQLNumResultCols( statement, &cols );

    if( !cols )
    {
	SQLFreeHandle( SQL_HANDLE_STMT, statement );
	return std::vector<std::string>();
    }

    ret = SQLFreeHandle( SQL_HANDLE_STMT, statement );


    SQLAllocHandle( SQL_HANDLE_STMT, *C(connection), &statement );

    std::vector<std::string> sl;

    char buffer[cols][200];

    for( int i = 0; i < cols; ++i )
	SQLBindCol( statement, i+1, SQL_C_CHAR, &buffer[i], 150, &e );

    SQLExecDirect( statement, (SQLCHAR*) query.c_str(), SQL_NTS );


    long erg = SQLFetch( statement );
    int count = 0;
    while( erg != SQL_NO_DATA )
    {
	count++;
	sl.push_back( std::vector<FXString>() );
	
	for( int i = 0; i < cols; ++i )
	{
	    FXString s;
	    s = buffer[i];
	    sl[count-1].push_back( s );
	}

	erg = SQLFetch( statement );
    }

    SQLFreeHandle( SQL_HANDLE_STMT, statement );

    return sl;
}

void ODBCDB::close()
{
    SQLDisconnect( *C(connection) );
}

int ODBCDB::insert_id()
{
    std::vector<std::string> sl = read( "SELECT LAST_INSERT_ID()" );
    
    if( !sl.empty() )
    {
	int d = 0;
	d = s2x<int>( sl[0][0].text(), 0 );
	return d;
    }

    return 0;
}

#endif
