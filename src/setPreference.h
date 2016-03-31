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
#include "wx/choicebk.h"
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
#define SYMBOL_SETPREFERENCE_SIZE wxSize(300, 490)
#define SYMBOL_SETPREFERENCE_POSITION wxDefaultPosition

#define MINIMUM_WINDOW_PREF_SIZE wxSize(490, 500)
#define ID_NOTEBOOK 20001
#define myID_SETFONT 30000
#define ID_FACTORY_DEFAULT 30001
#define ID_USER_DEFAULT 30002
#define ID_REVERT 30003
#define ID_APPLY 30004
////@end control identifiers
#define SYMBOL_GLOBALPREFERENCE_TITLE _("Global Preferences...")

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

    bool create( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style );

    /// wxEVT_CLOSE_WINDOW event handler for ID_COORDINATESDIALOG
    void OnCloseWindow( wxCloseEvent& event );
    void OnCancel( wxCommandEvent& event );

	/// Change the window title when the file name changes
	void UpdateWindowTitle(void);

////@begin setPreference event handler declarations

    void OnSetFont(wxCommandEvent& event);

    /// noteBook event
    void OnChoicebook(wxChoicebookEvent& event);

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOK( wxCommandEvent& event );
	/// Call back for when the factory defaults button is clicked.
	/// The function copies the defaults for the current pane into the
	/// temporary prefs for the pane.
    void facDefaults( wxCommandEvent& event );
	/// Call back for when the user defaults button is clicked.
	/// The function copies the user defaults for the current pane into the
	/// temporary prefs for the pane.
    void userDefaults( wxCommandEvent& event );
	/// Call back for the apply button. Pushes the prefs for the current pane
	/// to the active prefs for the window.
    void OnApply( wxCommandEvent& event );
	/// Call back for the revert button. Copies the active preferences for the
	/// window back into the temporary prefs for the active pane.
    void OnRevert( wxCommandEvent& event );
////@end setPreference event handler declarations

////@begin setPreference member function declarations

    MolDisplayWin * GetParent() const { return mParent ; }
    void SetParent(MolDisplayWin * value) { mParent = value ; }

    void RecreateBooks();
    wxBookCtrlBase *GetCurrentBook();
    void CreateInitialPages(wxBookCtrlBase *parent);
    void saveCurrPrefs(int panelID);
    void copyCurrPrefs(int panelID, WinPrefs* newPrefs);
    void updatePanels(int panelID);
    //int GetIconIndex(wxBookCtrlBase* bookCtrl);
////@end SetPreference member function declarations

 private:

    wxPanel *m_panel;
    wxChoicebook   *m_choiceBook;
    //wxNotebook* m_noteBook;
    wxBoxSizer *m_sizer;
    wxImageList *m_imageList;
    wxBoxSizer *m_bottomSizer;
    wxBoxSizer *m_midSizer;
    wxBoxSizer *m_midLeftSizer;
    wxBoxSizer *m_midRightSizer;

    wxButton* m_buttOK;
    wxButton* m_buttApply;
    wxButton* m_buttCancel;
    wxButton* m_buttFont;
    wxButton* m_buttFacDef;
    wxButton* m_buttUserDef;
    wxButton* m_buttRevert;

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
