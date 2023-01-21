/*
 * $Log: mysql_db.h,v $
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

#ifndef TOOLS_MYSQL_DB_H
#define TOOLS_MYSQL_DB_H

#include "../tools_config.h"

#ifdef TOOLS_USE_MYSQL

#include "db.h"

namespace Tools {
  
class MySqlDB : public DB
{
 private:
    void *db;

 public:
    MySqlDB();
    ~MySqlDB();

    bool connect(  const char *hostname, const char* username, const char *passwd );
    bool select_db( const char *db_name );
    const char* error();
    void close();

    DBErg<DBRowList> query( const std::string &sql );
    DBErg<DBRowList> select( const std::string &sql, bool table_names );

    int insert_id();
};

} // namespace Tools

#endif

#endif
