#ifndef SEARCHWIN_H
#define SEARCHWIN_H

#include <fx.h>

#include "search.h"

#include "theme.h"

using namespace Tools;

class ResultWin;
class Main;
class DescCombo;
class Tab;

class SearchWin : public ThemeHorizontalFrame
{
  FXDECLARE( SearchWin );

 private:
  SearchWin() {}

 public:
  enum {
	ID_CB_SEARCH = FXHorizontalFrame::ID_LAST,
	ID_BT_SEARCH,
	ID_CB_PATH,
	ID_BT_PATH,
	ID_CB_FILES,
	ID_CX_HIGHLIGHT,
	ID_SP_LINES,
	ID_RESULT,
	ID_TIMER,
	ID_START_NOW,
	ID_DESTROY,
	ID_LAST
  };

 protected:
  FXComboBox *cb_search;
  FXComboBox *cb_path;
  DescCombo *cb_files;
  FXCheckButton *cx_regex;
  FXCheckButton *cx_icase;
  FXCheckButton *cx_highlight;
  FXSpinner *sp_lines;
  FXProgressBar *pb_state;
  ResultWin *result;
  FXButton *bt_search;
  FXButton *bt_lookup;
  FXLabel *l_runtime;

  Main *main;

  Search::MTAccess<bool> mt_running;
  Search::MTAccess<int>  mt_status;
  Search::MTAccess<int>  mt_status_max;
  Search::MTAccess<bool> mt_stop;
  Search::MTAccess<std::list<Search::Result> > mt_result;
  Search::MTAccess<std::chrono::milliseconds> mt_runtime;

  Search::Config *config;
  Tab *tab;

  bool destroy;

  static const int MAX_ITEMS;

 public:

  SearchWin( Main *main, FXComposite *p, FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_SPACING,FXint pr=DEFAULT_SPACING,FXint pt=DEFAULT_SPACING,FXint pb=DEFAULT_SPACING,FXint hs=DEFAULT_SPACING,FXint vs=DEFAULT_SPACING);

  ~SearchWin();
  long onSelPath( FXObject *obj, FXSelector sel, void * );
  long onSearch( FXObject *obj, FXSelector sel, void * );
  long onTimeout( FXObject *obj, FXSelector sel, void * );
  long onVisChanged( FXObject *obj, FXSelector sel, void * );
  long onDestroy( FXObject *obj, FXSelector sel, void * );
  long onClose( FXObject *obj, FXSelector sel, void * );

  const char *LC( const char *s );

  void startwith( const Search::Config &config );

  void setTab( Tab *tab_ ) { tab = tab_; }

  void create();
  void writeConfig();
};

#endif
