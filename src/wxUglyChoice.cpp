#include "wxUglyChoice.h"
#include "xpms/arrow.xpm"

IMPLEMENT_DYNAMIC_CLASS(wxUglyChoice,wxPanel)

//BEGIN_EVENT_TABLE(wxUglyChoice, wxPanel)
//END_EVENT_TABLE()

wxUglyChoice::wxUglyChoice(wxWindow* parent,
                           wxWindowID id,
                           const wxPoint &pos,
                           const wxSize &size,
                           int n,
                           const wxString choices[],
                           long style) :
                  wxPanel(parent,
                          id,
                          pos,
                          size,
                          wxTAB_TRAVERSAL,
                          wxT("ugly_choice")) {

    this->SetSizeHints(size.GetWidth(), size.GetHeight());

    /* Initialize control elements */

    m_txt = new wxTextCtrl(this,
                           wxID_ANY,
                           wxT(""),
                           wxDefaultPosition,
                           wxDefaultSize,
                           wxTE_READONLY);

    wxBitmap btnBitmap(arrow_xpm);
    m_btn = new wxBitmapButton(this, wxID_ANY, btnBitmap);
    wxSize tSize = m_txt->GetSize();
    m_btn->SetSize(tSize.GetHeight(), tSize.GetHeight());
    btnID = m_btn->GetId();
    Connect(btnID,
            wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(wxUglyChoice::onButtonClick));

    item.clear();
    selection = -1;

    /* Add components to panel */

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(m_txt, 1, wxEXPAND);
    sizer->Add(m_btn);
    SetSizer(sizer);

    /* Add initial items to control */
    
    if(choices != NULL) {
        for(int i = 0; i < n; ++i) {
            Append(choices[i]);
        }
    }
}

wxUglyChoice::wxUglyChoice(wxWindow* parent,
                           wxWindowID id,
                           const wxPoint &pos,
                           const wxSize &size,
                           const wxArrayString & choices,
                           long style) :
						wxPanel(parent,
								id,
								pos,
								size,
								wxTAB_TRAVERSAL,
								wxT("ugly_choice")) {
	
    this->SetSizeHints(size.GetWidth(), size.GetHeight());
	
    /* Initialize control elements */
	
    m_txt = new wxTextCtrl(this,
                           wxID_ANY,
                           wxT(""),
                           wxDefaultPosition,
                           wxDefaultSize,
                           wxTE_READONLY);
	
    wxBitmap btnBitmap(arrow_xpm);
    m_btn = new wxBitmapButton(this, wxID_ANY, btnBitmap);
    wxSize tSize = m_txt->GetSize();
    m_btn->SetSize(tSize.GetHeight(), tSize.GetHeight());
    btnID = m_btn->GetId();
    Connect(btnID,
            wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(wxUglyChoice::onButtonClick));
	
    item.clear();
    selection = -1;
	
    /* Add components to panel */
	
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(m_txt, 1, wxEXPAND);
    sizer->Add(m_btn);
    SetSizer(sizer);
	
    /* Add initial items to control */
    
    if(!choices.IsEmpty()) {
        for(int i = 0; i < choices.GetCount(); ++i) {
            Append(choices.Item(i));
        }
    }
}

wxUglyChoice::~wxUglyChoice() {

}

int wxUglyChoice::Append(const wxString &item) {
    wxMenuItem *mi = NULL;

    if(!item.IsEmpty()) {
#if 0 //wxHAS_RADIO_MENU_ITEMS
        mi = m_menu.AppendRadioItem(wxID_ANY, item);
#else
        mi = m_menu.Append(wxID_ANY, item);
#endif
        this->item.push_back(mi->GetId());

        Connect(mi->GetId(),
                wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(wxUglyChoice::onMenuSelect));
    }
    else {
        m_menu.AppendSeparator();
    }
            
    // TODO:  Return valid value
    return 0;
}

int wxUglyChoice::Insert(const wxString &item, int pos) {
    wxMenuItem *mi = NULL;

    if(!item.IsEmpty()) {
#if 0 //wxHAS_RADIO_MENU_ITEMS
        mi = m_menu.InsertRadioItem(pos, wxID_ANY, item);
#else
        mi = m_menu.Insert(pos, wxID_ANY, item);
#endif
        this->item.insert(this->item.begin() + pos, mi->GetId());

        Connect(mi->GetId(),
                wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(wxUglyChoice::onMenuSelect));
    }
    else {
        m_menu.InsertSeparator(pos);
    }
            
    // TODO:  Return valid value
    return 0;
}

void wxUglyChoice::Delete(int n) {
    m_menu.Destroy(item[n]);
    Disconnect(item[n],
               wxEVT_COMMAND_MENU_SELECTED,
               wxCommandEventHandler(wxUglyChoice::onMenuSelect));
    item.erase(item.begin() + n);
}

void wxUglyChoice::Clear() {
    vector< int >::iterator i;

    for(i = item.begin(); i != item.end(); ++i) {
        m_menu.Destroy(*i);
        Disconnect(*i,
                   wxEVT_COMMAND_MENU_SELECTED,
                   wxCommandEventHandler(wxUglyChoice::onMenuSelect));
    }
    item.clear();
}

bool wxUglyChoice::IsEmpty() const {
    return item.empty();
}

int wxUglyChoice::GetCount() const {
    return item.size();
}

void wxUglyChoice::SetToolTip(const wxString& tip) {
    m_txt->SetToolTip(tip);
    m_btn->SetToolTip(tip);
}

void wxUglyChoice::SetEnabled(int n, bool enabled) {
    m_menu.Enable(item[n], enabled);
}

bool wxUglyChoice::GetEnabled(int n) const {
    return m_menu.IsEnabled(item[n]);
}

bool wxUglyChoice::SetSelection(int n) {
	if ((n<0)||(n>item.size())) return false;
    selection = n;
    m_txt->SetValue(m_menu.GetLabel(item[selection]));
#if 0 //wxHAS_RADIO_MENU_ITEMS
    m_menu.Check(item[selection], true);
#endif

    return true;
}

bool wxUglyChoice::SetStringSelection(const wxString &string) {
    int id = 0;
    if((id = m_menu.FindItem(string)) == wxNOT_FOUND) {
        return false;
    }
    for(int i = 0; i < item.size(); ++i) {
        if(item[i] == id) {
            return SetSelection(i);
        }
    }
	return false;
}

int wxUglyChoice::GetSelection() const {
    return selection;
}

wxString wxUglyChoice::GetString(int n) const {
    return m_menu.GetLabel(item[n]);
}

wxString wxUglyChoice::GetStringSelection() const {
    return m_menu.GetLabel(item[selection]);
}

void wxUglyChoice::onButtonClick(wxCommandEvent &event) {
    PopupMenu(&m_menu);
}

void wxUglyChoice::onMenuSelect(wxCommandEvent &event) {
    wxCommandEvent evt(wxEVT_COMMAND_CHOICE_SELECTED, GetId());
    unsigned int i = 0;

    for(i = 0; i < item.size(); i++) {
        if(item[i] == event.GetId()) break;
    }

    SetSelection(i);
    evt.SetInt(selection);
    evt.SetString(GetStringSelection());
    wxPostEvent(this, evt);
}

