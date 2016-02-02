/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	PrefPanes.h    -- on wxWidgets 
*/

#ifndef __PrefPanes__
#define __PrefPanes__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "PrefsPanes.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/wx.h"
#include <wx/bookctrl.h>
#include <wx/fontdlg.h>
#include "MolDisplayWin.h"
#include <wx/spinctrl.h>
////@end includes

class colorArea;
class colorPatternArea;

#define NUM_ATOM_LABELS 6

class PrefsPane : public wxPanel {
	public:
		//Constructor should add the needed pane items and do one time inits
		PrefsPane(MolDisplayWin* targetWindow, WinPrefs* targetPrefs, short PaneID, Boolean GlobalPrefs, wxBookCtrlBase *parent);

		~PrefsPane(); 

		//setupitems should do all inits that might/can be done more than once
		//like setting control values
		virtual void SetupPaneItems(MolDisplayWin* targetWindow)=0;

		virtual void saveToTempPrefs()=0;

		Boolean PrefsAreGlobal(void) {return isGlobalPrefs;};

	private:
		Boolean isGlobalPrefs;

	protected:
		WinPrefs* mTargetPrefs;
};

class AtomPrefsPane : public PrefsPane {
	public:
		AtomPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs);
		~AtomPrefsPane();
		virtual void SetupPaneItems(MolDisplayWin* targetWindow);
		virtual void saveToTempPrefs();

	private:
		wxScrolledWindow *scroll_win;
		short	ActiveEdit;
		wxString mLabels[NUM_ATOM_LABELS];
		
		wxFlexGridSizer* mMainSizer;

		wxTextCtrl* mEleNames[kMaxAtomTypes];
		wxTextCtrl* mEleSizes[kMaxAtomTypes];
		wxTextCtrl* mEleMasses[kMaxAtomTypes];
		colorArea* mColorArea[kMaxAtomTypes];
		colorPatternArea* mPatternArea[kMaxAtomTypes];
};


class BondPrefsPane : public PrefsPane {
	public:
		BondPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs);
		~BondPrefsPane();
		virtual void SetupPaneItems(MolDisplayWin* targetWindow);
		virtual void saveToTempPrefs();

		void OnChoice( wxCommandEvent &event );
		void OnSliderUpdate( wxCommandEvent &event);
		void OnToggleAnim(wxCommandEvent& event);
		void OnToggleRotation(wxCommandEvent& event);

	private:
		wxBoxSizer* mMainSizer;
		wxBoxSizer* mUpperSizer;
		wxBoxSizer* mSldSizer;
		wxStaticBoxSizer* mBox;
		wxBoxSizer* mLowerSizer;
		wxFlexGridSizer* mInnerSizer;

		wxCheckBox* mChkAnim;
		wxCheckBox* mChkRotation;
		wxSlider* mSldScale;
		wxChoice* mBondChoice;
		colorArea *mColorArea, *mNormColorArea;

		int mChoiceId;

		DECLARE_EVENT_TABLE()
};

class DisplayPrefsPane : public PrefsPane {
	public:
		DisplayPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs);
		virtual void SetupPaneItems(MolDisplayWin* targetWindow);
		virtual void saveToTempPrefs();

		void OnRadio( wxCommandEvent &event );
		void OnLabelsRadio( wxCommandEvent &event );
		void OnCheckBox( wxCommandEvent& WXUNUSED(event));
		void OnAtomLabelSlider(wxCommandEvent &event);
		void OnAnnotationLabelSlider(wxCommandEvent &event);

	private:
		wxBoxSizer* mMainSizer;

		wxRadioBox* mRdoBox;
		wxCheckBox* mChkColor;
		wxRadioBox*	mAtomLabels;
		wxSlider*	mAtomLabelSizeSlider;
		wxSlider*	mAnnotationLabelSizeSlider;

		DECLARE_EVENT_TABLE()
};


class EnergyPrefsPane : public PrefsPane {
	public:
		EnergyPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs);
		~EnergyPrefsPane();
		virtual void SetupPaneItems(MolDisplayWin* targetWindow);
		virtual void saveToTempPrefs();

		void OnSliderUpdate( wxCommandEvent &WXUNUSED(event) );
		void OnRadio( wxCommandEvent &event );
		void OnCheckBox( wxCommandEvent& event);

	private:

		void setHiddenCtrls();

		wxBoxSizer* mMainSizer;
		wxBoxSizer* mUpperSizer;
		wxGridSizer* mLowerSizer;
		wxBoxSizer* mBottomSizer;
		wxBoxSizer* mLeftBottomSizer;
		wxBoxSizer* mRightBottomSizer;
		wxGridSizer* mRight1BottomSizer;
		wxGridSizer* mRight2BottomSizer;

		wxSlider* mSldBallSize;
		wxTextCtrl* mNumDigitsArea;
		colorArea* mEColor;
		colorArea* mMPColor;
		colorArea* mPEColor;
		colorArea* mOtherColor;
		wxTextCtrl* mY1pt;
		wxTextCtrl* mY2pt;
		wxCheckBox* mTotalEny;
		wxCheckBox* mMP2Eny;
		wxCheckBox* mPotEny;
		wxRadioBox* mRdoUnit;
		wxRadioBox* mRdoMisc;
		wxTextCtrl* mAtomText[3];

		short	ActiveEdit;

		DECLARE_EVENT_TABLE()
};


class FilePrefsPane : public PrefsPane {
	public:
		FilePrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs);
		virtual void SetupPaneItems(MolDisplayWin* targetWindow);
		virtual void saveToTempPrefs();

		void OnCheckBox( wxCommandEvent& WXUNUSED(event));
		void OnSliderUpdate( wxCommandEvent &WXUNUSED(event) );

	private:
		wxBoxSizer* mMainSizer;

		wxCheckBox* mChkBox[9];
		wxSlider* mSldTol;
		wxTextCtrl* mPointSkip;

		short	ActiveEdit;
		DECLARE_EVENT_TABLE()
};


class ScalingPrefsPane : public PrefsPane {
	public:
		ScalingPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs);
		virtual void SetupPaneItems(MolDisplayWin* targetWindow);
		virtual void saveToTempPrefs();

		void OnSliderUpdate( wxCommandEvent &event );

	private:
		wxFlexGridSizer* mMainSizer;

		wxSlider* mSld[3];

		DECLARE_EVENT_TABLE()
};


class StereoPrefsPane : public PrefsPane {
	public:
		StereoPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs);
		virtual void SetupPaneItems(MolDisplayWin* targetWindow);
		virtual void saveToTempPrefs();

		void OnCheckBox( wxCommandEvent& WXUNUSED(event));

	private:
		wxBoxSizer* mMainSizer;
		wxBoxSizer* mMiddleSizer;

		wxCheckBox* mChkActive;
		wxTextCtrl* mOffDegree;

		short	ActiveEdit;

		DECLARE_EVENT_TABLE()
};


class SurfacePrefsPane : public PrefsPane {
	public:
		SurfacePrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs);
		~SurfacePrefsPane();
		virtual void SetupPaneItems(MolDisplayWin* targetWindow);
		virtual void saveToTempPrefs();

		void OnCheckBox( wxCommandEvent& event);
		void OnSliderUpdate( wxCommandEvent &event );

	private:
		wxBoxSizer* mMainSizer;
		wxBoxSizer* mLeftSizer;
		wxBoxSizer* mRightSizer;
		wxGridSizer* mLeftMidSizer;
		wxBoxSizer* mRightUpperSizer;
		wxFlexGridSizer* mRightMidSizer;

		colorArea* mSurfColor[2];
		wxSlider* mSldGridPoint;
		wxSlider* mSldGridSize;
		wxTextCtrl* mNumContour;
		wxTextCtrl* mMaxContourValue;
		wxCheckBox* mZeroContour;
		wxCheckBox* mAtomPosition;

		short	ActiveEdit;

		DECLARE_EVENT_TABLE()
};


class QD3DPrefsPane : public PrefsPane {
	public:
		QD3DPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs);
		virtual void SetupPaneItems(MolDisplayWin* targetWindow);
		virtual void saveToTempPrefs();

		void OnShaderChoice(wxCommandEvent& event);
		void OnSliderUpdate( wxCommandEvent &event );
		void OnSpinChange(wxSpinEvent & event);

	private:
		wxFlexGridSizer *mMainSizer;

		colorArea* mBackgrdColor;
		wxChoice *mShaderChooser;
		wxSlider* mSld[6];
		wxCheckBox* mAutoRotateCheck;

		DECLARE_EVENT_TABLE()
};

#endif
