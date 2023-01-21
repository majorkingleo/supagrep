/*
 * $Log: odbc_db.h,v $
 * Revision 1.3  2007/08/27 17:22:51  wamas
 * Updated odbc Driver
 *
 * Revision 1.2  2006/03/17 19:53:40  wamas
 * tools_config.h serched in the upper directory
 *
 * Revision 1.1.1.1  2006/03/17 19:49:16  wamas
 * own tools reponsitority
 *
 * Revision 1.2  2006/03/09 00:48:27  wamas
 * Added CVS Log Info
 *
 */

#ifndef TOOLS_ODBC_DB_H
#define TOOLS_ODBC_DB_H

#include "../tools_config.h"

#ifdef TOOLS_USE_ODBC

#include "db.h"

namespace Tools {

class ODBCDB : public DB
{
    void *handle;
    void *connection;

    std::string err;

 public:

    ODBCDB();
    ~ODBCDB();

    const char * error();
    bool connect( const char *hostname, const char* username, const char* password );
    bool select_db( const char *db );
    DBErg<DBRowList> select( const std::string & query, bool table_names );
	DBErg<DBRowList> query( const std::string &sql );
    void close();
    int insert_id();    
};

} // namespace Tools

#endif

#endif
