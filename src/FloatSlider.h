#ifndef FLOATSLIDER_H
#define FLOATSLIDER_H

#include <wx/wx.h>

/* ------------------------------------------------------------------------- */
/**
 * A compound widget that wraps around wxSlider to support single-precision
 * values.  The slider value can be adjusted via the slider itself and a
 * text control for more precise selection. The widget displays the min and
 * max values for the slider along with the current value in an edit field.
 */

class FloatSlider : public wxPanel {
	public:
		enum {
			POW = 0x1
		};

		FloatSlider(wxWindow *parent = NULL,
					int id = wxID_ANY,
					float val = 0.0f,
					float min = 0.0f,
					float max = 1.0f,
					int style = 0,
					const wxSize& size = wxDefaultSize);
		/// Return the current value of the control
		float GetValue() const;
		/// Set the control to the specified value.
		void SetValue(float val);
		/// Set the minimum value for the control
		void SetMin(float min);
		/// Set the maximum value for the control
		void SetMax(float max);
		/// Return the current maximum value for the control
		float GetMax() const;
		/// Return the current minimum value for the control
		float GetMin() const;
		/// Sync the slider setting after a changing the Min or Max.
		/// Also called internally to sync the slider after a change to the value field.
		void SyncSlider();

	private:
		/// Sync the test value after a change to the slider.
		void SyncText();
		/// Call back for a change in the slider
		void OnSliderChange(wxCommandEvent& event);
		/// Call back for a change in the edit text field.
		void OnTextChange(wxCommandEvent& event);
		/// Internal call to ensure the parent received the right control id.
		void OnTextEnter(wxCommandEvent& event);

		wxSlider *slider;
		wxStaticText *min_label;
		wxTextCtrl *val_box;
		wxStaticText *max_label;

		float min;
		float max;
		bool is_pow;

		DECLARE_EVENT_TABLE()
};

/* ------------------------------------------------------------------------- */

#endif
