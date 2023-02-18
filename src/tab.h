#ifndef TAB_H
#define TAB_H


#include <fx.h>

class Tab : public FXHorizontalFrame {
  FXDECLARE(Tab);
 protected:
  Tab(){}
 private:
  Tab(const Tab&);
  Tab& operator=(const Tab&);

 public:
  enum {
	ID_LABEL = FXHorizontalFrame::ID_LAST,
	ID_BUTTON,
	ID_CLOSE_ITEM,
	ID_LAST
  };

 protected:
  FXButton *label;
  FXButton *bt_close;
  FXIcon *ic_close;

  long my_id;
  FXObject *tgt;
  
 public:
  long onPaint(FXObject*,FXSelector,void*);
  long onFocusIn(FXObject*,FXSelector,void*);
  long onFocusOut(FXObject*,FXSelector,void*);
  long onUngrabbed(FXObject*,FXSelector,void*);
  long onLeftBtnPress(FXObject*,FXSelector,void*);
  long onLeftBtnRelease(FXObject*,FXSelector,void*);
  long onKeyPress(FXObject*,FXSelector,void*);
  long onKeyRelease(FXObject*,FXSelector,void*);
  long onHotKeyPress(FXObject*,FXSelector,void*);
  long onHotKeyRelease(FXObject*,FXSelector,void*);
  long onEnter(FXObject*,FXSelector,void*);
  long onLeave(FXObject*,FXSelector,void*);
  long onLabel(FXObject*,FXSelector,void*);
  long onButton(FXObject*,FXSelector,void*);
 public:

  // Construct a tab item
  Tab(FXTabBar* p,const FXString& text,FXObject *tgt, long id,FXuint opts=TAB_TOP_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD,FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD);
  
  // Returns true because a tab item can receive focus

#if FOX_MAJOR >= 1 && FOX_MINOR >= 7
  virtual FXbool canFocus() const;
#else
  virtual bool canFocus() const;
#endif
  
  // Return current tab item orientation
  FXuint getTabOrientation() const;

  void setTarget( FXObject *tgt_ ) { tgt = tgt_; }

  void setText( const FXString & title );

  // Change tab item orientation
  void setTabOrientation(FXuint style);
};

#endif
