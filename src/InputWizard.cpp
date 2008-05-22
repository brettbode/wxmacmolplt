#include "InputWizard.h"

/* ------------------------------------------------------------------------- */
/**
 * Constructs a simple wizard to build a GAMESS input file.
 * @param parent Parent window
 * @param id Control id for event handling
 */
InputWizard::InputWizard(wxWindow *parent,
						 int id)
	: wxWizard(parent, id) {

	wxBoxSizer *box_sizer;

	pages[0] = new InputWizardPage(this, NULL, NULL);
	box_sizer = new wxBoxSizer(wxVERTICAL);
	box_sizer->Add(new wxStaticText(pages[0], wxID_ANY, wxT("first page")));
	pages[0]->SetSizerAndFit(box_sizer);

	pages[1] = new InputWizardPage(this, pages[0], NULL);
	pages[0]->SetNext(pages[1]);
	box_sizer = new wxBoxSizer(wxVERTICAL);
	box_sizer->Add(new wxStaticText(pages[1], wxID_ANY, wxT("second page")));
	pages[1]->SetSizerAndFit(box_sizer);

	pages[2] = new InputWizardPage(this, pages[1], NULL);
	pages[1]->SetNext(pages[2]);
	box_sizer = new wxBoxSizer(wxVERTICAL);
	box_sizer->Add(new wxStaticText(pages[2], wxID_ANY, wxT("third page")));
	pages[2]->SetSizerAndFit(box_sizer);

}

/* ------------------------------------------------------------------------- */
/**
 * Start the wizard at the first page.
 */

void InputWizard::RunWizard() {

	wxWizard::RunWizard(pages[0]);

}

/* ------------------------------------------------------------------------- */
/**
 * Constructs a InputWizardPage.
 * @param parent Parent wizard
 * @param prev Previous page; NULL if none.
 * @param next Next page; NULL if none.
 */

InputWizardPage::InputWizardPage(InputWizard *parent,
		                         InputWizardPage *prev,
								 InputWizardPage *next)
	: wxWizardPageSimple(parent, prev, next) {

}

#if 0
/* ------------------------------------------------------------------------- */
/**
 * Retrieve next page in wizard.
 * @return Pointer to next page, NULL if none.
 */

InputWizardPage *InputWizardPage::GetNext() const {

	return next;

}

/* ------------------------------------------------------------------------- */
/**
 * Retrieve previous page in wizard.
 * @return Pointer to previous page, NULL if none.
 */

InputWizardPage *InputWizardPage::GetPrev() const {

	return prev;

}
#endif

/* ------------------------------------------------------------------------- */

