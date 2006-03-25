/*
 * (c) 2005 Iowa State University
 *     see the LICENSE file in the top level directory
 */

/***************************************
 * MolDisplayWin.cpp
 *
 * Created:       11-02-2005  Steven Schulteis
 * Last Modified: 01-19-2006  Steven Schulteis
 ***************************************/

#include "Globals.h"
#include "MolDisplayWin.h"
#include "Prefs.h"
#include "main.h"
#include "Frame.h"
#include "Math3D.h"
#include "myFiles.h"

extern WinPrefs * gPreferences;

BEGIN_EVENT_TABLE(MolDisplayWin, wxFrame)
    EVT_MENU (wxID_SAVE,			MolDisplayWin::menuFileSave)
    EVT_MENU (wxID_SAVEAS,			MolDisplayWin::menuFileSave_as)
	EVT_MENU (wxID_CLOSE,			MolDisplayWin::menuFileClose)
	EVT_CLOSE(						MolDisplayWin::FileClose)
    EVT_MENU (wxID_PRINT_SETUP,		MolDisplayWin::menuFilePage_setup)
    EVT_MENU (wxID_PREVIEW,			MolDisplayWin::menuFilePrint_preview)
    EVT_MENU (wxID_PRINT,			MolDisplayWin::menuFilePrint)

    EVT_MENU (wxID_UNDO,			MolDisplayWin::menuEditUndo)
    EVT_MENU (wxID_CUT,				MolDisplayWin::menuEditCut)
    EVT_MENU (wxID_COPY,			MolDisplayWin::menuEditCopy)
    EVT_MENU (wxID_PASTE,			MolDisplayWin::menuEditPaste)
    EVT_MENU (wxID_CLEAR,			MolDisplayWin::menuEditClear)
    EVT_MENU (wxID_SELECTALL,		MolDisplayWin::menuEditSelect_all)

END_EVENT_TABLE()

MolDisplayWin::MolDisplayWin(const wxString &title,
                         const wxPoint  &position,
                         const wxSize   &size,
                         long            style,
                         const wxString &name)
            :wxFrame((wxWindow *)NULL, wxID_ANY, title,
                     position, size, style, name) {

    createMenuBar();
    SetMenuBar(menuBar);

    // TODO:  Make room for other controls (Frame picker, etc.)
    MainData = new MoleculeData;
	Prefs = new WinPrefs;
	*Prefs = *gPreferences;
	Dirty = false;
	
    glCanvas = new MpGLCanvas(this);

    Show(true);
}

MolDisplayWin::~MolDisplayWin() {
    // TODO:  Destroy any dialogs that are still in existence.

    if(MainData != NULL) {
        glCanvas->setMolData(NULL);
        delete MainData;
    }
	if (Prefs != NULL) {
		delete Prefs;
	}
}

void MolDisplayWin::createMenuBar(void) {
    menuBar = new wxMenuBar;

    menuFile = new wxMenu;
    menuEdit = new wxMenu;
    menuView = new wxMenu;
    menuMolecule = new wxMenu;
    menuWindow = new wxMenu;
    menuHelp = new wxMenu;

    // TODO:  Create IDs for custom menu items
    menuFile->Append(wxID_NEW, wxT("&New\tCtrl+N"));
    menuFile->Append(wxID_OPEN, wxT("&Open ...\tCtrl+O"));
    menuFile->Append(wxID_SAVE, wxT("&Save\tCtrl+S"));
    menuFile->Append(wxID_SAVEAS, wxT("Save &as ...\tCtrl+Shift+S"));
    menuFile->Append(wxID_CLOSE, wxT("&Close\tCtrl+W"));
    menuFile->AppendSeparator();
    //menuFile->Append(, wxT("Append New Frame"));
    //menuFile->Append(, wxT("Add Frames from File ..."));
    //menuFile->Append(, wxT("Delete Frame"));
    //menuFile->AppendSeparator();
    //menuFile->Append(, wxT("Import"));
    //menuFile->Append(, wxT("Export"));
    //menuFile->AppendSeparator();
    menuFile->Append(wxID_PRINT_SETUP, wxT("Page Set&up ..."));
    menuFile->Append(wxID_PREVIEW, wxT("Print Pre&view\tCtrl+Shift+P"));
    menuFile->Append(wxID_PRINT, wxT("&Print ...\tCtrl+P"));
    // TODO:  Make Mac display Quit menu item in the correct place
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, wxT("&Quit\tCtrl+Q"));
    
    menuEdit->Append(wxID_UNDO, wxT("&Undo\tCtrl+Z"));
    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_CUT, wxT("Cu&t\tCtrl+X"));
    menuEdit->Append(wxID_COPY, wxT("&Copy\tCtrl+C"));
    //menuEdit->Append(,wxT("Copy Coordinates"));
    menuEdit->Append(wxID_PASTE, wxT("&Paste\tCtrl+V"));
    menuEdit->Append(wxID_CLEAR, wxT("&Delete\tDel"));
    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_SELECTALL, wxT("&Select all\tCtrl+A"));
    // TODO:  Potentially make Mac display Preferences menu item elsewhere
    //menuEdit->AppendSeparator();
    //menuEdit->Append(, wxT("Default Pr&eferences ..."));

    // TODO:  Create menu items for remaining menus

    // TODO:  Make Mac handle help menu properly
    // TODO:  Make Mac display About menu item in the correct place
    menuHelp->Append(wxID_ABOUT, wxT("&About ..."));

    menuBar->Append(menuFile, wxT("&File"));
    menuBar->Append(menuEdit, wxT("&Edit"));
    menuBar->Append(menuView, wxT("&View"));
    menuBar->Append(menuMolecule, wxT("&Molecule"));
    menuBar->Append(menuWindow, wxT("&Window"));
    menuBar->Append(menuHelp, wxT("&Help"));
}

/* Event handler functions */

/* File menu */
void MolDisplayWin::menuFileOpen(wxCommandEvent &event) {
	//Its possible we could handle this here if the current data is empty?
}

void MolDisplayWin::menuFileSave(wxCommandEvent &event) {
}

void MolDisplayWin::menuFileSave_as(wxCommandEvent &event) {
}

void MolDisplayWin::menuFileClose(wxCommandEvent &event) {
	Close();
}

void MolDisplayWin::FileClose(wxCloseEvent &event) {
	bool canVeto = event.CanVeto();
	//First we should check to see if a save is needed which could abort the close
	if (Dirty && Prefs->GetPrompt4Save()) {
		//prompt the user to see if they want the file saved
		//Note the message should be slightly different if we can't abort the close
		int style = wxYES_NO | wxICON_QUESTION;
		if (canVeto) style = style | wxCANCEL;
		int r = wxMessageBox(wxT("The file filename here has unsaved changes."),
							 wxT("Do you wish to save the current data and customizations before closing?"),
							 style, this);
		if (r == wxCANCEL) {
			event.Veto(true);
			return;
		} else if (r == wxYES) {
			//process the save
		}
	}
	MpApp & app = wxGetApp();
	//Once we decide to close the window it may be system dependant whether we 
	//leave an empty window up. On the Mac the window is always destroyed.
#ifndef __WXMAC__
	if ((app.WindowCount() <= 1) && canVeto) {	
		//This is the last window! Clear it out, but leave it open
		delete MainData;
		MainData = new MoleculeData;
		delete Prefs;
		Prefs = new winPrefs;
		*Prefs = *gPfreferences;
		Dirty = false;
		SetTitle(wxT("Untitled"));
		SetName(wxT("Untitled"));
		event.Veto(true);
		return;
	}
#endif
	Destroy();
	app.destroyMainFrame(this);
}

void MolDisplayWin::menuFilePage_setup(wxCommandEvent &event) {
}

void MolDisplayWin::menuFilePrint_preview(wxCommandEvent &event) {
}

void MolDisplayWin::menuFilePrint(wxCommandEvent &event) {
}

/* Edit menu */

void MolDisplayWin::menuEditUndo(wxCommandEvent &event) {
}

void MolDisplayWin::menuEditCut(wxCommandEvent &event) {
}

void MolDisplayWin::menuEditCopy(wxCommandEvent &event) {
}

void MolDisplayWin::menuEditPaste(wxCommandEvent &event) {
}
void MolDisplayWin::PasteText(void) {
	//relax this restriction later (while in build mode)
	if (MainData->cFrame->NumAtoms != 0) return;	//Do not allow pasting if there are already atoms in this frame
//	if (gScrap.GetText()) {
		long		iline, test, Type;
		CPoint3D	Position, offset;
		
//		BufferFile * TextBuffer = new BufferFile(gScrap.GetText(), GetHandleSize(gScrap.GetText()));
//		if (!TextBuffer) return;
		
		if (MainData->NumFrames == 1) {	//If this is the only frame, make sure it is init'ed
			InitRotationMatrix(MainData->TotalRotation);
		}
		long NumLines =0;
	//	long NumLines = TextBuffer->GetNumLines(-1);
		// There may be up to NumLines atoms so dimension memory accordingly
		if (!MainData->SetupFrameMemory(NumLines, 0)) {
//			delete TextBuffer;
			return;
		}
		
		/*Now interpert each of the lines*/
		for (iline=0; iline < NumLines; iline++) {
			char LineText[kMaxLineLength];
			
//			TextBuffer->GetLine(LineText);
			//Parse the current line: All values will be set in place if the
			//parse is successful (test will return (-) if not). Normal modes
			//shouldn't be pasted so pass in 0 for Mode.
			test = ParseCartLine(LineText, &Type,&Position, &offset, 0);
			if (test > 0) {	/*something was wrong with this line so skip it*/
				
				//A special atom was entered, store its offset.
				if (Type > 115) {
					if (Type > 255)
						if (((Type - 255) < 1)||((Type - 255) > NumLines)) break;
					if (!MainData->cFrame->AddSpecialAtom(offset, iline)) break;
				}
				
				MainData->cFrame->AddAtom(Type, Position);
				MainData->MaxSize = MAX(MainData->MaxSize, fabs(Position.x));
				MainData->MaxSize = MAX(MainData->MaxSize, fabs(Position.y));
				MainData->MaxSize = MAX(MainData->MaxSize, fabs(Position.z));
			}
		}
		//Done with the text handle so unlock it
//		delete TextBuffer;
		
		if (iline == 0) {	/*No atoms were found so clear the memory I just allocated*/
//			MainData->ResetFrameMemory();
			return;
		}

//If there were special atoms found turn on drawing by default
		if (MainData->cFrame->SpecialAtoms)	MainData->SetSpecialAtomDrawMode(true);

		if (Prefs->GetAutoBond())
		MainData->cFrame->SetBonds(Prefs, false);
		//Now reset the display to take into account the new atoms
		MainData->CenterModelWindow();
		MainData->WindowSize = 2.0*MainData->MaxSize;
		MainData->ResetRotation();
		FrameChanged();
//	}
}

void MolDisplayWin::menuEditClear(wxCommandEvent &event) {
}

void MolDisplayWin::menuEditSelect_all(wxCommandEvent &event) {
}

void MolDisplayWin::BondsChanged(void) {
#warning Need to pass event to bonds list here
//	if (BondsDlog) BondsDlog->ResetList();
	FrameChanged();
};

//scan the new frame changing any mode bits for consistancy
void MolDisplayWin::FrameChanged(void) {
	//Check for and update any surfaces depending on the screen plane
	Surface * temp = MainData->cFrame->SurfaceList;
	while (temp) {
		temp->RotateEvent(MainData);
		temp = temp->GetNextSurface();
	}
	UpdateModelDisplay();
}
void MolDisplayWin::UpdateModelDisplay(void) {
#warning Activate the lines below when the functions are added
	//	DrawFrame();
	//	UpdateGLModel();
	//	draw();
}
void MolDisplayWin::ResetView(void) {
	//Check for and update any surfaces depending on the screen plane
	Surface * lSurface = MainData->cFrame->SurfaceList;
	while (lSurface) {
		lSurface->RotateEvent(MainData);
		lSurface = lSurface->GetNextSurface();
	}
//	UpdateGLView();
//	draw();
}
void MolDisplayWin::ResetModel(bool Center) {
	if (Center) {
		MainData->CenterModelWindow();
		MainData->WindowSize = 2.0*(MainData->MaxSize+1.0);
	}
	MainData->ResetRotation();
	//Check for and update any surfaces depending on the screen plane
	Surface * lSurface = MainData->cFrame->SurfaceList;
	while (lSurface) {
		lSurface->RotateEvent(MainData);
		lSurface = lSurface->GetNextSurface();
	}
//	UpdateGLModel();
	// Reset the frame scroll bar
//	::SetControlMaximum(FrameScroll, MainData->NumFrames);
//	::SetControlValue(FrameScroll, MainData->CurrentFrame);
//	draw();
}
void MolDisplayWin::AbortOpen(const char * msg) {
	
	if (this) Close(true);
	
	if (msg != NULL) MessageAlert(msg);
} /* AbortOpen */

long MolDisplayWin::OpenFile(wxString fileName) {
	//This is modeled on OpenTextFile in the Mac version
	long				test=0;
	
	FILE * myfile = fopen(fileName, "r");
	if (myfile == NULL) {
		AbortOpen(wxT("Unable to open the requested file."));
		return 0;
	}
//	try {
		BufferFile * Buffer = new BufferFile(myfile, false);
//		Window->SetSkipPoints(nSkip);
		
		// Attempt to identify the file type by looking for key words
		TextFileType type = Buffer->GetFileType((char *) fileName.mb_str(wxConvUTF8));
	//	Window->BeginOperation();
		switch (type) {
			case kMolType:
				test = OpenMolPltFile(Buffer);
				break;
			case kGAMESSlogType:
	//			test = OpenGAMESSlog(Buffer, false, flip, offset);
				break;
			case kGAMESSIRCType:
	//			test = OpenGAMESSIRC(Buffer, false,flip,offset);
				break;
			case kGAMESSDRCType:
	//			test = OpenGAMESSDRC(Buffer, false, false,flip,offset);
				break;
			case kGAMESSInputType:
				test = OpenGAMESSInput(Buffer);
				break;
			case kXYZType:
				test = OpenXYZFile(Buffer);
				break;
			case kPDBType:
				test = OpenPDBFile(Buffer);
				break;
			case kMDLMolFile:
				test = OpenMDLMolFile(Buffer);
				break;
			case CMLFile:
	//		{
	//			test = OpenCMLFile(Buffer);
	//			if (test == 0) Window->AbortOpen(0);
	//		}
				break;
//			default:	//Should only get here for unknown file types.
//				Window->AbortOpen(34);
		}
//	}
//	catch (std::bad_alloc) {//Out of memory error
//		Window->AbortOpen(3);
//	}
//	catch (MemoryError) {
//		Window->AbortOpen(3);
//	}
//	catch (UserCancel) {
//		Window->AbortOpen(6);
//	}
//	catch (DataError Error) {//Error parsing the file data
//		if (!Error.ErrorSet())  Window->AbortOpen(21);
//		else {
//			Error.WriteError();
//			delete Window; Window = NULL;
//		}
//	}
	//Some kind of File system related error
//	catch (FileError Error) { Error.WriteError(); Window->AbortOpen(-1);}
///	catch (...) { Window->AbortOpen(40);}
	if (Buffer) delete Buffer;		//Done reading so free up the buffer
	if (test) {//Note test is left 0 if any exception occurs(which causes Window to be deleted)
//		if (gPreferences->ChangeFileType()) {
//			// Looks like this is a good file so set the creator type for the neat icon
//			FInfo	myFInfo;
//			HGetFInfo(myFile->vRefNum, myFile->parID, myFile->name, &myFInfo);
//			if (myFInfo.fdCreator != (OSType) 'BMBm') {
//				myFInfo.fdCreator = (OSType) 'BMBm';
//				HSetFInfo(myFile->vRefNum, myFile->parID, myFile->name, &myFInfo);
//			}
//		}
		//Text files are not used after opening so close it immediately
		fclose(myfile);
//		Window->CloseFile();	//Hmmm should this happen for CML files?
//		if (!Window->IsSavedFile()) Window->SetFileType(5);
//		Window->FinishOperation();	//Close the progress dialog, if opened
//		if (!Window->IsSavedFile() && gPreferences->Default3DOn()) Window->Activate3D();
		//Tell the window its data has changed so that it will be redrawn correctly
//		if (!Window->IsSavedFile()) Window->ResetModel(true);
	}
	return test;
}

