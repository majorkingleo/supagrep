#include "tab.h"
#include <fxkeys.h>
#include "icon_close.h"
#include "tools/theme.h"

using namespace Tools;

#define TAB_ORIENT_MASK    (TAB_TOP|TAB_LEFT|TAB_RIGHT|TAB_BOTTOM)
#define TABBOOK_MASK       (TABBOOK_SIDEWAYS|TABBOOK_BOTTOMTABS)

using namespace FX;

/*******************************************************************************/

// Map
FXDEFMAP(Tab) TabMap[]={
  FXMAPFUNC(SEL_PAINT,0,Tab::onPaint),
  FXMAPFUNC(SEL_FOCUSIN,0,Tab::onFocusIn),
  FXMAPFUNC(SEL_FOCUSOUT,0,Tab::onFocusOut),
  FXMAPFUNC(SEL_UNGRABBED,0,Tab::onUngrabbed),
  FXMAPFUNC(SEL_COMMAND,Tab::ID_LABEL,Tab::onLabel),
  FXMAPFUNC(SEL_COMMAND,Tab::ID_BUTTON,Tab::onButton),
#if 0
  FXMAPFUNC(SEL_KEYPRESS,FXWindow::ID_HOTKEY,Tab::onHotKeyPress),
  FXMAPFUNC(SEL_KEYRELEASE,FXWindow::ID_HOTKEY,Tab::onHotKeyRelease),
  FXMAPFUNC(SEL_KEYPRESS,0,Tab::onKeyPress),
  FXMAPFUNC(SEL_KEYRELEASE,0,Tab::onKeyRelease),
  FXMAPFUNC(SEL_ENTER,0,Tab::onEnter),
  FXMAPFUNC(SEL_ENTER,0,Tab::onLeave),
  FXMAPFUNC(SEL_LEFTBUTTONPRESS,0,Tab::onLeftBtnPress),
  FXMAPFUNC(SEL_LEFTBUTTONRELEASE,0,Tab::onLeftBtnRelease),
#endif
  };


// Object implementation
FXIMPLEMENT(Tab,FXHorizontalFrame,TabMap,ARRAYNUMBER(TabMap));


// Tab item
Tab::Tab(FXTabBar* p,const FXString& text,FXObject *tgt_, long id_ ,FXuint opts,FXint x,FXint y,FXint w,FXint h,FXint pl,FXint pr,FXint pt,FXint pb):
  FXHorizontalFrame(p,opts,x,y,w,h,pl,pr,pt,pb),
  my_id(id_), tgt(tgt_)
{
  border=2;

  ic_close = new FXGIFIcon( getApp(), icon_close );
  ic_close->scale( 16, 16 );

  label = new FXButton( this, text, NULL, this, ID_LABEL, FRAME_NONE );
  bt_close = new ThemeButton( this, FXString::null, ic_close, this, ID_BUTTON, FRAME_NONE );

  shadowColor = FXRGB(123,158,189);
  borderColor = FXRGB(123,158,189);
  hiliteColor = FXRGB(123,158,189);
}


// If window can have focus
#if FOX_MAJOR >= 1 && FOX_MINOR >= 7
FXbool Tab::canFocus() const { return true; }
#else
bool Tab::canFocus() const { return true; }
#endif


// Gained focus
long Tab::onFocusIn(FXObject* sender,FXSelector sel,void* ptr)
{
  FXHorizontalFrame::onFocusIn(sender,sel,ptr);
  update(border,border,width-(border<<1),height-(border<<1));
  return 1;
}


// Lost focus
long Tab::onFocusOut(FXObject* sender,FXSelector sel,void* ptr)
{
  FXHorizontalFrame::onFocusOut(sender,sel,ptr);
  update(border,border,width-(border<<1),height-(border<<1));
  return 1;
}


// Pressed mouse button
long Tab::onLeftBtnPress(FXObject* sender,FXSelector sel,void* ptr)
{
  if(!FXHorizontalFrame::onLeftBtnPress(sender,sel,ptr)){
    if(isEnabled()){
      getParent()->handle(this,FXSEL(SEL_COMMAND,FXTabBar::ID_OPEN_ITEM),ptr);
      flags|=FLAG_PRESSED;
      flags&=~FLAG_UPDATE;
      return 1;
	}
  }
  return 0;
}


// Released mouse button
long Tab::onLeftBtnRelease(FXObject* sender,FXSelector sel,void* ptr)
{
  if(!FXHorizontalFrame::onLeftBtnRelease(sender,sel,ptr)){
    if(isEnabled()){
      flags|=FLAG_UPDATE;
      flags&=~FLAG_PRESSED;
      return 1;
	}
  }
  return 0;
}


// Lost the grab for some reason
long Tab::onUngrabbed(FXObject* sender,FXSelector sel,void* ptr)
{
  FXHorizontalFrame::onUngrabbed(sender,sel,ptr);
  flags&=~FLAG_PRESSED;
  flags|=FLAG_UPDATE;
  return 1;
}


// Key Press
long Tab::onKeyPress(FXObject*,FXSelector,void* ptr)
{
  FXEvent* event=(FXEvent*)ptr;
  flags&=~FLAG_TIP;
  if(isEnabled()){
    if(target && target->tryHandle(this,FXSEL(SEL_KEYPRESS,message),ptr)) return 1;
    if(event->code==KEY_space || event->code==KEY_KP_Space){
      getParent()->handle(this,FXSEL(SEL_COMMAND,FXTabBar::ID_OPEN_ITEM),ptr);
      return 1;
	}
  }
  return 0;
}


// Key Release
long Tab::onKeyRelease(FXObject*,FXSelector,void* ptr)
{
  FXEvent* event=(FXEvent*)ptr;
  if(isEnabled()){
    if(target && target->tryHandle(this,FXSEL(SEL_KEYRELEASE,message),ptr)) return 1;
    if(event->code==KEY_space || event->code==KEY_KP_Space){
      return 1;
	}
  }
  return 0;
}


// Hot key combination pressed
long Tab::onHotKeyPress(FXObject*,FXSelector,void* ptr)
{
  handle(this,FXSEL(SEL_FOCUS_SELF,0),ptr);
  flags&=~FLAG_TIP;
  if(isEnabled()){
    getParent()->handle(this,FXSEL(SEL_COMMAND,FXTabBar::ID_OPEN_ITEM),ptr);
  }
  return 1;
}


// Hot key combination released
long Tab::onHotKeyRelease(FXObject*,FXSelector,void*)
{
  return 1;
}


// Handle repaint
long Tab::onPaint(FXObject*,FXSelector,void* ptr)
{
  FXEvent *ev=(FXEvent*)ptr;
  FXDCWindow dc(this,ev);
#if 0
  FXint tw=0,th=0,iw=0,ih=0,tx,ty,ix,iy;
#endif
  dc.setForeground(backColor);
  //dc.setForeground(FXRGB(255,0,0));
  dc.fillRectangle(ev->rect.x,ev->rect.y,ev->rect.w,ev->rect.h);
  switch(options&TAB_ORIENT_MASK){
  case TAB_LEFT:
	dc.setForeground(hiliteColor);
	dc.drawLine(2,0,width-1,0);
	dc.drawLine(0,2,1,1);
	dc.drawLine(0,height-4,0,2);
	dc.setForeground(shadowColor);
	dc.fillRectangle(1,height-3,1,1);
	dc.fillRectangle(2,height-2,width-3,1);
	dc.setForeground(borderColor);
	dc.drawLine(3,height-1,width-1,height-1);
	break;
  case TAB_RIGHT:
	dc.setForeground(hiliteColor);
	dc.drawLine(0,0,width-3,0);
	dc.drawLine(width-3,0,width-1,3);
	dc.setForeground(shadowColor);
	dc.drawLine(width-2,2,width-2,height-2);
	dc.drawLine(0,height-2,width-2,height-2);
	dc.setForeground(borderColor);
	dc.drawLine(0,height-1,width-3,height-1);
	dc.drawLine(width-1,3,width-1,height-4);
	dc.drawLine(width-3,height-1,width-1,height-4);
	break;
  case TAB_BOTTOM:
	dc.setForeground(hiliteColor);
	dc.drawLine(0,0,0,height-4);
	dc.drawLine(0,height-4,1,height-2);
	dc.setForeground(shadowColor);
	dc.fillRectangle(2,height-2,width-4,1);
	dc.drawLine(width-2,0,width-2,height-3);
	dc.fillRectangle(width-2,0,2,1);
	dc.setForeground(borderColor);
	dc.drawLine(3,height-1,width-4,height-1);
	dc.drawLine(width-4,height-1,width-1,height-4);
	dc.fillRectangle(width-1,1,1,height-4);
	break;
  case TAB_TOP:
	dc.setForeground(hiliteColor);
	dc.fillRectangle(0,2,1,height-2);
	dc.drawLine(0,2,2,0);
	dc.fillRectangle(2,0,width-4,1);
	dc.setForeground(shadowColor);
	dc.fillRectangle(width-2,1,1,height-1);
	dc.setForeground(borderColor);
	dc.drawLine(width-2,1,width-1,2);
	dc.fillRectangle(width-1,2,1,height-3);
	break;
  }
#if 0
  if(!label.empty())
	{
	  tw=labelWidth(label);
	  th=labelHeight(label);
	}
  if(icon)
	{
	  iw=icon->getWidth();
	  ih=icon->getHeight();
	}

  just_x(tx,ix,tw,iw);
  just_y(ty,iy,th,ih);

  if(icon){
    if(isEnabled())
      dc.drawIcon(icon,ix,iy);
    else
      dc.drawIconSunken(icon,ix,iy);
    }
  if(!label.empty()){
    dc.setFont(font);
    if(isEnabled()){
      dc.setForeground(textColor);
      drawLabel(dc,label,hotoff,tx,ty,tw,th);
      if(hasFocus()){
        dc.drawFocusRectangle(border+1,border+1,width-2*border-2,height-2*border-2);
	  }
	}
    else{
      dc.setForeground(hiliteColor);
      drawLabel(dc,label,hotoff,tx+1,ty+1,tw,th);
      dc.setForeground(shadowColor);
      drawLabel(dc,label,hotoff,tx,ty,tw,th);
	}
  }
#endif
  return 1;
}


// Get tab style
FXuint Tab::getTabOrientation() const 
{
  return (options&TAB_ORIENT_MASK);
}


// Set tab style
void Tab::setTabOrientation(FXuint style)
{
  FXuint opts=(options&~TAB_ORIENT_MASK) | (style&TAB_ORIENT_MASK);
  if(options!=opts){
    options=opts;
    recalc();
    update();
  }
}

long Tab::onEnter(FXObject*,FXSelector,void*)
{


  if( isEnabled() )
	grab();

  return 1;
}
 
long Tab::onLeave(FXObject*,FXSelector,void*)
{


  if( isEnabled() )
	ungrab();

  return 1;
}

long Tab::onLabel(FXObject* obj,FXSelector sel,void* ptr)
{
  getParent()->handle(this,FXSEL(SEL_COMMAND,FXTabBar::ID_OPEN_ITEM),ptr);
  flags|=FLAG_PRESSED;
  flags&=~FLAG_UPDATE;
  return 1;
}


long Tab::onButton(FXObject* obj,FXSelector sel,void* ptr)
{
  FXWindow* child = getParent()->getFirst();

  while( child )
	{
	  if( child == this )
		{
		  child = child->getNext();
		  break;
		}

	  child = child->getNext();
	} 
  
  if( tgt )
	tgt->handle( this, FXSEL( SEL_COMMAND, my_id ), child );
  else 
	delete child;

  delete this;
  return 1;
}

void Tab::setText( const FXString & title )
{
  label->setText( title );
}
