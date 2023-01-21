#ifndef RESULT_WIN_H
#define RESULT_WIN_H

#include <fx.h>

#include "search.h"

class Main;

class ResultWin : public FXList
{
  FXDECLARE( ResultWin );

 private:
  ResultWin() {}

 public:
  enum {
	ID_COPY = FXList::ID_LAST,
    ID_OPEN,
	ID_OPEN_VI,
	ID_OPEN_GVIM,
	ID_OPEN_KVIM,
	ID_OPEN_EMACS,
	ID_OPEN_EMACS_CLIENT,
	ID_OPEN_XEMACS,
	ID_OPEN_XEMACS_CLIENT,
	ID_OPEN_ADIE,
	ID_OPEN_ADIE_WIN,
	ID_OPEN_KATE,
	ID_OPEN_KWRITE,
	ID_OPEN_WINVI,
    ID_CONSOLE,
	ID_KONSOLE,
	ID_GONSOLE,
	ID_CMD,
	ID_LAST
  };

 protected:
  struct Cmd
  {
	int id;
	FXString name;
	FXString exec;
	FXString open_cmd;

	Cmd( int id_, 
		 const FXString & name_, 
		 const FXString & exec_, 
		 const FXString & open_cmd_ )
	  : id( id_ ), 
		 name( name_ ), 
		 exec( exec_ ), 
		 open_cmd( open_cmd_ )
	{}
  };

  std::vector<Cmd> cmds;

  struct Entry
  {
	Search::Result result;
	FXString path;

	Entry( const Search::Result &res, const FXString &p )
	  : result( res ), path( p )
	{}
  };

  typedef std::list<Entry> result_list;

  result_list results;

  int view_lines;

  FXFont *my_font;
  FXIcon *icon;
  Main *main;
  FXString clipped;

 public:
  ResultWin( Main *main, FXComposite *p,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=LIST_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

  void appendItem( const Search::Result & result, const FXString &path, bool do_append = true, void *address = NULL );
  void clear();
  void setVisibleLines( int vl );
  void create();

  const char *LC( const char* s );

  long onQueryMenu( FXObject *obj, FXSelector sel, void *ptr );
  long onOpen( FXObject *obj, FXSelector sel, void *ptr );
  long onOpenExec( FXObject *obj, FXSelector sel, void *ptr );
  long onCopy( FXObject *obj, FXSelector sel, void *ptr );
  long onConsole( FXObject *obj, FXSelector sel, void *ptr );
  long onClipboardRequest(FXObject* sender,FXSelector sel,void* ptr);


 protected:
  FXString getLineAtPos( const FXString & file, long pos, int lines );
  
  int do_system( const FXString & cmd );
  void add_cmd( const Cmd & cmd );
};

#endif
