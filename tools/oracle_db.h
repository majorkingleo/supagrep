/*
 * $Log: oracle_db.h,v $
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

#ifndef TOOLS_ORACLE_DB_H
#define TOOLS_ORACLE_DB_H

#include "../tools_config.h"

#ifdef TOOLS_USE_ORACLE

#include "db.h"

namespace Tools {

class OracleDB: public DB
  {
  private:
	void *db;
	static bool done_init;
	std::string err;

  public:
	OracleDB();
	~OracleDB();

	bool connect( const char *hostname, const char *username, const char *passwd );
	bool select_db( const char *db_name );
	const char *error();
	void close();
	
	DBErg<DBRowList> query( const std::string &sql );
	DBErg<DBRowList> select( const std::string &sql, bool table_names );

	int insert_id();
  };

}

#endif

#endif
