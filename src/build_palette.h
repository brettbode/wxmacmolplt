#ifndef BUILD_PALETTE_H
#define BUILD_PALETTE_H

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/gbsizer.h>

#include "wx/bitmap.h"
#include "wx/image.h"
#include "wx/dcmemory.h"
#include "wx/brush.h"
#include "wx/minifram.h"
#include "wx/notebook.h"

#include <stdio.h>

#include "Globals.h"
#ifndef __MyTypes__
#include "MyTypes.h"
#endif
#include "Prefs.h"
#include "preview_canvas.h"

#define kNumTableElements	112
#define kNUM_STRUC_GROUPS 4

/* ------------------------------------------------------------------------- */
/* TYPES                                                                     */
/* ------------------------------------------------------------------------- */

typedef struct {
	wxBitmapButton *button;
	wxBitmap *off_bmp;
	wxBitmap *on_bmp;
} element_t;

class PreviewCanvas;

class Structure {
	friend std::ostream& operator<<(std::ostream& stream, const Structure& s);

	public:
		mpAtom *atoms;
		int natoms;
		Bond *bonds;
		int nbonds;
		std::string FragName;
		std::string frag_def;
		wxString name;
		int atom_to_prune;
		int link_atom;
		int link_site;

		Structure();
		~Structure();

		void WriteXML(XMLElement *parent) const;
		bool ReadXML(XMLElement *struc_el);
		void SetPruneAtom(int atom_id);

	private:
};

class StructureGroup {
	public:
		std::vector<Structure *> structures;
		wxString filename;
		bool is_custom;
	
	StructureGroup(const wxString & fn, bool custom);
	~StructureGroup(void);
	long WriteCMLFile() const;
	long ReadCMLFile(void);
};

/* ------------------------------------------------------------------------- */
/* CLASSES                                                                   */
/* ------------------------------------------------------------------------- */

class BuilderDlg: public wxMiniFrame {    

	public:
		BuilderDlg(
			const wxString& title = _("Periodic Table"), int xpos = 100,
			int ypos = 100);
		~BuilderDlg();

		static void NumberToTableCoords(int atomic_number, int *row, int *col);
		void KeyHandler(wxKeyEvent &event);
		bool InStructuresMode(void) const;
		bool InPeriodicMode(void) const;
		void AddUserStructure(Structure *structure);
		void ElementSelected(wxCommandEvent& event);

	private:
		void OnCoordinationChoice(wxCommandEvent& event);
		void OnLPChoice(wxCommandEvent& event);
		void MouseMoved(wxMouseEvent& event);
		void OnClose(wxCloseEvent& event);
		wxPanel *GetPeriodicPanel(void);
		wxPanel *GetStructuresPanel(void);
		void UpdateRenameStructures(wxUpdateUIEvent& event);
		void UpdateDeleteStructures(wxUpdateUIEvent& event);
		void DeleteStructure(wxCommandEvent& event);
		void RenameStructure(wxCommandEvent& event);
		void OnStructureChoice(wxCommandEvent& event);
		void TabChanged(wxNotebookEvent& event);	///< Called when switching between the element and prototype selection
		void ChangeStructureGroup(wxCommandEvent& event);	///< Called when the prototype group is changed

		int nelements;
		int prev_id;
		wxStaticText *mTextArea;
		wxChoice *mCoordinationChoice;
		wxChoice *mLPChoice;
		element_t *elements;
		wxNotebook *tabs;
		wxPanel *periodic_panel;
		wxPanel *structures_panel;
		wxPanel *canvas_panel;
		wxFlexGridSizer *canvas_panel_sizer;
		wxListBox *mStructureChoice;
		PreviewCanvas *canvas;	///< Display area used for the prototypes
		wxString sys_prefs_path;

		wxStaticText *element_label;
		wxStaticText *coord_num_label;
		wxStaticText *lp_num_label;

		wxGridBagSizer *struc_sizer;

		wxBoxSizer *struc_custom_sizer;
		wxChoice *struc_groups;

		wxButton *delete_button;
		wxButton *rename_button;

	DECLARE_DYNAMIC_CLASS(BuilderDlg)
	DECLARE_EVENT_TABLE()

};

class BuilderInterface {
public:
	BuilderInterface();
	~BuilderInterface();
	
	/// Show or Hide the bulder tool palate on the screen
	void ShowPalette(bool state);
	/// Is the palette window shown?
	bool IsPaletteVisible(void) const;
	/// Call to close the palette window
	void ClosePalette(void);
	/// Return the atomic number of the currently selected element (or 0 if none selected)
	int GetSelectedElement(void) const;
	/// Obtain a pointer to the currently selected structure (NULL if none is selected)
	Structure *GetSelectedStructure(void) const;
	/// Obtain a pointer to the ith structure in the current group (NULL if i is invalid)
	Structure *GetStructure(int i) const;
	/// Return the coordination number for the selected element
	short GetSelectedCoordination(void) const;
	/// Return the long pair count for the selected element
	short GetSelectedLonePairCount(void) const;
	/// Keyhandler whether the palette is active or not
	void KeyHandler(wxKeyEvent &event);
	bool InStructuresMode(void) const {return (currentTab!=0);};
	bool InPeriodicMode(void) const {return (currentTab==0);};
	/// Call (by the palette window) when the active tab is changed
	void TabChanged(int index);
	/// Add the provided structure to the user structure group
	void AddUserStructure(Structure *structure);
	/// Delete the specified structure
	void DeleteStructure(int index);
	/// Rename the indicated structure
	void RenameStructure(int index, wxString & new_name);
	///Return the user structure with the specified fragName
	Structure * FindFragment(const std::string fragName) const;
	/// Select the desired element
	void ElementSelected(int elementType);
	/// Is the current structure group editable?
	bool currentGroupIsCustom(void) const;
	/// Change the target group to the new selection
	void ChangeTargetGroup(int new_group);
	/// Change the target structure to the new index
	int ChangeTargetStructure(int index);
	/// Obtain the index of the selected structure group
	inline int getTargetGroup(void) const {return targetList;};
	/// Obtain the selected structure index
	inline int getSelectedStructureIndex(void) const {return selectedStructure;};
	/// Build out the list of structure names
	void BuildNameList(wxListBox * theList) const;
	/// Set the coordination number for the selected element
	void SetSelectedCoordinationNumber(int val);
	/// Set the lone pair count for the selection element
	void SetSelectedLPCount(int val);

private:
	/// Call to save the currently selected structure group to file
	void SaveStructures();

	short coordinationNumber[kNumTableElements];
	short LonePairCount[kNumTableElements];
	int nelements;
	int selectedElement;
	unsigned char keyBuffer[3];
	wxStopWatch	secondKeytimer;
	element_t *elements;
	int targetList;	///< Which of the structure groups is selected?
	int selectedStructure; ///< Index of the selected structure in the currently selected group
	std::vector<StructureGroup *> StructureGroups;	///< One group per molecule grouping in the builder
	wxString sys_prefs_path;
	int currentTab;			///< Currently active panel (periodic table or structures)
	
	BuilderDlg * BuilderPalette;
};

#endif
