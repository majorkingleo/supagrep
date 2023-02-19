#ifndef DESCCOMBO_H
#define DESCCOMBO_H

#include <fx.h>

#include "theme.h"

using namespace Tools;

class DescCombo : public ThemeComboBox
{
  FXDECLARE( DescCombo );

 private:
  DescCombo() {}

 public:
  DescCombo( FXComposite *p,FXint cols,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=COMBOBOX_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD,FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD);

  long onCommand( FXObject *obj, FXSelector sel, void *ptr );

  void setCurrentItem( int index );
};

#endif
