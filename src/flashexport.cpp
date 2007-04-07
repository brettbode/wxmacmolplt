#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/mstream.h"

#include "Globals.h"
#include "MolDisplayWin.h"

#ifdef HAVE_LIBMING
#include <mingpp.h>
#endif

#ifdef HAVE_LIBMING
void MolDisplayWin::CreateFrameMovie(wxString &filePath) {
    long SavedFrameNum = MainData->GetCurrentFrame();
    long i;
    SWFMovie *movie = new SWFMovie();
    SWFInput *in;
    SWFBitmap *bm;
    SWFDisplayItem *di = NULL;
    long width;
    long height;
    long AnimateTime = 10*Prefs->GetAnimateTime();

    if(AnimateTime < 1) AnimateTime = 1;

    getCanvasSize(&width, &height);

    movie->setBackground(0xFF, 0xFF, 0xFF);
    movie->setRate(1000.0/(double)AnimateTime);
    movie->setDimension(width, height);

    for(i = 1; i <= MainData->NumFrames; ++i) {
        MainData->SetCurrentFrame(i);

        Surface *temp = MainData->cFrame->SurfaceList;
        while(temp) {
            temp->RotateEvent(MainData);
            temp = temp->GetNextSurface();
        }
        MainData->ResetRotation();
        UpdateGLModel();
        DrawGL();

        wxImage frame = glCanvas->getImage(0, 0);
        wxMemoryOutputStream *memOutStream = new wxMemoryOutputStream();
        frame.SaveFile(*(wxOutputStream *)memOutStream, (int)wxBITMAP_TYPE_JPEG);
        long datLen = memOutStream->GetSize();
        char unsigned *jpegData = new unsigned char [datLen];
        memOutStream->CopyTo(jpegData, datLen);
        delete memOutStream;
        in = new SWFInput(jpegData, datLen);
        bm = new SWFBitmap(in);
        
        di = movie->add((SWFBlock *)bm);
        movie->nextFrame();
        movie->remove(di);
        //delete jpegData;
    }

    movie->save(filePath.mb_str(wxConvUTF8), 0);
    delete movie;
    MainData->SetCurrentFrame(SavedFrameNum);
}
#endif

