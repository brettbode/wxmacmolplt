#include "FloatSlider.h"

#define NTICS 1000

enum {
	ID_SLIDER,
	ID_VAL_BOX
};

/* ------------------------------------------------------------------------- */
/**
 * Construct a new single-precision slider object.
 * @param parent Containing window.
 * @param id Unique id of the slider.
 * @param val Initial value of slider.
 * @param min Minimum value of slider.
 * @param max Maximum value of slider.
 */

FloatSlider::FloatSlider(wxWindow *parent,
						 int id,
						 float val,
						 float min,
						 float max,
						 int style,
						 const wxSize& size)
	: wxPanel(parent, id, wxDefaultPosition, size),
	  min(min),
	  max(max),
	  is_pow(style & FloatSlider::POW) {

   assert(val >= min && val <= max);

   wxBoxSizer *vsizer = new wxBoxSizer(wxVERTICAL);
   wxGridSizer *hsizer = new wxGridSizer(1, 3, 0, 0);

   slider = new wxSlider(this, ID_SLIDER, 0, 0, NTICS, wxDefaultPosition, wxSize(100, wxDefaultCoord));
   slider->SetMinSize(slider->GetSize());
   vsizer->Add(slider, wxSizerFlags().Expand());

   wxString label;

   label.Printf(wxT("%.3f"), min);
   min_label = new wxStaticText(this, wxID_ANY, label);

   label.Printf(wxT("%.3f"), val);
   val_box = new wxTextCtrl(this, ID_VAL_BOX, label, wxDefaultPosition,
                            wxSize(50, wxDefaultCoord), wxTE_PROCESS_ENTER);

   label.Printf(wxT("%.3f"), max);
   max_label = new wxStaticText(this, wxID_ANY, label);

   hsizer->Add(min_label, wxSizerFlags().Align(wxALIGN_CENTER_VERTICAL)
                                        .Left());
   hsizer->Add(val_box, wxSizerFlags().Center());
   hsizer->Add(max_label, wxSizerFlags().Align(wxALIGN_CENTER_VERTICAL)
                                        .Right());

   vsizer->Add(hsizer, wxSizerFlags().Expand());

   SetMinSize(size);
   /* SetSizer(vsizer); */
   SetSizerAndFit(vsizer);

   // Only the text control contains the current value since the slider tracks
   // ints only (and we want floats).  So, for the most part, the text control
   // maintains the state and the slider is adjusted to reflect it.  Of course,
   // the user can adjust the slider directly, so in that case, the text
   // control must reflect the slider position.
   SyncSlider();

}

/* ------------------------------------------------------------------------- */
/**
 * Handles scroll events for the slider.
 * @param event The scroll event.
 */ 

void FloatSlider::OnSliderChange(wxCommandEvent& event) {

   // Text control needs updating too.
   SyncText();

   // Parent is expecting events with the panel id.
   event.SetId(GetId());
   event.Skip();

}

/* ------------------------------------------------------------------------- */
/**
 * Handles changes in the value text field.
 * @param event The text change event.
 */

void FloatSlider::OnTextChange(wxCommandEvent& event) {

   // Slider control needs updating too.
   SyncSlider();

   // Change must propogate up to parent.  The slider event handler does this,
   // so we just trigger a fake event on it.
   wxCommandEvent fake_event(wxEVT_COMMAND_SLIDER_UPDATED, GetId());
   GetEventHandler()->ProcessEvent(fake_event);

}

/* ------------------------------------------------------------------------- */
/**
 * Retrieve the current slider value.
 * @return The current value of the slider.
 */ 

float FloatSlider::GetValue() const {

   // The value is pulled from the text control, not the slider.
   double val;
   val_box->GetValue().ToDouble(&val);
   return (float) val;

}

/* ------------------------------------------------------------------------- */
/**
 * Set the value of the slider. Adjusts both text field and slider.
 * @param val New value of the slider.  Must be between min and max.
 */

void FloatSlider::SetValue(float val) {

   if (val < min || val > max) {
	  val = min;
   }
   /* assert(val >= min && val <= max); */

   wxString text;
   text.Printf(wxT("%.3f"), val);
#if wxCHECK_VERSION(2,9,0)
   val_box->ChangeValue(text);
#else
   val_box->SetValue(text);
#endif

   SyncSlider();

}

/* ------------------------------------------------------------------------- */
/**
 * Retrieve the slider's maximum value.
 * @return Maximum value.
 */

float FloatSlider::GetMax() const {

   return max;

}

/* ------------------------------------------------------------------------- */
/**
 * Retrieve the slider's minimum value.
 * @return Minimum value.
 */

float FloatSlider::GetMin() const {

   return min;

}

/* ------------------------------------------------------------------------- */
/**
 * Synchronize the slider control with the value contained in the text
 * control.
 */

void FloatSlider::SyncSlider() {

	int new_val;

	if (is_pow) {
		/* new_val = ((float) slider->GetValue() * slider->GetValue()) / (NTICS * NTICS) * (max - min) + min; */
		new_val = (int) sqrtf((GetValue() - min) / (max - min) * (NTICS * NTICS));
	} else {
		new_val = (int) ((GetValue() - min) / (max - min) * NTICS);
	}

	slider->SetValue(new_val);

}

/* ------------------------------------------------------------------------- */
/**
 * Synchronize the text control with the value contained in the slider
 * control.
 */

void FloatSlider::SyncText() {

	wxString text;
	float new_val;

	if (is_pow) {
		new_val = ((float) slider->GetValue() * slider->GetValue()) / (NTICS * NTICS) * (max - min) + min;
	} else {
		new_val = ((float) slider->GetValue()) / NTICS * (max - min) + min;
	}

	text.Printf(wxT("%.3f"), new_val);
#if wxCHECK_VERSION(2,9,0)
	val_box->ChangeValue(text);
#else
	val_box->SetValue(text);
#endif

}

/* ------------------------------------------------------------------------- */
/**
 * Assign slider's minimum value.
 * @param New minimum.
 */

void FloatSlider::SetMin(float min) {

	this->min = min;
	wxString str;
	str.Printf(wxT("%.3f"), min);
	min_label->SetLabel(str);

}

/* ------------------------------------------------------------------------- */
/**
 * Assign slider's maximum value.
 * @param New maximum.
 */

void FloatSlider::SetMax(float max) {

	this->max = max;
	wxString str;
	str.Printf(wxT("%.3f"), max);
	max_label->SetLabel(str);

}

/* ------------------------------------------------------------------------- */

BEGIN_EVENT_TABLE(FloatSlider, wxPanel)
   EVT_SLIDER(ID_SLIDER, FloatSlider::OnSliderChange)
   EVT_TEXT_ENTER(ID_VAL_BOX, FloatSlider::OnTextChange)
END_EVENT_TABLE()

/* ------------------------------------------------------------------------- */

