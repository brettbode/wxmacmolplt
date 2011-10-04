/*
 * (c) 2005 Iowa State University
 *     see the LICENSE file in the top level directory
 */

/***************************************
 * mpGLCanvas.h
 *
 * Defines a wxWidgets component that displays a 3-dimensional representation
 * of a molecule.
 ***************************************/

#ifndef MPGLCANVAS_H
#define MPGLCANVAS_H 1

#include "wx/wx.h"
#include "mmp_gl.h"
#include "wx/popupwin.h"
#include "wx/generic/dragimgg.h"
#define wxDragImage wxGenericDragImage

#include "MoleculeData.h"
#include "Frame.h"

#define NEW_ATOM_TYPE_DIALOG 30001
#define NEW_ATOM_TYPE_TITLE _("Select an Atom type")
#define ID_NEW_ATOM_TYPE_CHOICE 30010
#define GL_Popup_Menu_Apply_All 30011
#define GL_Popup_Delete_Item_Current_Frame 30012
#define GL_Popup_Delete_Item_All_Frames 30013
#define GL_Popup_To_Hydrogen_Bond 30014
#define GL_Popup_To_Single_Bond 30015
#define GL_Popup_To_Double_Bond 30016
#define GL_Popup_To_Triple_Bond 30017
#define GL_Popup_To_Aromatic_Bond 30018
#define GL_Popup_Mark_Atom 30019
#define GL_Popup_Measure_Length 30020
#define GL_Popup_Measure_Angle 30021
#define GL_Popup_Measure_Dihedral 30022
#define GL_Popup_Unmark_Atom 30023
#define GL_Popup_Delete_Length 30024
#define GL_Popup_Delete_Angle 30025
#define GL_Popup_Delete_Dihedral 30026
#define GL_Popup_Delete_Bond 30027
#define GL_Popup_Lock_To_Annotation 30028
#define GL_Popup_Fit_To_Plane 30029
#define GL_Popup_Change_Atom 30030
#define GL_Popup_Change_Coord_Num 30031
#define GL_Popup_Set_Anno_Param 30032
#define GL_Popup_Change_LPCount 30033
#define GL_Popup_To_Coordination_Zero 30034
#define GL_Popup_To_Coordination_One 30035
#define GL_Popup_To_Coordination_Two 30036
#define GL_Popup_To_Coordination_Three 30037
#define GL_Popup_To_Coordination_Four 30038
#define GL_Popup_To_Coordination_Five 30039
#define GL_Popup_To_Coordination_Six 30040
#define GL_Popup_To_Coordination_Seven 30041
#define GL_Popup_To_LPCount_Zero 30042
#define GL_Popup_To_LPCount_One 30043
#define GL_Popup_To_LPCount_Two 30044
#define GL_Popup_To_LPCount_Three 30045
#define GL_Popup_To_LPCount_Four 30046
#define GL_Popup_To_LPCount_Five 30047
#define GL_Popup_Paste_At 30048
#define GL_Popup_Add_Plane_Normal 30049
#define GL_Popup_Save_Prototype 30050
#define GL_Popup_Change_EFP_To_AllElec 30051

#define FBO_SIZE 512

class BuilderDlg;

typedef class MolDisplayWin MolDisplayWin;
/**
 * Defines a widget for displaying an OpenGL canvas.	At this time it is a
 * child of the wxGLCanvas widget class.
 */
class MpGLCanvas : public wxGLCanvas {
	private:
#if wxCHECK_VERSION(2,9,0)
		wxGLContext *context;
#endif
		MolDisplayWin * MolWin;
		WinPrefs * Prefs;
		bool initialized;

		MoleculeData *mMainData;
		//coordPopup* lPopup;
		wxDragImage *mDragWin;
		
		int mSelectState;
		int oldSelect;
		GLdouble atomDepth;
		GLint winDiffX, winDiffY;
		bool was_zooming;
		bool was_just_rotating;
		bool leftDown;
		bool did_edit;

		void initGL(void);
		int bitmap_fontd;

		void interactPopupMenu(int x, int y, bool isAtom);
		void bondPopupMenu(int x, int y);
		void annoPopupMenu(int x, int y);
		void measurePopupMenu(int x, int y);
		int select_stack[4];
		int select_stack_top;

		void ShowBlankPopup(int x, int y);
		void PasteAtMouse(wxCommandEvent& event);

		void ConstrainPosition(const int anno_id, double x, double y,
							   double z);
		void ChangeAtom(wxCommandEvent& event);
		void ChangeCoordinationNumber(wxCommandEvent& event);
		void ChangeLPCount(wxCommandEvent& event);
		void HandleLassoing(wxMouseEvent& event, const wxPoint& curr_pt);
		void HandleEditing(wxMouseEvent& event, const wxPoint& curr_pt,
						   const wxPoint& prev_pt);

		int selected;           // Id of object clicked on, -1 if invalid
		int selected_type;      // Type of object clicked on
		int selected_site;      // Id of bond site clicked on, -1 if no site
		wxPoint curr_mouse;     // Current mouse position
		wxPoint prev_mouse;     // Previous mouse position
		int first_site_clicked; // Id of bonding site clicked on by user
		int first_atom_clicked; // Atom connected to clicked on site
		int width;              // Width of canvas in pixels
		int height;             // Height of canvas in pixels
		int ndrag_events;       // Number of drag events in current drag
		bool is_depth_zooming;  // True if user is changing atom's depth
		bool window_just_focused; // Flag indicating window getting focus
		wxStopWatch mouse_activate_timer;
		bool ignore_next_up;
		bool is_lassoing;
		bool do_stereo;

		void AddPlaneNormal(wxCommandEvent& event);
		void ConnectSelectedToSite(int src_atom, int src_site,
								   int dst_atom, int dst_site);
		void OnIdleEvent(wxIdleEvent& event);
		void SavePrototype(wxCommandEvent& event);
		/**
		 Convert the selected effective fragment into all-electron atoms.
		 */
		void ConvertEFPToAllElec(wxCommandEvent& event);

	public:
		/**
		* Constructor that provides some useful default values.
		* @param parent A pointer to the parent widget.
		* @param id The window identifier for the new widget.
		* @param position The initial position of the new widget.
		* @param size The initial size of the new widget.
		* @param attributes wx window attributes
		* @param do_stereo If true setup the context as a quad-buffer stereo window.
		* @param style The style of the new widget.	See wxWindow class docs.
		* @param name The name of the new widget.
		*/
		MpGLCanvas(MolDisplayWin *parent,
				   wxWindowID id = wxID_ANY,
				   const wxPoint &position = wxDefaultPosition,
				   const wxSize	&size = wxDefaultSize,
				   int *attributes = NULL,
				   bool do_stereo = false,
				   long	style = wxNO_BORDER,
				   const wxString &name = wxT("DemoGLCanvas"));

		~MpGLCanvas();

		/**
		 * Sets the internal pointer to the window's preferences.  If the
		 * data structure needs to be deleted externally, this function should
		 * first be called with the parameter value set to NULL.	This will
		 * prevent the MpGLCanvas object from trying to reference a deleted
		 * data structure.
		 * @param newPrefs A pointer to the window preferences that the canvas should use.
		 */
		void SetPrefs(WinPrefs *newPrefs);
		void FitToPlane(wxCommandEvent& event);
		void SelectWholeFragments();

		void Draw(void);
		void SetProjection(float aspect_ratio);

		MolDisplayWin *GetParent(void) { return MolWin; }

#if wxCHECK_VERSION(2,9,0)
		// HUH: I don't know why this needs scope resolved, but it does.
		void SetCurrent() { wxGLCanvas::SetCurrent(*context); }
		// And defining the above seems to hide the following from wxGLCanvas.
		void SetCurrent(wxGLContext& context) { wxGLCanvas::SetCurrent(context); }
#endif
		  
		/**
		* Creates an image of the canvas for purposes such as printing or
		* saving to a file.
		*
		* At present, the width and height parameters are ignored, and the
		* returned image is the resolution of the canvas.	It can be used as
		* follows:
		*
		* void MyFrame::OnSS(wxCommandEvent& WXUNUSED(event))
		* {
		*	 wxImage img = GetCanvas()->getImage();
		*	 wxInitAllImageHandlers();
		*	 img.SaveFile("ss.png");
		* }
		* 
		* @param width The desired width of the image in pixels.
		* @param height The desired height of the image in pixels.
		* @return A wxImage, the validity of which can be tested with
		* wxImage::Ok().
		*/
		wxImage getImage(const int width, const int height);
		  
		/**
		* @param dc The device context to send the output to.
		* @param scaleFactor The ratio to scale the image up from the screen size.
		* @param progress  A progress dialog to use in case this takes a while.
		* @param Center  Center the output in the device context.
		* @param frame  Add a black frame around the image.
		*/
		void GenerateHiResImage(wxDC * dc, const float & scaleFactor, Progress * progress,
										  bool Center, bool frame);

		/**
		* Draws the viewport into the given device context at the resolution
		* of the device context.  If the device context height to width ratio
		* is different than that of the viewport, the image will be clipped in
		* the same way that it would be if the viewport were resized to the
		* dimensions of the device context.
		*
		* @param dc An initialized device context to draw the viewport into.
		*/
		void GenerateHiResImageForExport(wxDC *dc);

		// Event Handlers

		/**
		* Handles resize events received by the widget.
		* @param event The event to handle.
		*/
		void eventSize(wxSizeEvent &event);
		  
		/**
		* Handles paint events received by the widget.	This is where OpenGL
		* code is run from.
		* @param event The event to handle.
		*/
		void eventPaint(wxPaintEvent &event);

		/**
		* Handles erase events received by the widget.	This is a no-op that
		* is in place to prevent flickering in Microsoft Windows.
		* @param event The event to handle.
		*/
		void eventErase(wxEraseEvent &event);

		/**
		* Handles mouse events received by the widget.	This is used to allow
		* rotation of the view and perhaps eventually selection and
		* modification of objects within the view.
		* @param event The event to handle.
		*/
		void eventMouse(wxMouseEvent &event);
#if wxCHECK_VERSION(2, 8, 0)
		void eventMouseCaptureLost(wxMouseCaptureLostEvent& event);
#endif
		  
		/**
		* Handles Char events received by the widget. 
		* @param event The event to handle.
		*/
		void KeyHandler(wxKeyEvent & event);

		/**
		 * Handles KEYDOWN events received by the widget. 
		 * @param event The event to handle.
		 */
		void KeyDownHandler(wxKeyEvent & event);
		
		/**
		 * Handles KEYUP events received by the widget. 
		 * @param event The event to handle.
		 */
		void KeyUpHandler(wxKeyEvent & event);
		
		void On_Apply_All(wxCommandEvent& event);
		void On_Delete_Single_Frame(wxCommandEvent& event);
		void On_Delete_All_Frames(wxCommandEvent& event);
		/// Simple routine to cause a resize event, needed to access the private AddPendingEvent function
		void SizeChanged(void) {wxSizeEvent size_event; AddPendingEvent(size_event);};
		void ChangeBonding(wxCommandEvent& event);
		void AddAnnotation(wxCommandEvent& event);
		void DeleteAnnotation(wxCommandEvent& event);
		void DeleteBond(wxCommandEvent& event);
		//implementation of the correspondent popup-menu selection
			
		void testPicking(int x, int y);
		void SelectObj(int selected_type, int select_id, bool mode);
		void findReal3DCoord(GLdouble x, GLdouble y, GLdouble z, GLdouble& realX, GLdouble& realY, GLdouble& realZ);
		void findWinCoord(GLfloat x, GLfloat y, GLfloat z, GLdouble& winX, GLdouble& winY, GLdouble& winZ);
		/* MpGLCanvas *GetParent(void) { return parent; } */
		WinPrefs *GetPrefs(void) { return Prefs; }
		int NumberSelectedAtoms(void) const {return select_stack_top;};
		void insertAnnotationMenuItems(wxMenu& menu);
		void ConstrainToAnnotation(wxCommandEvent& event);
		void SetAnnotationParameter(wxCommandEvent& event);

		void eventMouseMiddleWentUp(wxMouseEvent& event);
		void eventMouseRightWentUp(wxMouseEvent& event);
		void eventMouseLeftWentUp(wxMouseEvent& event);
		void eventMouseDragging(wxMouseEvent& event);
		void eventMouseMiddleWentDown(wxMouseEvent& event);
		void eventMouseRightWentDown(wxMouseEvent& event);
		void eventMouseLeftWentDown(wxMouseEvent& event);
		void eventMouseEnterWindow(wxMouseEvent& event);
		void eventWindowActivated(wxActivateEvent& event);
		void eventMouseLeftDoubleClick(wxMouseEvent& event);
		void eventMouseWheel(wxMouseEvent& event);

	DECLARE_EVENT_TABLE()

};

#endif

