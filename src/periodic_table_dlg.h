#ifndef _PERIODIC_TABLE_DLG_H_
#define _PERIODIC_TABLE_DLG_H_

/* ------------------------------------------------------------------------- */
/* FILE:   periodic_table_dlg.h                                              */
/* AUTHOR: Chris Johnson                                                     */
/* DATE:   Jan 24 2007                                                       */
/*                                                                           */
/*                                                                           */
/* ------------------------------------------------------------------------- */

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "periodic_table_dlg.h"
#endif

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/bitmap.h"
#include "wx/image.h"
#include "wx/dcmemory.h"
#include "wx/brush.h"

#include <stdio.h>

#include "Globals.h"
#include "mpGLCanvas.h"
#include "Prefs.h"

class MpGLCanvas;

/* ------------------------------------------------------------------------- */
/* TYPES                                                                     */
/* ------------------------------------------------------------------------- */

typedef struct {
   wxBitmapButton *button;
   wxBitmap *off_bmp;
   wxBitmap *on_bmp;
} element_t;

/* ------------------------------------------------------------------------- */
/* CLASSES                                                                   */
/* ------------------------------------------------------------------------- */

class PeriodicTableDlg: public wxDialog {    

   public:
      PeriodicTableDlg();
      PeriodicTableDlg(MpGLCanvas *parent,
                       const wxChar *title = wxT("Periodic Table"),
                       int xpos = 100, int ypos = 100);
      ~PeriodicTableDlg();

      bool Create(MpGLCanvas *parent,
                  const wxChar *title = wxT("Periodic Table"),
                  int xpos = 100, int ypos = 100);
      void New(wxCommandEvent& WXUNUSED(event));
      void ElementSelected(wxCommandEvent& event);
      void MouseMoved(wxMouseEvent& event);
      int GetSelectedID(void);
      void OnClose(wxCloseEvent& event);

   private:
      wxButton *button;
      element_t *elements;
      int nelements;
      int prev_id;
      MpGLCanvas *parent;
      void NumberToTableCoords(int atomic_number, int *row, int *col);
      int platform_offset;

   DECLARE_DYNAMIC_CLASS(PeriodicTableDlg)
   DECLARE_EVENT_TABLE()

};

#endif
