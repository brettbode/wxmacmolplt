#ifndef MPGLCANVAS_H
#define MPGLCANVAS_H 1

#include "wx/wx.h"

#include "mpMolData.h"

class MpGLCanvas {
    private:
        MpMolData *molData;

        void setMolData(const MpMolData *molData);

        void initGL(void);
        void render(void);

    public:
        MpGLCanvas(void);
}

#endif

