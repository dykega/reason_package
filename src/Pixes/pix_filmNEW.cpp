////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_filmNEW.h"
#include <ctype.h>

#include "Pixes/filmQT4L.h"
#include "Pixes/filmAVI.h"
#include "Pixes/filmAVIPLAY.h"
#include "Pixes/filmFFMPEG.h"
#include "Pixes/filmMPEG1.h"
#include "Pixes/filmMPEG3.h"

CPPEXTERN_NEW_WITH_ONE_ARG(pix_filmNEW, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_filmNEW
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_filmNEW :: pix_filmNEW(t_symbol *filename) :
  m_auto(0), 
  m_numFrames(0), m_reqFrame(0), m_curFrame(0),
  m_numTracks(0), m_track(0), m_film(true)
{
  // setting the current frame
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("img_num"));
  // create an outlet to send out how many frames are in the movie + bang when we reached the end
  m_outNumFrames = outlet_new(this->x_obj, 0);
  m_outEnd       = outlet_new(this->x_obj, 0);
  
  m_handle=0;
  int i=MAX_FILM_HANDLES;
  while(i--)m_handles[i]=0;
  m_numHandles=0;

  m_handles[m_numHandles]=new filmQT4L();    post("handle %d\t%X", m_numHandles, m_handles[m_numHandles]);m_numHandles++;
  m_handles[m_numHandles]=new filmAVI();    post("handle %d\t%X", m_numHandles, m_handles[m_numHandles]);m_numHandles++;
  m_handles[m_numHandles]=new filmAVIPLAY();  post("handle %d\t%X", m_numHandles, m_handles[m_numHandles]);m_numHandles++;
  m_handles[m_numHandles]=new filmFFMPEG();   post("handle %d\t%X", m_numHandles, m_handles[m_numHandles]);m_numHandles++;
  m_handles[m_numHandles]=new filmMPEG3();    post("handle %d\t%X", m_numHandles, m_handles[m_numHandles]);m_numHandles++;
  m_handles[m_numHandles]=new filmMPEG1();    post("handle %d\t%X", m_numHandles, m_handles[m_numHandles]);m_numHandles++;

  //openMess(filename);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_filmNEW :: ~pix_filmNEW()
{
  // Clean up the movie
  closeMess();
}

/////////////////////////////////////////////////////////
// closeMess
//
/////////////////////////////////////////////////////////
void pix_filmNEW :: closeMess(void){
  // Clean up any open files
  int i=MAX_FILM_HANDLES;
  while(i--){
    if(m_handles[i])m_handles[i]->close();
  }
  if (m_handle!=0) m_handle->close();
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////

void pix_filmNEW :: openMess(t_symbol *filename, int format)
{
  //  if (filename==x_filename)return;
  closeMess();

  char buf[MAXPDSTRING];
  canvas_makefilename(getCanvas(), filename->s_name, buf, MAXPDSTRING);

  int i=-1;
  post("opening %s with format %X", buf, format);
  while(i++<m_numHandles){
    if (m_handles[i] && m_handles[i]->open(buf, format ))      {
      m_handle = m_handles[i];
      break;
    }
  }

  if (!m_handle)return;

  t_atom ap[3];
  SETFLOAT(ap, m_handle->getFrameNum());
  SETFLOAT(ap+1, m_handle->getWidth());
  SETFLOAT(ap+2, m_handle->getHeight());
  m_numFrames=m_handle->getFrameNum();
  post("GEM: pix_filmNEW: Loaded file: %s with %d frames (%dx%d)", 
       buf, 
       m_handle->getFrameNum(), 
       m_handle->getWidth(), 
       m_handle->getHeight());
  outlet_list(m_outNumFrames, 0, 3, ap);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_filmNEW :: render(GemState *state)
{
  /* get the current frame from the file */
  if (!state || !m_handle)return;
  state->image=m_handle->getFrame();
  int frame=(int)m_reqFrame;
  if (state->image==0){
    outlet_float(m_outEnd,(m_numFrames>0 && (int)m_reqFrame<0)?(m_numFrames-1):0);
  }
  if (frame!=(int)m_reqFrame)render(state);//state->image=m_handle->getFrame();
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_filmNEW :: postrender(GemState *state)
{
  if (state && state->image)state->image->newimage = 0;
  // automatic proceeding
  if (m_auto!=0){
    if (m_handle&&m_handle->changeImage((int)(m_reqFrame+=m_auto))==FILM_ERROR_FAILURE){
      //      m_reqFrame = m_numFrames;
      outlet_bang(m_outEnd);
    }
  }
}

/////////////////////////////////////////////////////////
// changeImage
//
/////////////////////////////////////////////////////////
void pix_filmNEW :: changeImage(int imgNum, int trackNum)
{
  if (imgNum < 0){
    error("GEM: pix_filmNEW: selection number must be > 0");
    imgNum=0;
  }
  if (trackNum < 0){
    error("GEM: pix_filmNEW: track number must be > 0");
    trackNum=0;
  }
  if (m_handle)
    if (m_handle->changeImage(imgNum, trackNum)==FILM_ERROR_FAILURE){
      outlet_bang(m_outEnd);
    }
  m_reqFrame=imgNum;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_filmNEW :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_filmNEW::openMessCallback,
		  gensym("open"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmNEW::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmNEW::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
}
void pix_filmNEW :: openMessCallback(void *data, t_symbol*s,int argc, t_atom*argv)
{
  int format=0;
  if (argc!=1 && argc!=2)goto illegal_openmess;
  if (argv[0].a_type != A_SYMBOL)goto illegal_openmess;
  if (argc==2){
    if (argv[1].a_type != A_SYMBOL)goto illegal_openmess;
    else {
      char c =tolower(*atom_getsymbol(argv+1)->s_name);
      char c2=tolower(atom_getsymbol(argv+1)->s_name[3]);

      switch (c){
      case 'g': format=GL_LUMINANCE; break;
      case 'y': format=GL_YCBCR_422_GEM; break;
      case 'r':
	if (c2=='a')format=GL_RGBA;
	else format=GL_RGB;
	break;
#if defined (GL_BGRA) & defined (GL_BGR)
      case 'b':
	if (c2=='a')format=GL_BGRA;	else 
		format=GL_BGR;
	break;
#endif
      default: format=GL_RGBA;
      }
    }
  }
  GetMyClass(data)->openMess(atom_getsymbol(argv), format);

  return;
 illegal_openmess:
  error("pix_film: \"open <filename> [<format>]\"");
  return;
  
}

void pix_filmNEW :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv){
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_filmNEW :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=state;
}
