#ifndef CHOOSEDIALOG_H
#define CHOOSEDIALOG_H

#include "wx/wx.h"
#include "wx/dialog.h"

class ChooseDialog : public wxDialog {
	public:
		ChooseDialog();
		ChooseDialog(wxWindow *parent, wxWindowID id, wxString& message,
					 int nitems, wxString *items);
		~ChooseDialog();
		int GetSelectedIndex();
		void SetSelectedIndex(int new_index);
		void Create(wxWindow *parent, wxWindowID id, wxString& message,
				    int nitems, wxString *items);

	private:
		int selected_idx;
		wxChoice *choices;

		void OnChange(wxCommandEvent& event);
		void OnMouse(wxMouseEvent& event);

	DECLARE_EVENT_TABLE()
};

#endif
