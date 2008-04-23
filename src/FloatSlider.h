#ifndef FLOATSLIDER_H
#define FLOATSLIDER_H

#include <wx/wx.h>

/* ------------------------------------------------------------------------- */
/**
 * A compound widget that wraps around wxSlider to support single-precision
 * values.  The slider value can be adjusted via the slider itself and a
 * text control for more precise selection.
 */

class FloatSlider : public wxPanel {
   public:
      FloatSlider(wxWindow *parent = NULL, int id = wxID_ANY,
                  float val = 0.0f, float min = 0.0f, float max = 1.0f);
      float GetValue() const;
      void SetValue(float val);
      void SetMin(float min);
      void SetMax(float max);
      float GetMax() const;
      float GetMin() const;
      void SyncSlider();
      void SyncText();
      
   private:
      void OnSliderChange(wxCommandEvent& event);
      void OnTextChange(wxCommandEvent& event);

      wxSlider *slider;
      wxStaticText *min_label;
      wxTextCtrl *val_box;
      wxStaticText *max_label;

      float min;
      float max;

   DECLARE_EVENT_TABLE()
};

/* ------------------------------------------------------------------------- */

#endif
