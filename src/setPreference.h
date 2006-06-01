/////////////////////////////////////////////////////////////////////////////
// Name:        setPreference.h
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SETPREFERENCE_H_
#define _SETPREFERENCE_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "setPreference.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/sizer.h"
#include "wx/gbsizer.h"
//#include "wx/listbook.h"
//#include "wx/notebook.h"
#include "wx/choicebk.h"
#include "wx/imaglist.h"
#include "wx/artprov.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class AtomPrefsPane;
class BondPrefsPane;
class DisplayPrefsPane;
class EnergyPrefsPane;
class FilePrefsPane;
class ScalingPrefsPane;
class StereoPrefsPane;
class SurfacePrefsPane;
class QD3DPrefsPane;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_MY_PRE_DIALOG 10033
#define SYMBOL_SETPREFERENCE_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SETPREFERENCE_TITLE _("Preferences...")
#define SYMBOL_SETPREFERENCE_IDNAME ID_MY_PRE_DIALOG
#define SYMBOL_SETPREFERENCE_SIZE wxSize(40, 30)
#define SYMBOL_SETPREFERENCE_POSITION wxDefaultPosition
#define MARGIN 4

#define ID_LISTBOOK 20000
#define ID_NOTEBOOK 20001
#define myID_SETFONT 30000
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * SetPreference class declaration
 */

class setPreference : public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( setPreference )

public:
    /// Constructors
    setPreference( );
    setPreference( MolDisplayWin* parent, bool isGlobal, wxWindowID id = SYMBOL_SETPREFERENCE_IDNAME, const wxString& caption = SYMBOL_SETPREFERENCE_TITLE, const wxPoint& pos = SYMBOL_SETPREFERENCE_POSITION, const wxSize& size = SYMBOL_SETPREFERENCE_SIZE, long style = SYMBOL_SETPREFERENCE_STYLE );
    // Destructor
    ~setPreference();

    bool Create( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style );

////@begin setPreference event handler declarations

    //void OnAddPage(wxCommandEvent& event);
    //we don't need adding pages for the moment

    void OnSetFont(wxCommandEvent& event);

    /// noteBook event
    void OnChoicebook(wxChoicebookEvent& event);

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOK( wxCommandEvent& event );

////@end setPreference event handler declarations

////@begin setPreference member function declarations

    MolDisplayWin * GetParent() const { return mParent ; }
    void SetParent(MolDisplayWin * value) { mParent = value ; }

    void RecreateBooks();
    wxBookCtrlBase *GetCurrentBook();
    void CreateInitialPages(wxBookCtrlBase *parent);
    //int GetIconIndex(wxBookCtrlBase* bookCtrl);
////@end SetPreference member function declarations

 private:

    wxPanel *m_panel;
    wxChoicebook   *m_choiceBook;
    //wxNotebook* m_noteBook;
    wxBoxSizer *m_sizer;
    wxImageList *m_imageList;
    wxBoxSizer *m_bottomSizer;
    wxButton* m_buttOK;
    wxButton* m_buttCancel;
    wxButton* m_buttFont;

    AtomPrefsPane *atomPanel;
    BondPrefsPane *bondPanel;
    DisplayPrefsPane *displayPanel;
    EnergyPrefsPane *energyPanel;
    FilePrefsPane* filePanel;
    ScalingPrefsPane* scalPanel;
    StereoPrefsPane* stereoPanel;
    SurfacePrefsPane* surfPanel;
    QD3DPrefsPane* qd3dPanel;

    MolDisplayWin * mParent;
    WinPrefs* mPrefs;
    bool mIsGlobal;

    int currPanel;

   DECLARE_EVENT_TABLE() 
};

#endif
