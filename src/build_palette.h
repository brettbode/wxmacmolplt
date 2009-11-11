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

		void New(wxCommandEvent& WXUNUSED(event));
		int GetSelectedElement(void) const;
		Structure *GetSelectedStructure(void) const;
		int GetNumStructures(void) const;
		Structure *GetStructure(int i) const;
		short GetSelectedCoordination(void) const;
		short GetSelectedLonePairCount(void) const;
		static void NumberToTableCoords(int atomic_number, int *row, int *col);
		void KeyHandler(wxKeyEvent &event);
		bool InStructuresMode(void) const;
		bool InPeriodicMode(void) const;
		void AddUserStructure(Structure *structure);
		///Return the user structure with the specified fragName
		Structure * FindFragment(const std::string fragName) const;

	private:
		/// Adds the provided structure to the currently active prototype group (takes ownership of the structure)
		void AddStructure(Structure *structure);
		void ElementSelected(wxCommandEvent& event);
		void OnCoordinationChoice(wxCommandEvent& event);
		void OnLPChoice(wxCommandEvent& event);
		void MouseMoved(wxMouseEvent& event);
		void OnClose(wxCloseEvent& event);
		wxPanel *GetPeriodicPanel(void);
		wxPanel *GetStructuresPanel(void);
		void SaveStructures();
		void UpdateRenameStructures(wxUpdateUIEvent& event);
		void UpdateDeleteStructures(wxUpdateUIEvent& event);
		void DeleteStructure(wxCommandEvent& event);
		void RenameStructure(wxCommandEvent& event);
		void OnStructureChoice(wxCommandEvent& event);
		void TabChanged(wxNotebookEvent& event);	///< Called when switching between the element and prototype selection
		void ChangeStructureGroup(wxCommandEvent& event);	///< Called when the prototype group is changed

		short coordinationNumber[kNumTableElements];
		short LonePairCount[kNumTableElements];
		int nelements;
		int prev_id;
		unsigned char keyBuffer[3];
		wxStopWatch	secondKeytimer;
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
		int targetList;	///< Which of the structure groups is selected?
		std::vector<StructureGroup *> StructureGroups;	///< One group per molecule grouping in the builder
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

#endif
