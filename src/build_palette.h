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
		void AddStructure(Structure *structure);
		void AddUserStructure(Structure *structure);

		void StructuresSaveCheck();

	private:
		void ElementSelected(wxCommandEvent& event);
		void OnCoordinationChoice(wxCommandEvent& event);
		void OnLPChoice(wxCommandEvent& event);
		void MouseMoved(wxMouseEvent& event);
		void OnClose(wxCloseEvent& event);
		wxPanel *GetPeriodicPanel(void);
		wxPanel *GetStructuresPanel(void);
		void SaveStructuresAs(wxCommandEvent& event);
		void SaveStructures(wxCommandEvent& event);
		long WriteCMLFile(BufferFile *Buffer) const;
		long ReadCMLFile(BufferFile *Buffer);
		bool LoadStructuresFromFile(const wxString& filename);
		void UpdateSaveStructures(wxUpdateUIEvent& event);
		void UpdateRenameStructures(wxUpdateUIEvent& event);
		void UpdateDeleteStructures(wxUpdateUIEvent& event);
		void DeleteStructure(wxCommandEvent& event);
		void RenameStructure(wxCommandEvent& event);
		void OnStructureChoice(wxCommandEvent& event);
		void TabChanged(wxNotebookEvent& event);
		void ChangeStructureGroup(wxCommandEvent& event);
		void DeleteAllStructures();

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
		std::vector<Structure *> structures;
		std::vector<Structure *> user_strucs;
		PreviewCanvas *canvas;
		wxString sys_prefs_path;

		bool strucs_in_mem;

		wxStaticText *element_label;
		wxStaticText *coord_num_label;
		wxStaticText *lp_num_label;

		wxGridBagSizer *struc_sizer;

		wxString struc_filename;
		wxStaticText *struc_filename_label;
		wxBoxSizer *struc_custom_sizer;
		wxChoice *struc_groups;

		wxButton *save_button;
		wxButton *save_as_button;
		wxButton *delete_button;
		wxButton *rename_button;

	DECLARE_DYNAMIC_CLASS(BuilderDlg)
	DECLARE_EVENT_TABLE()

};

#endif
