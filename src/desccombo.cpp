#include "desccombo.h"

FXDEFMAP( DescCombo ) DescComboMap[] = {
  FXMAPFUNC( SEL_COMMAND, DescCombo::ID_LIST, DescCombo::onCommand )
};

FXIMPLEMENT( DescCombo, ThemeComboBox, DescComboMap, ARRAYNUMBER( DescComboMap ));

DescCombo::DescCombo( FXComposite *p,
					  FXint cols,
					  FXObject* tgt,
					  FXSelector sel,
					  FXuint opts,
					  FXint x,
					  FXint y,
					  FXint w,
					  FXint h,
					  FXint pl,
					  FXint pr,
					  FXint pt,
					  FXint pb)
  : ThemeComboBox( p, cols, tgt, sel, opts, x, y, w, h, pl, pr, pt, pb )
{

}

long DescCombo::onCommand( FXObject *obj, FXSelector sel, void *ptr )
{
  int index = getCurrentItem();

  if( index >= 0 ) {
	FXString *s = static_cast<FXString*>(getItemData( index ));

	if( s ) {
	  setText( *s );
	}
  }
  return target && target->tryHandle(this,FXSEL(SEL_COMMAND,message),ptr);
}

void DescCombo::setCurrentItem( int index )
{
  FXComboBox::setCurrentItem(index);
  onCommand( NULL, 0, NULL );
}
