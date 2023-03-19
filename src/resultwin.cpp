#ifdef WIN32
#include <windows.h>
# include <tchar.h>
#endif

#include "resultwin.h"
#include <FXMemMap.h>
#include <FXGIFIcon.h>
#include <FX88591Codec.h>
#include <FXUTF16Codec.h>
#include "string_utils.h"
#include "icon_add.h"
#include "main.h"
#include "utf8_util.h"
#include "debug.h"
#include <read_file.h>
#include <getline.h>

#include "format.h"

using namespace Tools;

using namespace Tools;

FXDEFMAP( ResultWin ) ResultWinMap[] = {
  FXMAPFUNC(SEL_RIGHTBUTTONPRESS,     0,     ResultWin::onQueryMenu),
  FXMAPFUNC(SEL_COMMAND,     ResultWin::ID_COPY,     ResultWin::onCopy),
  FXMAPFUNC(SEL_CLIPBOARD_REQUEST, 0, ResultWin::onClipboardRequest ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_OPEN, ResultWin::onOpen ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_OPEN_VI, ResultWin::onOpenExec ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_OPEN_GVIM, ResultWin::onOpenExec ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_OPEN_KVIM, ResultWin::onOpenExec ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_OPEN_EMACS, ResultWin::onOpenExec ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_OPEN_EMACS_CLIENT, ResultWin::onOpenExec ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_OPEN_XEMACS, ResultWin::onOpenExec ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_OPEN_XEMACS_CLIENT, ResultWin::onOpenExec ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_OPEN_KATE, ResultWin::onOpenExec ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_OPEN_KWRITE, ResultWin::onOpenExec ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_OPEN_ADIE, ResultWin::onOpenExec ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_OPEN_ADIE_WIN, ResultWin::onOpenExec ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_OPEN_WINVI, ResultWin::onOpenExec ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_OPEN_ECLIPSE, ResultWin::onOpenExec ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_CONSOLE, ResultWin::onOpenExec ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_KONSOLE, ResultWin::onOpenExec ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_GONSOLE, ResultWin::onOpenExec ),
  FXMAPFUNC(SEL_COMMAND, ResultWin::ID_DO_CMD, ResultWin::onOpenExec ),
};

FXIMPLEMENT( ResultWin, FXList, ResultWinMap, ARRAYNUMBER( ResultWinMap ));

ResultWin::ResultWin( Main *main_,
					  FXComposite *p,
					  FXObject* tgt,
					  FXSelector sel,
					  FXuint opts,
					  FXint x,
					  FXint y,
					  FXint w,
					  FXint h)
  : FXList( p, tgt, sel, opts, x, y, w, h ),
    ResultWinCommon(),
	main(main_)
{
  my_font = new FXFont( getApp(), "Courier", 10 );
  setFont(my_font);
  icon = new FXGIFIcon( getApp(), icon_add );

  add_cmd( Cmd( ID_OPEN_GVIM, 
				format( LC( "Open File with %s" ), LC( "GVim" ) ),
				"gvim", "gvim +%d %s" ) );
  add_cmd( Cmd( ID_OPEN_KVIM, 
				format( LC( "Open File with %s" ), LC( "KVim" ) ),
				"kvim", "kvim +%d %s" ) );
  add_cmd( Cmd( ID_OPEN_EMACS, 
				format( LC( "Open File with %s" ), LC( "Emacs" ) ),
				"emacs", "emacs +%d %s" ) );
  add_cmd( Cmd( ID_OPEN_EMACS_CLIENT, 
				format( LC( "Open File with %s" ), LC( "Emacs Client" ) ),
				"emacsclient", "emacsclient +%d %s" ) );
  add_cmd( Cmd( ID_OPEN_XEMACS, 
				format( LC( "Open File with %s" ), LC( "XEmacs" ) ),
				"xemacs", "xemacs +%d %s" ) );
  add_cmd( Cmd( ID_OPEN_XEMACS_CLIENT, 
				format( LC( "Open File with %s" ), LC( "XEmacs Client" ) ),
				"xemacsclient", "xemacsclient +%d %s" ) );
  add_cmd( Cmd( ID_OPEN_KATE, 
				format( LC( "Open File with %s" ), LC( "Kate" ) ),
				"kate", "kate --line %d %s" ) );
  add_cmd( Cmd( ID_OPEN_ADIE, 
				format( LC( "Open File with %s" ), LC( "Adie" ) ),
				"adie", "adie --line %d %s" ) );
  add_cmd( Cmd( ID_OPEN_ADIE_WIN, 
				format( LC( "Open File with %s" ), LC( "Adie" ) ),
				"adie.exe", "adie --line %d %s" ) );
  add_cmd( Cmd( ID_OPEN_KWRITE, 
				format( LC( "Open File with %s" ), LC( "Kwrite" ) ),
				"kwrite", "kwrite --line %d %s" ) );

  add_cmd( Cmd( ID_OPEN_WINVI, 
				format( LC( "Open File with %s" ), LC( "WinVi" ) ),
				"Winvi32.exe", "winvi32 +%d %s" ) );

  add_cmd( Cmd( ID_OPEN_VI, 
				format( LC( "Open File with %s" ), LC( "Vi" ) ),
				"vi.exe", "vi +%d %s" ) );


  add_cmd( Cmd( ID_CONSOLE, 
				format( LC( "Start %s" ), LC( "XTerm" ) ),
				"xterm", "xterm" ) );

  add_cmd( Cmd( ID_KONSOLE, 
				format( LC( "Start %s" ), LC( "Konsole" ) ),
				"konsole", "konsole" ) );

  add_cmd( Cmd( ID_GONSOLE, 
				format( LC( "Start %s" ), LC( "Gnome Terminal" ) ),
				"gnome-terminal", "gnome-terminal" ) );
				
  add_cmd( Cmd( ID_DO_CMD, 
				format( LC( "Start %s" ), LC( "Console" ) ),
				"cmd.exe", "cmd" ) );

  add_cmd( Cmd( ID_OPEN_ECLIPSE,
				format( LC( "Open File with %s" ), LC( "Eclipse" ) ),
				"eclipse.exe", "eclipse.exe --launcher.openFile %s:%d" ) );
}

void ResultWin::create()
{
  FXList::create();

  my_font->create();
  icon->create();
}

void ResultWin::clear()
{
  clearItems();
  results.clear();
}

void ResultWin::append( const std::wstring & path, bool use_icon, void *address )
{
	FXList::appendItem( path.c_str(), use_icon ? icon : nullptr, address );
}

void ResultWin::append( const std::vector<std::wstring> & paths, bool use_icon, void *address )
{
	for( const auto & path : paths ) {
		FXList::appendItem( path.c_str(), use_icon ? icon : nullptr, address );
	}
}


void ResultWin::setVisibleLines( int vl )
{
  view_lines = vl;
  clearItems();

  for( result_list::iterator it = results.begin(); 
	   it != results.end(); it++ )
	{
	  appendResult( it->result, it->path, false, &(*it) );
	}
}

long ResultWin::onQueryMenu( FXObject *obj, FXSelector sel, void *ptr )
{
  FXEvent *event = static_cast<FXEvent*>(ptr);
  int index;

  if( event && ( index = getItemAt( event->win_x, event->win_y ) ) >= 0 )
	{
	  selectItem( index, true );
	  setCurrentItem( index );

	  FXMenuPane pane(this);

#ifdef WIN32	  
	  new FXMenuCommand(&pane, LC("Open with default App"), NULL, this, ID_OPEN );
#endif

	  for( unsigned i = 0; i < cmds.size(); i++ ) {
		new FXMenuCommand(&pane, cmds[i].name, NULL, this, cmds[i].id );
	  }

	  new FXMenuCommand(&pane, LC("Copy Filename"), NULL, this, ID_COPY );
	  
	  pane.create();
	  
	  pane.popup(NULL, event->root_x,event->root_y );
	  getApp()->runModalWhileShown(&pane);
	}
  return 1;
}

const char *ResultWin::LC( const char *s )
{
  return main->LC( s );
}

long ResultWin::onCopy( FXObject *obj, FXSelector sel, void *ptr )
{
  int index = getCurrentItem();

  if( index < 0 )
	return 0;

  FXDragType types[4]={stringType,textType,utf8Type,utf16Type};

  if(acquireClipboard(types,4)){
	Entry *entry = static_cast<Entry*>(getItemData( index ));

	if( entry == NULL )
	  return 0;

	clipped = entry->result.file.wstring().c_str();
	return 0;
  }

  return 1;
}

// Somebody wants our clipped text
long ResultWin::onClipboardRequest(FXObject* sender,FXSelector sel,void* ptr)
{
  FXEvent *event=(FXEvent*)ptr;
  FXString string;
  
  // Perhaps the target wants to supply its own data for the clipboard
  // if(FXFrame::onClipboardRequest(sender,sel,ptr)) return 1;

  // Recognize the request?
  if(event->target==stringType || 
	 event->target==textType || 
	 event->target==utf8Type ||
	 event->target==utf16Type) 
	{
	  
	  // Get clipped string
	  string=clipped;
	  
	  DEBUG( wformat(L"Clippboard String: '%s'", Utf8Util::utf8toWString(string.text()) ) );

	  setDNDData(FROM_CLIPBOARD,event->target,string);
	  return 1;
	}
  return 0;
}

long ResultWin::onOpen( FXObject *obj, FXSelector sel, void *ptr )
{
  int index = getCurrentItem();

  if( index < 0 )
	return 0;

  Entry *entry = static_cast<Entry*>(getItemData( index ));

  if( entry == NULL )
    return 0;
    
#ifdef WIN32
  ShellExecute( NULL, "open", entry->result.file.string().c_str(), "", "", SW_SHOWNORMAL );
#else
# warning TODO on Unix
#endif
  return 1;
}

int ResultWin::do_system( const FXString & cmd )
{
#ifdef WIN32    
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  LPTSTR szCmdline=(TCHAR*)_tcsdup(TEXT(cmd.text()));
  LPTSTR szCwd=(TCHAR*)_tcsdup(TEXT(FXSystem::getCurrentDirectory().text()));

  ZeroMemory( &si, sizeof(si) );
  // si.wShowWindow = SW_SHOWMINNOACTIVE;
  // si.dwFlags = STARTF_USESHOWWINDOW;
  si.cb = sizeof(si);
  ZeroMemory( &pi, sizeof(pi) );

  DEBUG( format( "cmd: '%s'",cmd.text()) );

  if( !CreateProcess( NULL, szCmdline, NULL, NULL, false, 0, NULL, szCwd, &si, &
pi ) )
  {
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    lpDisplayBuf = LocalAlloc(LMEM_ZEROINIT,
        (strlen((char*)lpMsgBuf)+40)*sizeof(TCHAR));

    wsprintf((char*)lpDisplayBuf,
        TEXT("error %d: %s"),
        dw, lpMsgBuf);

        FXMessageBox::error( this, MBOX_OK, "Fehler beim Starten des Programms", (char*) lpDisplayBuf );

      LocalFree(lpMsgBuf);
      LocalFree(lpDisplayBuf);
      // ExitProcess(dw);
      return 0;
      }
  return -1;
#else
  return system( cmd.text() );   
#endif
}

void ResultWin::add_cmd( const Cmd & cmd )
{
  if( FXPath::search( FXSystem::getExecPath(), cmd.exec ).empty() )
	return;

  cmds.push_back( cmd );
};

long ResultWin::onOpenExec( FXObject *obj, FXSelector sel, void *ptr )
{
  int index = getCurrentItem();
  
  if( index < 0 )
	return 0;
  
  Entry *entry = static_cast<Entry*>(getItemData( index ));
  
  if( entry == NULL )
    return 0;

  for( unsigned i = 0; i < cmds.size(); i++ ) 
	{
	  if( FXSELID( sel ) == cmds[i].id ) 
		{
		  FXString workingdir = FXSystem::getCurrentDirectory();
		  
		  FXSystem::setCurrentDirectory( FXPath::directory( entry->result.file.wstring().c_str() ) );

		  int idx_line = cmds[i].open_cmd.find( "%d" );
		  int idx_file = cmds[i].open_cmd.find( "%s" );

		  FXString s;

		  if( idx_line != -1 ) {
			  if( idx_line < idx_file ) {
				 s = format( cmds[i].open_cmd.text(), entry->result.line, entry->result.file.string() ).c_str();
			  } else {
				 s = format( cmds[i].open_cmd.text(), entry->result.file.string(), entry->result.line ).c_str();
			  }
		  } else {
			  s =  format( cmds[i].open_cmd.text(), entry->result.file.string() ).c_str();
		  }


#ifndef WIN32		  
		  s += " &";
#endif		  

		  getApp()->beginWaitCursor();
		  do_system( s );
		  getApp()->endWaitCursor();

		  FXSystem::setCurrentDirectory( workingdir );

		  return 1;
		}
	}
  return 0;
}
