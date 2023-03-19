#ifndef RESULT_WIN_H
#define RESULT_WIN_H

#include <fx.h>

#include "search.h"
#include <ResultWinCommon.h>

class Main;

class ResultWin : public FXList, public ResultWinCommon
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
	ID_OPEN_ECLIPSE,
    ID_CONSOLE,
	ID_KONSOLE,
	ID_GONSOLE,
	ID_DO_CMD,
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

	Cmd( int id_,
		 const std::string & name_,
		 const std::string & exec_,
		 const std::string & open_cmd_ )
	  : id( id_ ),
		 name( name_.c_str() ),
		 exec( exec_.c_str() ),
		 open_cmd( open_cmd_.c_str() )
	{}
  };

  std::vector<Cmd> cmds;

  FXFont *my_font;
  FXIcon *icon;
  Main *main;
  FXString clipped;

 public:
  ResultWin( Main *main, FXComposite *p,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=LIST_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0);


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

  void append( const std::wstring & path, bool use_icon, void *address ) override;
  void append( const std::vector<std::wstring> & paths, bool use_icon, void *address ) override;
  
  int do_system( const FXString & cmd );
  void add_cmd( const Cmd & cmd );
};

#endif
