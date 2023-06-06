#include <fx.h>
#include <fxkeys.h>
#include <locale.h>
#include "main.h"
#include "searchwin.h"
#include "string_utils.h"
#include "tab.h"
#include "format.h"
#include "arg.h"
#include "debug.h"
#include "OutDebug.h"
#include "DetectLocale.h"
#include <algorithm>

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
  // new FXMenuCommand(filemenu,LC( "&Settings" ) ,NULL,this,ID_SETTINGS);
  new FXMenuSeparator( filemenu );
  new FXMenuCommand(filemenu,LC( "&Quit the application" ),NULL,getApp(),FXApp::ID_QUIT);
  new FXMenuTitle(menubar,LC( "&Program" ),NULL,filemenu);
  FXMenuPane *helpmenu=new FXMenuPane(this);
  new FXMenuCommand(helpmenu,LC( "&About SupaGrep..." ),NULL,this,ID_ABOUT,0);
  new FXMenuTitle(menubar,LC( "&Help" ),NULL,helpmenu,LAYOUT_RIGHT);

  tab = new FXTabBook( this, NULL, 0,PACK_UNIFORM_HEIGHT|LAYOUT_FILL_X|LAYOUT_FILL_Y); 

  if( getApp()->getArgc() > 1 ) {
	Search::Config conf = Search::getConfFromCommandLine( getApp()->getArgc(), getApp()->getArgv() );

	if( !conf.search.empty() ) {
		add_searchwin( false, &conf );
	} else {
		add_searchwin();
	}
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
								"Copyright (C) 2007 - 2023 by Martin Oberzalek kingleo@gmx.at\n"
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

static void usage( const std::string & prog )
{
  std::cerr << "usage: "
			<< prog << " SEARCH PATTERN\n";
}

int main( int argc, char **argv )
{
  setlocale( LC_ALL, "" );

  FXApp app( "SupaGrep", "KingLeo" );
  
  app.init( argc, argv );

  Arg::Arg arg( argc, argv );

  arg.addPrefix( "-" );
  arg.addPrefix( "--" );

  Arg::OptionChain oc_info;
  arg.addChainR( &oc_info );
  oc_info.setMinMatch( 1 );
  oc_info.setContinueOnMatch( false );
  oc_info.setContinueOnFail( true );

  Arg::FlagOption o_help( "h" );
  o_help.addName( "help" );
  o_help.setDescription( "Show this page" );
  oc_info.addOptionR( &o_help );

  Arg::FlagOption o_debug("debug");
  o_debug.setDescription("print debugging messages");
  o_debug.setRequired(false);
  arg.addOptionR( &o_debug );

  arg.parse();

  if( o_help.getState() )
  {
	  usage(argv[0]);
	  std::cout << arg.getHelp(5,20,30, 80 ) << std::endl;
	  return 0;
  }

  if( o_debug.getState() )
  {
#ifdef _WIN32
	  AllocConsole();
	  freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
#endif
	  Tools::x_debug = new OutDebug();
  }

  new Main( &app );
  
  app.create();
  
  return app.run();
}

