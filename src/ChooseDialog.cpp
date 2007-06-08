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

	wxBoxSizer *main_sizer, *upper_sizer, *lower_sizer;
	wxButton *button_ok, *button_cancel;

	wxDialog::Create(parent, id, wxT("select..."), wxDefaultPosition,
					 wxDefaultSize);

	selected_idx = 0;

	choices = new wxChoice(this, CHOICES_ID, wxPoint(30, 30),
						   wxSize(200, wxDefaultCoord), nitems, items);
	choices->SetSelection(0);

	button_ok = new wxButton(this, wxID_OK, wxT("OK"));
	button_cancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));

	main_sizer = new wxBoxSizer(wxVERTICAL); 
	upper_sizer = new wxBoxSizer(wxVERTICAL); 
	lower_sizer = new wxBoxSizer(wxHORIZONTAL); 

	upper_sizer->Add(choices, 0, wxALIGN_CENTRE | wxALL, 20); 
	upper_sizer->Add(8, 8); 
	lower_sizer->Add(10, 10); 
	lower_sizer->Add(button_ok, 0, wxALIGN_CENTRE | wxALL, 10); 
	lower_sizer->Add(button_cancel, 0, wxALIGN_CENTRE | wxALL, 10); 

	main_sizer->Add(upper_sizer); 
	main_sizer->Add(lower_sizer); 

	main_sizer->Layout(); 
	SetSizer(main_sizer); 
	Centre(wxBOTH); 

}

// --------------------------------------------------------------------------- 

void ChooseDialog::OnMouse(wxMouseEvent& event) {
	return;
}

// --------------------------------------------------------------------------- 

BEGIN_EVENT_TABLE(ChooseDialog, wxDialog)
	EVT_CHOICE(CHOICES_ID, ChooseDialog::OnChange)
	EVT_MOUSE_EVENTS(ChooseDialog::OnMouse)
END_EVENT_TABLE()
