/////////////////////////////////////////////////////////////////////////////
// Name:        setPreference.cpp
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "setPreference.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes
#include "Globals.h"
#include "PrefsPanes.h"
//#include "MolDisplayWin.h"

#include "setPreference.h"


#define ADDED_PAGE_NAME wxT("Added ")

using namespace std;

extern WinPrefs *	gPreferences;
extern WinPrefs *	gPrefDefaults;

IMPLEMENT_DYNAMIC_CLASS( setPreference, wxDialog )

/*
 * SetPreference event table definition
 */

BEGIN_EVENT_TABLE( setPreference, wxDialog )

  EVT_CHOICEBOOK_PAGE_CHANGED(ID_NOTEBOOK, setPreference::OnChoicebook)
  EVT_CHOICEBOOK_PAGE_CHANGING(ID_NOTEBOOK, setPreference::OnChoicebook)
  //related to wxNoteBook

  EVT_BUTTON( myID_SETFONT, setPreference::OnSetFont )
  EVT_BUTTON( wxID_OK, setPreference::OnOK )

END_EVENT_TABLE()


/*!
 * SetPreference constructors
 */

setPreference::setPreference( )
{
}

setPreference::~setPreference( )
{
  delete mPrefs;

  /*if (m_imageList)
    {
        delete m_imageList;
        m_imageList = (wxImageList *) NULL;
    }
  */

}

setPreference::setPreference( MolDisplayWin* parent, bool isGlobal, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  mIsGlobal = isGlobal;

  mPrefs = new WinPrefs;

  if (isGlobal)
    *mPrefs = *gPreferences;
  else
    mPrefs = parent->GetPrefs();

  Create(parent, id, caption, pos, size, style);
}

/*!
 * setPreference creator
 */

bool setPreference::Create( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    mParent = parent;

    wxDialog::Create( parent, id, caption, pos, size, style );

    m_sizer = new wxBoxSizer(wxVERTICAL);
    m_choiceBook   = (wxChoicebook *)   NULL;
    m_panel      = (wxPanel *)      NULL;

    /*wxSize imageSize(32, 32);

    m_imageList
        = new wxImageList( imageSize.GetWidth(), imageSize.GetHeight() );

    m_imageList->Add
        (
            wxArtProvider::GetIcon(wxART_INFORMATION, wxART_OTHER, imageSize)
        );

    m_imageList->Add
        (
            wxArtProvider::GetIcon(wxART_QUESTION, wxART_OTHER, imageSize)
        );

    m_imageList->Add
        (
            wxArtProvider::GetIcon(wxART_WARNING, wxART_OTHER, imageSize)
        );

    m_imageList->Add
        (
            wxArtProvider::GetIcon(wxART_ERROR, wxART_OTHER, imageSize)
        );
    */

    m_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxTAB_TRAVERSAL | wxCLIP_CHILDREN | wxNO_BORDER | wxNO_FULL_REPAINT_ON_RESIZE);

    RecreateBooks();

    m_bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    m_buttFont = new wxButton(m_panel, myID_SETFONT, wxT("Set Font"));
    m_buttOK = new wxButton(m_panel, wxID_OK, wxT("OK"));
    m_buttCancel = new wxButton(m_panel, wxID_CANCEL, wxT("Cancel"));
    m_bottomSizer->Add(m_buttFont, wxSizerFlags().Border(wxALL, 7));
    m_bottomSizer->Add(m_buttCancel, wxSizerFlags().Border(wxALL, 7));
    m_bottomSizer->Add(m_buttOK, wxSizerFlags().Border(wxALL, 7));
    m_bottomSizer->Layout();
    m_sizer->Add(m_bottomSizer, wxSizerFlags().Center());

    m_panel->SetSizer(m_sizer);
    m_sizer->Fit(this);
    m_sizer->SetSizeHints(this);

    m_sizer->Layout();

    Centre(wxBOTH);

    return true;
}

/*
int setPreference::GetIconIndex(wxBookCtrlBase* bookCtrl)
{
    if (bookCtrl && bookCtrl->GetImageList())
    {
       int nImages = bookCtrl->GetImageList()->GetImageCount();

       if (nImages > 0)
       {
           return bookCtrl->GetPageCount() % nImages;
       }
    }

    return -1;
}
*/

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void setPreference::OnOK( wxCommandEvent& event )
{
  switch (currPanel)
    {
    case 0: atomPanel->saveToTempPrefs(); break;
    case 1: bondPanel->saveToTempPrefs(); break;
    case 2: displayPanel->saveToTempPrefs(); break;
    case 3: energyPanel->saveToTempPrefs(); break;
    case 4: filePanel->saveToTempPrefs(); break;
    case 5: scalPanel->saveToTempPrefs(); break;
    case 6: stereoPanel->saveToTempPrefs(); break;
    case 7: surfPanel->saveToTempPrefs(); break;
    case 8: qd3dPanel->saveToTempPrefs(); break;
    }

  *gPreferences = *mPrefs;
  gPreferences->WriteUserPrefs();

  Close();
}

void setPreference::RecreateBooks()
{ 
  wxChoicebook *oldBook = m_choiceBook;

  m_choiceBook = new wxChoicebook(m_panel, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT);

  //m_choiceBook->SetImageList(m_imageList);

  if (oldBook)
    {
      int sel = oldBook->GetSelection();
      int count = oldBook->GetPageCount();

      for (int n = 0; n < count; n++)
	{
	  wxString str = oldBook->GetPageText(n);
	  wxWindow *page = new wxPanel(m_choiceBook);
	  m_choiceBook->AddPage(page, str, false );
        }

      m_sizer->Detach(oldBook);
      delete oldBook;

      if (sel != wxNOT_FOUND)
	{
	  m_choiceBook->SetSelection(sel);
	}                                                                           }
  else
    {
      CreateInitialPages(m_choiceBook);
    }

  m_sizer->Insert(0, m_choiceBook, 5, wxEXPAND | wxALL, MARGIN);                    
  m_sizer->Show(m_choiceBook);
  m_sizer->Layout();
}
    

wxBookCtrlBase *setPreference::GetCurrentBook()
{
  return m_choiceBook;
}

void setPreference::CreateInitialPages(wxBookCtrlBase *parent)
{
    // Create and add some panels to the notebook
    atomPanel = new AtomPrefsPane(mParent, parent, mIsGlobal);
    atomPanel->SetupPaneItems(mParent, mPrefs);
    parent->AddPage( atomPanel, wxT("Elements"), false );

    bondPanel = new BondPrefsPane(mParent, parent, mIsGlobal);
    bondPanel->SetupPaneItems(mParent, mPrefs);
    parent->AddPage( bondPanel, wxT("Bonds/Vectors"), false );

    displayPanel = new DisplayPrefsPane(mParent, parent, mIsGlobal);
    displayPanel->SetupPaneItems(mParent, mPrefs);
    parent->AddPage( displayPanel, wxT("Display Options"), false );

    energyPanel = new EnergyPrefsPane(mParent, parent, mIsGlobal);
    energyPanel->SetupPaneItems(mParent, mPrefs);
    parent->AddPage( energyPanel, wxT("Energy Options"), false );

    filePanel = new FilePrefsPane(mParent, parent, mIsGlobal);
    filePanel->SetupPaneItems(mParent, mPrefs);
    parent->AddPage( filePanel, wxT("File"), false );

    scalPanel = new ScalingPrefsPane(mParent, parent, mIsGlobal);
    scalPanel->SetupPaneItems(mParent, mPrefs);
    parent->AddPage( scalPanel, wxT("Scaling/Speed"), false );

    stereoPanel = new StereoPrefsPane(mParent, parent, mIsGlobal);
    stereoPanel->SetupPaneItems(mParent, mPrefs);
    parent->AddPage( stereoPanel, wxT("Stereo Options"), false );

    surfPanel = new SurfacePrefsPane(mParent, parent, mIsGlobal);
    surfPanel->SetupPaneItems(mParent, mPrefs);
    parent->AddPage( surfPanel, wxT("Surface Options"), false );

    qd3dPanel = new QD3DPrefsPane(mParent, parent, mIsGlobal);
    qd3dPanel->SetupPaneItems(mParent, mPrefs);
    parent->AddPage( qd3dPanel, wxT("3D Options"), false );

    parent->SetSelection(0);
    currPanel = 0;
}

/*void setPreference::OnAddPage(wxCommandEvent& WXUNUSED(event))
{
    static unsigned s_pageAdded = 0;
 
    wxBookCtrlBase *currBook = GetCurrentBook();

    if ( currBook )
      {
        wxPanel *panel = new wxPanel( currBook, wxID_ANY );
        (void) new wxButton( panel, wxID_ANY, wxT("First button"),
            wxPoint(10, 10), wxDefaultSize );
        (void) new wxButton( panel, wxID_ANY, wxT("Second button"),
            wxPoint(50, 100), wxDefaultSize );

        currBook->AddPage(panel, wxString::Format(ADDED_PAGE_NAME wxT("%u"),
		  ++s_pageAdded), true, GetIconIndex(currBook) ); 
    }
}*/

void setPreference::OnSetFont(wxCommandEvent& WXUNUSED(event))
{
  wxFontData data;
  wxFontDialog dialog(this, data);

    if ( dialog.ShowModal() == wxID_OK )
    {
      wxFontData retData = dialog.GetFontData();
      wxFont myFont = retData.GetChosenFont();
    }
}

void setPreference::OnChoicebook(wxChoicebookEvent& event)
{
  int idx = event.GetOldSelection();
  
  switch (idx)
    {
    case 0: atomPanel->saveToTempPrefs(); break;
    case 1: bondPanel->saveToTempPrefs(); break;
    case 2: displayPanel->saveToTempPrefs(); break;
    case 3: energyPanel->saveToTempPrefs(); break;
    case 4: filePanel->saveToTempPrefs(); break;
    case 5: scalPanel->saveToTempPrefs(); break;
    case 6: stereoPanel->saveToTempPrefs(); break;
    case 7: surfPanel->saveToTempPrefs(); break;
    case 8: qd3dPanel->saveToTempPrefs(); break;
    }

  currPanel = event.GetSelection();

}
