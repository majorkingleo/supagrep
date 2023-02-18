#include <fx.h>
#include <fxkeys.h>
#include <locale.h>
#include "main.h"
#include "searchwin.h"
#include "string_utils.h"
#include "tab.h"
#include "format.h"

using namespace Tools;

FXDEFMAP( Main ) MainMap[] = {
  FXMAPFUNC( SEL_COMMAND, Main::ID_ABOUT, Main::onAbout ),
  FXMAPFUNC( SEL_COMMAND, Main::ID_NEW, Main::onNew ),
  FXMAPFUNC( SEL_CLOSE, 0, Main::onClose ),
};

FXIMPLEMENT( Main, FXMainWindow, MainMap, ARRAYNUMBER( MainMap ));


Main::Main( FXApp *app_ )
  : FXMainWindow( app_, "SupaGrep " VERSION, NULL, NULL, DECOR_ALL, 50, 50, 800, 600 ),
	lang(),
	setup( &lang ),
	searchcount(0)
{
  FXMenuBar *menubar=new FXMenuBar(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|FRAME_RAISED);
  FXMenuPane *filemenu=new FXMenuPane(this);
  new FXMenuCommand(filemenu, LC( "&New Search" ),NULL,this,ID_NEW);
  new FXMenuCommand(filemenu,LC( "&Settings" ) ,NULL,this,ID_SETTINGS);
  new FXMenuSeparator( filemenu );
  new FXMenuCommand(filemenu,LC( "&Quit the application" ),NULL,getApp(),FXApp::ID_QUIT);
  new FXMenuTitle(menubar,LC( "&Program" ),NULL,filemenu);
  FXMenuPane *helpmenu=new FXMenuPane(this);
  new FXMenuCommand(helpmenu,LC( "&About SupaGrep..." ),NULL,this,ID_ABOUT,0);
  new FXMenuTitle(menubar,LC( "&Help" ),NULL,helpmenu,LAYOUT_RIGHT);

  tab = new FXTabBook( this, NULL, 0,PACK_UNIFORM_HEIGHT|LAYOUT_FILL_X|LAYOUT_FILL_Y); 

  if( getApp()->getArgc() > 1 ) {
	Search::Config conf;
	int count = 1;

#ifndef WIN32
	if( icase_cmp( getApp()->getArgv()[0], "qc" ) ) {
	  conf.pattern = "*.c,*.cpp,*.cc";
	} else if( icase_cmp( getApp()->getArgv()[0], "qh" ) ) {
	  conf.pattern = "*.h,*.hh";
	} else if( icase_cmp( getApp()->getArgv()[0], "qch" ) ) {
	  conf.pattern = "*.h,*.hh,*.c,*.cpp,*.cc";
	} else if( icase_cmp( getApp()->getArgv()[0], "qrc" ) ) {
	  conf.pattern = "*.rc";
	} 
#endif
	
	if( getApp()->getArgc() > 2 ) {
	  if( icase_cmp( getApp()->getArgv()[1], "-i" ) ) {
		conf.icase = true;
		count++;
	  }
	}

	conf.search = getApp()->getArgv()[count++];

    for( int i = count; i < getApp()->getArgc(); i++ )
    {
         if( conf.pattern.empty() ) {
             conf.pattern = getApp()->getArgv()[i];
         } else {
             conf.pattern += ',' + FXString(getApp()->getArgv()[i]);
         }    
    }

	conf.path = FXSystem::getCurrentDirectory();

	add_searchwin( false, &conf );

  } else {
	add_searchwin();
  }

  getAccelTable()->addAccel(MKUINT(KEY_q,CONTROLMASK),this,FXSEL(SEL_COMMAND,ID_CLOSE));
  getAccelTable()->addAccel(MKUINT(KEY_n,CONTROLMASK),this,FXSEL(SEL_COMMAND,ID_NEW));
  getAccelTable()->addAccel(MKUINT(KEY_t,CONTROLMASK),this,FXSEL(SEL_COMMAND,ID_NEW));
}

Main::~Main()
{

}

void Main::create()
{
     init_theme(getApp());
	FXMainWindow::create();

	show();
}

const char *Main::LC( const char *s )
{
  return lang.translate( s );
}

void Main::add_searchwin( bool create_ , const Search::Config *conf )
{
  Tab *ti = new Tab( tab, FXString( format( LC( "Search %d"), ++searchcount ).c_str() ), NULL, SearchWin::ID_DESTROY );
  SearchWin *w = new SearchWin( this, tab, LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_RAISED );
  ti->setTarget( w );
  w->setTab( ti );

  if( conf ) {
	w->startwith( *conf );
  }

  if( create_ ) 
	{
	  ti->create();
	  w->create();
	  tab->recalc();
	  tab->setCurrent(searchcount-1);
	}
}

long Main::onNew( FXObject *obj, FXSelector sel, void *ptr )
{
  add_searchwin(true);
  return 1;
}

long Main::onAbout( FXObject *obj, FXSelector sel, void *ptr )
{
  FXMessageBox::information(this,MBOX_OK,
							LC("About SupaGrep"),
							LC( "SupaGrep version " VERSION ",\n"
								"Copyright (C) 2007 by Martin Oberzalek kingleo@gmx.at\n"
								"SupaGrep comes with ABSOLUTELY NO WARRANTY;\n"
								"This is free software, and you are welcome to redistribute it\n"
								"under certain conditions (GPL); Hove a look at COPYING for details." )
							);

    return 1;
}

long Main::onClose( FXObject *obj, FXSelector sel, void *ptr )
{
  FXWindow *child = tab->getFirst();
  
  while( child )
	{
	  child->handle( this, sel, ptr );
	  child = child->getNext();
	}

  return FXMainWindow::onCmdClose( obj, sel, ptr );
}

int main( int argc, char **argv )
{
  setlocale( LC_ALL, "" );

  FXApp app( "SupaGrep", "KingLeo" );
  
  app.init( argc, argv );

  new Main( &app );
  
  app.create();
  
  return app.run();
}

