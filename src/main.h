#ifndef MAIN_H
#define MAIN_H

#include <fx.h>

#include "lang.h"
#include "setup.h"
#include "search.h"

class Main : public FXMainWindow
{
  FXDECLARE( Main );
  
 private:
  Main() : setup(NULL) {}

 public:
  enum {
	ID_ABOUT = FXMainWindow::ID_LAST,
	ID_SETTINGS,
	ID_NEW,
	ID_LAST
  };

 protected:
  Lang lang;
  Setup setup;
  FXTabBook *tab;
    
  int searchcount;

 public:
  Main( FXApp * app );
  ~Main();
  
  virtual void create();

  const char *LC( const char *s );  
  
  Setup & getSetup() { return setup; }

  void add_searchwin( bool create = false, const Search::Config * conf = NULL );
  long onAbout( FXObject *obj, FXSelector sel, void *ptr );
  long onNew( FXObject *obj, FXSelector sel, void *ptr );
  long onClose( FXObject *obj, FXSelector sel, void *ptr );
};

#endif
