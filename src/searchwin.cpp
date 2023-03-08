#include "searchwin.h"
#include "resultwin.h"
#include "main.h"
#include "desccombo.h"
#include "tab.h"
#include "theme.h"
#include "format.h"
#include "debug.h"
#include <thread>

using namespace Tools;

#if FOX_MAJOR >= 1 && FOX_MINOR >= 7
const unsigned TIMEOUT_VALUE = 300 * 1000000; // 100ms
#else
const unsigned TIMEOUT_VALUE = 300; // 100ms
#endif

FXDEFMAP( SearchWin ) SearchWinMap[] = {
  FXMAPFUNC( SEL_COMMAND, SearchWin::ID_BT_PATH, SearchWin::onSelPath ),
  FXMAPFUNC( SEL_COMMAND, SearchWin::ID_BT_SEARCH, SearchWin::onSearch ),
  FXMAPFUNC( SEL_COMMAND, SearchWin::ID_CB_SEARCH, SearchWin::onSearch ),
  FXMAPFUNC( SEL_CHORE,   SearchWin::ID_START_NOW, SearchWin::onSearch ),
  FXMAPFUNC( SEL_TIMEOUT, SearchWin::ID_TIMER, SearchWin::onTimeout ),
  FXMAPFUNC( SEL_COMMAND, SearchWin::ID_SP_LINES, SearchWin::onVisChanged ),
  FXMAPFUNC( SEL_COMMAND, SearchWin::ID_DESTROY, SearchWin::onDestroy ),
  FXMAPFUNC( SEL_CLOSE, 0, SearchWin::onClose ),
};

FXIMPLEMENT( SearchWin, FXHorizontalFrame, SearchWinMap, ARRAYNUMBER( SearchWinMap ));

const int SearchWin::MAX_ITEMS = 50;

SearchWin::SearchWin( Main *main_, 
					  FXComposite *p, 
					  FXuint opts,
					  FXint x,
					  FXint y,
					  FXint w,
					  FXint h,
					  FXint pl,
					  FXint pr,
					  FXint pt,
					  FXint pb,
					  FXint hs,
					  FXint vs)
  : ThemeHorizontalFrame( p, opts, x, y, w, h, pl, pr, pt, pb, hs, vs ),
	main(main_),
	config(0),
	tab(0),
	destroy(false)
{
  FXVerticalFrame *vf = new FXVerticalFrame( this, LAYOUT_FILL_Y );

  new FXLabel( vf, LC("Path:") );
  cb_path = new ThemeComboBox( vf, 30, this, ID_CB_PATH, FRAME_SUNKEN | COMBOBOX_INSERT_FIRST );
  bt_lookup = new ThemeButton( vf, LC("Lookup"), NULL, this, ID_BT_PATH, BUTTON_NORMAL | LAYOUT_FILL_X );

  new FXLabel( vf, LC("Files:") );
  cb_files = new DescCombo( vf, 30, this, ID_CB_FILES, FRAME_SUNKEN );

  new FXLabel( vf, LC("Search Options:") );
  cx_icase = new FXCheckButton( vf, LC("Case Insensitive") );
  cx_icase->setCheck(true);
  cx_regex = new FXCheckButton( vf, LC("Regular Expression") );

  new FXHorizontalSeparator( vf );

  new ThemeLabel( vf, LC("Search:") );
  cb_search = new ThemeComboBox( vf, 30, this, ID_CB_SEARCH, FRAME_SUNKEN | COMBOBOX_INSERT_FIRST );
  bt_search = new ThemeButton( vf, LC("Go"), NULL, this, ID_BT_SEARCH, BUTTON_NORMAL | LAYOUT_FILL_X );

  FXVerticalFrame *bf = new FXVerticalFrame( vf, LAYOUT_BOTTOM | LAYOUT_FILL_X, 0,0,0,0, 0,0,0,0 );

  new FXHorizontalSeparator( bf );

  new ThemeLabel( bf, LC("Result Options:") );
  FXHorizontalFrame *hf = new FXHorizontalFrame( bf );
  sp_lines = new FXSpinner( hf, 2, this, ID_SP_LINES, FRAME_SUNKEN );
  sp_lines->setValue(1);
  sp_lines->setRange( 1, 99 );
  sp_lines->setIncrement( 2 );
  new FXLabel( hf, LC("Number of lines\naround the keyword") );
  cx_highlight = new FXCheckButton( bf, LC("Highlight Keyword"), this, ID_CX_HIGHLIGHT );

  new FXHorizontalSeparator( bf );

  new FXLabel( bf, LC("Status:") );
  pb_state = new FXProgressBar( bf, NULL, 0, FRAME_SUNKEN | LAYOUT_FILL_X | PROGRESSBAR_PERCENTAGE );
  l_runtime = new FXLabel( bf, " " );

  new FXVerticalSeparator( this );

  FXVerticalFrame *vf2 = new ThemeVerticalFrame( this, LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_SUNKEN, 0,0,0,0, 0,0,0,0 );
  result = new ResultWin( main, vf2, this, ID_RESULT, LIST_SINGLESELECT | LAYOUT_FILL_X | LAYOUT_FILL_Y );


  /* add File pattern */
  for( Setup::config_pair_list_it it = main->getSetup().config.file_pattern.begin();
	   it != main->getSetup().config.file_pattern.end();
	   it++ ) 
	{
	  DEBUG( wformat( L"descr: '%s'; pattern: '%s'", it->descr, it->entry) );
	  cb_files->appendItem( FXString(it->descr.c_str()), &it->entry );
	}

  /* add first an empty record */
  cb_files->setText("");
  cb_files->setNumVisible( cb_files->getNumItems() );
  cb_search->setNumVisible( 5 );
  cb_path->setNumVisible( 5 );

  mt_running.set(false);
}

SearchWin::~SearchWin()
{
  getApp()->removeTimeout( this, FXSEL( SEL_TIMEOUT, ID_TIMER ));                       
                       
  if( config ) {
      if( mt_running.get() == true ) {
          mt_stop.set(true);
          while( mt_running.get() == true ) {
                 FXThread::yield();
          }
      }                           
  }
  
  delete config;
}

long SearchWin::onSelPath( FXObject *obj, FXSelector sel, void * )
{
  FXString res = FXFileDialog::getOpenDirectory( this, LC("Target Directory"), FXSystem::getCurrentDirectory() );
  
  cb_path->setText( res );
  cb_path->prependItem( res );

  return 1;
}

const char *SearchWin::LC( const char *s )
{
  return main->LC( s );
}

long SearchWin::onSearch( FXObject *obj, FXSelector sel, void * )
{     
  if( config )
	{
	  if( mt_running.get() == true ) {
		mt_stop.set( true );
		return 1;
	  } 
	}

  delete config;

  config = new Search::Config();

  mt_result.access().clear();
  mt_result.free();

  mt_running.set(false);
  mt_status.set(0);
  mt_status.clear();
  mt_stop.set(false);
  mt_status_max.set(0);
  mt_status_max.clear();

  config->mt_running = &mt_running;
  config->mt_status = &mt_status;
  config->mt_status_max = &mt_status_max;
  config->mt_stop = &mt_stop;
  config->mt_result = &mt_result;
  config->mt_runtime = &mt_runtime;

  if( cb_path->getText().empty() ) {
	cb_path->setText( FXSystem::getCurrentDirectory() );
  }

  if( cb_files->getText().empty() ) {
	cb_files->setCurrentItem(2);
  }

  config->regex = cx_regex->getCheck();
  config->icase = cx_icase->getCheck();
  config->pattern = cb_files->getText();
  config->path = cb_path->getText();
  config->search = cb_search->getText();

  result->clear();
  

  std::thread search_thread( [this]{
	  Search *s = new Search( *config );
	  mt_running.set(true);
	  s->run();
  } );

  search_thread.detach();

  if( tab )
	tab->setText( config->search );

  getApp()->addTimeout( this, ID_TIMER, TIMEOUT_VALUE );

  return 1;
}

long SearchWin::onTimeout( FXObject *obj, FXSelector sel, void *ptr )
{
  if( destroy ) {
	delete this;
	return 1;
  }

  if( mt_running.get() ) {
	cb_path->disable();
	cb_files->disable();
	cx_regex->disable();
	cx_highlight->disable();
	cx_icase->disable();
	// sp_lines->disable();
	bt_lookup->disable();
	cb_search->disable();
	
	bt_search->setText( LC( "Stop" ) );

	getApp()->addTimeout( this, ID_TIMER, 300000 );
  } else {
	cb_path->enable();
	cb_files->enable();
	cx_regex->enable();
	cx_icase->enable();
	cx_highlight->enable();
	// sp_lines->enable();
	bt_lookup->enable();
	cb_search->enable();
	
	bt_search->setText( LC( "Go" ) );

	pb_state->update();

	pb_state->setTotal(mt_status_max.getAndClear());
	pb_state->setProgress(mt_status_max.getAndClear());
	DEBUG( format( "done: max: %d", mt_status_max.getAndClear()) );
  }

  if( mt_status_max.changed() )
	{
	  DEBUG( format( "Max Status: '%d'", mt_status.get() ) );
	  pb_state->setTotal(mt_status_max.getAndClear());
	  // mt_status_max.clear();
	}

  if( mt_status.changed() ) 
	{
	  DEBUG( format( "Status: '%d'", mt_status.get() ) );
	  pb_state->setProgress(mt_status.getAndClear());
	  // mt_status.clear();
	}

  if( mt_result.changed() )
	{
	  std::list<Search::Result> & l = mt_result.access();

	  for( std::list<Search::Result>::iterator it = l.begin(); it != l.end(); it++ )
		{
		  result->appendItem( *it, config->path );
		}

	  l.clear();

	  mt_result.clear_and_free();
	}

  if( mt_runtime.changed() )
	{
	  l_runtime->setText( format( "%3.3f sec", mt_runtime.access() / 1000000000.0 ).c_str() );
	  mt_runtime.clear_and_free();
	}

  return 1;
}

long SearchWin::onVisChanged( FXObject *obj, FXSelector sel, void * )
{
  result->setVisibleLines( sp_lines->getValue() );
  return 1;
}

void SearchWin::startwith( const Search::Config & conf )
{
  cb_search->setText( conf.search );
  cx_icase->setCheck( conf.icase );
  cx_regex->setCheck( conf.regex );
  cb_files->setText( conf.pattern );
  cb_path->setText( conf.path );
  cb_path->prependItem( conf.path );
  getApp()->addChore( this, ID_START_NOW );
}

long SearchWin::onDestroy( FXObject *obj, FXSelector sel, void * )
{
  writeConfig();

  if( !mt_running.get() ) {
	delete this;
  } else {
	mt_stop.set(true);
	while( mt_running.get() ) {
	  FXThread::yield();
	}
	destroy = true;
  }

  return 1;
}

void SearchWin::create()
{
  FXHorizontalFrame::create();

  for( int i  = 0; i < MAX_ITEMS; i++ )
	{  
	  FXString s = getApp()->reg().readStringEntry( "PATH", format( "PATH%d", i ).c_str(), "" );

	  if( s.empty() )
		break;
		
	  if( !s.empty() )
		{
		  bool found = false;

		  for( int j = 0; j < cb_path->getNumItems(); j++ )
			{
			  if( cb_path->getItemText(j) == s )
				{
				  found = true;
				  break;
				}
			}
		  
		  if( found )
			continue;

		  cb_path->appendItem( s );
		}
	}
}

void SearchWin::writeConfig()
{
  for( int i = 0; i < MAX_ITEMS && i < cb_path->getNumItems(); i++ )
	{
	  getApp()->reg().writeStringEntry( "PATH", 
										format( "PATH%d", i ).c_str(),
										cb_path->getItemText(i).text() );
	}

  if( cb_path->getNumItems() == 0 && !cb_path->getText().empty() ) 
	{
	  getApp()->reg().writeStringEntry( "PATH", "PATH0", cb_path->getText().text() );
	}
}

long SearchWin::onClose( FXObject *obj, FXSelector sel, void * )
{
  writeConfig();
  return 1;
}
