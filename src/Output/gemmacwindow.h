/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Interface for the window manager

  Copyright (c) 2009 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_GEMMACWINDOW_H_
#define INCLUDE_GEMMACWINDOW_H_

#include "Base/GemContext.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemmacwindow

  The window manager

  DESCRIPTION

  Access to GemMan.

  "bang"  - swap the buffers
  "render" - render a frame now

  "create" - create a graphics window
  "destroy" - destroy the graphics window


  "buffer" - single or double buffered
  "fsaa" - full screen anti-aliasing

  "title" - set a title for the graphics window
  "border" - whether we want a border as decoration or not

  "dimen" - the window dimensions
  "fullscreen" - fullscreen mode
  "offset" - the window offset
  "secondscreen" - render to the secondscreen (auto-offset)

  "cursor" - whether we want a cursor or not
  "menubar" - hide notorious menubars
  "topmost" - set the window to stay on top

  -----------------------------------------------------------------*/


class GEM_EXTERN gemmacwindow : public GemContext
{
  CPPEXTERN_HEADER(gemmacwindow, GemContext)

    public:

  //////////
  // Constructor
  gemmacwindow(void);

 private:

  //////////
  // Destructor
  virtual ~gemmacwindow(void);


  // create window
  virtual bool create(void);

  // destroy window
  virtual void destroy(void);

  // check whether we have a window and if so, make it current
  virtual bool makeCurrent(void);

  void doRender(void);

  /* rendering */
  void renderMess(void);

  /* dispatch window events */
  void dispatch(void);

  /* render context (pre creation) */
  void  bufferMess(int buf);
  int         m_buffer;
  void    fsaaMess(int value);
  int         m_fsaa;

  /* window decoration (pre creation) */
  void titleMess(t_symbol* s);
  std::string     m_title;
  void borderMess(bool on);
  bool       m_border;

  /* window position/dimension (pre creation) */
  virtual void    dimensionsMess(int width, int height);
  unsigned int         m_width, m_height;

  void    fullscreenMess(bool on);
  bool              m_fullscreen;
  void        offsetMess(int x, int y);
  unsigned int      m_xoffset, m_yoffset;

  /* creation/destruction */
  void        createMess(void);
  void       destroyMess(void);

  /* post creation */
  void        cursorMess(bool on);
  bool              m_cursor;

  //////////
  // the real width/height of the window (set by createGemWindow())
  unsigned int real_w, real_h, real_x, real_y;

  //// X display specification (e.g. "remote:0.1")
  std::string m_display;

  //////////
  // Should the window be realized
#warning actuallyDisplay
  bool         m_actuallyDisplay;

 private:

  class Info;
  Info*m_info;

  //////////
  // Static member functions (rendering)
  static void     renderMessCallback(void *data);

  //////////
  // Static member functions (window pre-creation)
  static void     bufferMessCallback(void *data, t_floatarg buf);
  static void     fsaaMessCallback(void *data,t_floatarg val);

  static void     titleMessCallback(void *data, t_symbol* s);

  static void     dimensionsMessCallback(void *data, t_floatarg width, t_floatarg height);
  static void     offsetMessCallback(void *data, t_floatarg x, t_floatarg y);
  static void     fullscreenMessCallback(void *data, t_floatarg on);

  static void     borderMessCallback(void *, t_floatarg state);

  //////////
  // Static member functions (window creation)
  static void     createMessCallback(void *);
  static void     destroyMessCallback(void *);

  //////////
  // Static member functions (window post-creation)
  static void     cursorMessCallback(void *, t_floatarg);

  //////////
  // Static member functions (misc)
  static void     printMessCallback(void *);

  t_clock*m_clock;
  int m_polltime;
  static void clockCallback(void*);
  void clock(void);
};

#endif    // for header file