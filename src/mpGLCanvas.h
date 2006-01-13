/***************************************
 * mpGLCanvas.h
 *
 * Description goes here.
 *
 * Created:       01-09-2006  Steven Schulteis
 * Last Modified: 01-09-2006  Steven Schulteis
 ***************************************/

#ifndef MPGLCANVAS_H
#define MPGLCANVAS_H 1

#include "wx/wx.h"

#include "mpMolData.h"

struct GLWorldData {
};

class MpGLCanvas {
    private:
        MpMolData   *molData;
        GLWorldData  worldData;

        void initGL(void);
        void render(void);

    public:
        MpGLCanvas(void);

        // Call setMolData(NULL) before deleting the MpMolData object
        void setMolData(const MpMolData *molData);
};

#endif

