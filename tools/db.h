/*
 * $Log: db.h,v $
 * Revision 1.3  2006/11/24 09:47:27  wamas
 * -Wshadow Warnings ausgebaut
 *
 * Revision 1.2  2006/03/17 19:53:40  wamas
 * tools_config.h serched in the upper directory
 *
 * Revision 1.1.1.1  2006/03/17 19:49:15  wamas
 * own tools reponsitority
 *
 * Revision 1.2  2006/03/09 00:48:26  wamas
 * Added CVS Log Info
 *
 */

#ifndef TOOLS_DB_H
#define TOOLS_DB_H

#include "../tools_config.h"

#ifdef TOOLS_USE_DB

#include <string>
#include <vector>

namespace Tools {

struct DBRow
{
  std::vector<std::string> names;
  std::vector<std::string> values;
  
  public:
  DBRow( const std::vector<std::string> &names_ )
    : names( names_ )
  {}

  DBRow( const std::vector<std::string> &names_, const std::vector<std::string> &values_ )
    : names( names_ ), values( values_ )
  {}
};

struct DBRowList
{
  std::vector<std::string> names;
  std::vector< std::vector<std::string> > values;
  
  public:
  DBRowList() : names() {}
  DBRowList( const std::vector<std::string> &names_ )
    : names( names_ )
  {}
};

template<typename T> class DBErg
{
 public:
  T row_list;
  bool success;

 public:
  DBErg() : success( false ) {}
  DBErg( const std::vector<std::string> &names )
    : 
    row_list( names ),
    success( true )
      {}      

    bool operator!() const { return !success; }
};

class DB
{
 public:
    DB();
    virtual ~DB();

    virtual const char * error() = 0;
    virtual bool connect( const char *hostname, const char* username, const char* password ) = 0;
    virtual bool select_db( const char *db ) = 0;
    virtual DBErg<DBRowList> query( const std::string &sql ) = 0;
    virtual DBErg<DBRowList> select( const std::string &sql, bool table_names ) = 0;
    virtual void close() = 0;
    virtual int insert_id() = 0;
};

} // namespace Tools

#endif

#endif
