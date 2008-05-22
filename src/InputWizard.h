#ifndef INPUTWIZARD_H
#define INPUTWIZARD_H

#include <wx/wx.h>
#include <wx/wizard.h>

#define INPUTWIZ_NUM_PAGES 3

class InputWizard;
class InputWizardPage;

class InputWizard : public wxWizard {
	public:
		InputWizard(wxWindow *parent,
					int id = wxID_ANY);

		void RunWizard();

	private:
		InputWizardPage *pages[INPUTWIZ_NUM_PAGES];
};

class InputWizardPage : public wxWizardPageSimple {
	public:
		InputWizardPage(InputWizard *parent,
						InputWizardPage *prev,
						InputWizardPage *next);

	private:
};

#endif
