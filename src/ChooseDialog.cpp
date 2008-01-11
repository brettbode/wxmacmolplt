#include "ChooseDialog.h"

#define CHOICES_ID 20000

// --------------------------------------------------------------------------- 

ChooseDialog::ChooseDialog() {
}

// --------------------------------------------------------------------------- 

ChooseDialog::ChooseDialog(wxWindow *parent, wxWindowID id, wxString& message,
						   int nitems, wxString *items) {
	Create(parent, id, message, nitems, items);
}

// --------------------------------------------------------------------------- 

ChooseDialog::~ChooseDialog() {
}

// --------------------------------------------------------------------------- 

int ChooseDialog::GetSelectedIndex() {
	return selected_idx;
}

// --------------------------------------------------------------------------- 

void ChooseDialog::SetSelectedIndex(int new_index) {
	selected_idx = new_index;
	choices->SetSelection(new_index);
}

// --------------------------------------------------------------------------- 

void ChooseDialog::OnChange(wxCommandEvent& event) {
	selected_idx = event.GetInt();
}

// --------------------------------------------------------------------------- 

void ChooseDialog::Create(wxWindow *parent, wxWindowID id, wxString& message,
						  int nitems, wxString *items) {

	wxBoxSizer *upper_sizer, *lower_sizer;
	wxButton *button_ok, *button_cancel;

	wxDialog::Create(parent, id, wxT("select..."), wxDefaultPosition,
					 wxSize(-1, -1));

	selected_idx = 0;

	choices = new wxChoice(this, CHOICES_ID, wxPoint(-1, -1),
						   wxSize(-1, -1), nitems, items);
	choices->SetSelection(0);

	button_ok = new wxButton(this, wxID_OK, wxT("OK"));
	button_cancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));

	upper_sizer = new wxBoxSizer(wxVERTICAL); 
	lower_sizer = new wxBoxSizer(wxHORIZONTAL); 

	upper_sizer->AddSpacer(1);
	upper_sizer->Add(new wxStaticText(this, -1, message), 0, 
		wxALL | wxALIGN_LEFT, 3);
	upper_sizer->Add(choices, 0, wxALIGN_CENTRE | wxALL, 20); 

	lower_sizer->Add(button_ok, 0, wxALIGN_CENTRE | wxALL, 0); 
	lower_sizer->AddSpacer(3);
	lower_sizer->Add(button_cancel, 0, wxALIGN_CENTRE | wxALL, 0); 

	upper_sizer->Add(lower_sizer, 0, wxALIGN_CENTRE | wxALL, 0);
	upper_sizer->AddSpacer(4);

	// main_sizer->Layout();  
	upper_sizer->Fit(this);
	SetSizer(upper_sizer); 
	Centre(wxBOTH); 

}

// --------------------------------------------------------------------------- 

void ChooseDialog::OnMouse(wxMouseEvent& event) {
	return;
}

// --------------------------------------------------------------------------- 

BEGIN_EVENT_TABLE(ChooseDialog, wxDialog)
	EVT_CHOICE(CHOICES_ID, ChooseDialog::OnChange)
	// EVT_MOUSE_EVENTS(ChooseDialog::OnMouse) 
END_EVENT_TABLE()

// --------------------------------------------------------------------------- 

