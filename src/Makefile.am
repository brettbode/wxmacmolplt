bin_PROGRAMS = wxmacmolplt
wxmacmolplt_SOURCES =                                  \
    aboutwxmacmolplt.cpp       aboutwxmacmolplt.h      \
    appendframesoptions.cpp    appendframesoptions.h   \
    AtomTypeList.cpp           AtomTypeList.h          \
    BasisSet.cpp               BasisSet.h              \
    BFiles.cpp                 BFiles.h                \
    bondsdlg.cpp               bondsdlg.h              \
                               cell_table.h            \
    changeatomorderdialog.cpp  changeatomorderdialog.h \
    choosevecgroup.cpp         choosevecgroup.h        \
    ChooseDialog.cpp           ChooseDialog.h          \
    CML.cpp                    CML.h                   \
    colorArea.cpp              colorArea.h             \
    coordinateoffset.cpp       coordinateoffset.h      \
    coordinateswindow.cpp      coordinateswindow.h     \
    DataClasses.cpp                                    \
    energyplotdialog.cpp       energyplotdialog.h      \
    exportoptionsdialog.cpp    exportoptionsdialog.h   \
    Files.cpp                  Files.h                 \
    FloatSlider.cpp            FloatSlider.h           \
    Frame.cpp                  Frame.h                 \
    frameenergy.cpp            frameenergy.h           \
    frequenciesdialog.cpp      frequenciesdialog.h     \
    fmofragmentationdialog.cpp fmofragmentationdialog.h \
    GaussHermite.cpp           GaussHermite.h          \
    Geometries.cpp             Geometries.h            \
    glf.cpp                    glf.h                   \
                               GlobalExceptions.h      \
                               Globals.h               \
    Gradient.cpp               Gradient.h              \
    inputbuilder.cpp           inputbuilder.h          \
    InputData.cpp              InputData.h             \
    InputWizard.cpp            InputWizard.h           \
    Internals.cpp              Internals.h             \
    llmdialog.cpp              llmdialog.h             \
    main.cpp                   main.h                  \
    Math3D.cpp                 Math3D.h                \
    MEP.cpp                                            \
    MMPPrefs.cpp               MMPPrefs.h              \
    MolDisplayWin.cpp          MolDisplayWin.h         \
    MoleculeData.cpp           MoleculeData.h          \
    mpGLCanvas.cpp             mpGLCanvas.h            \
                               myFiles.h               \
    myPrefs.cpp                                        \
    MyTypes.cpp                MyTypes.h               \
    OpenGLGraphics.cpp         mmp_gl.h                \
    orbitals.cpp                                       \
    qtexport.cpp               qtexport.h              \
    build_palette.h            build_palette.cpp       \
    patterns.h                 Prefs.h                 \
                               PrefsPanes.h            \
    preview_canvas.cpp         preview_canvas.h        \
    printoptions.cpp           printoptions.h          \
    Progress.cpp               Progress.h              \
    RysPolynomial.cpp          RysPolynomial.h         \
    setbondlength.cpp          setbondlength.h         \
    setPreference.cpp          setPreference.h         \
    setscreenplane.cpp         setscreenplane.h        \
    SurfaceBase.cpp                                    \
    surfaceDlg.cpp             surfaceDlg.h            \
    surfaceswindow.cpp         surfaceswindow.h        \
                               SurfaceTypes.h          \
    SurfWin.cpp                                        \
    symmetrypointgroupdlg.cpp  symmetrypointgroupdlg.h \
    swfexport.cpp                                      \
    swfexport-g.cpp            swfexport.h             \
    VirtualSphere.cpp          VirtualSphere.h         \
    windowparameters.cpp       windowparameters.h      \
    wxFiles.cpp                                        \
    wxMolGraph.cpp             wxMolGraph.h            \
    wxUglyChoice.cpp           wxUglyChoice.h          \
    XML.cpp                    XML.hpp                 \
    zmatrixcalculator.cpp      zmatrixcalculator.h
##The following allows the built in glew files to always
##be included in the distribution, but are compiled and
##linked in only if there is not a system provided glew.
if !SYSTEM_GLEW
wxmacmolplt_SOURCES += glew.cpp glew.h glxew.h wglew.h
endif

AM_CXXFLAGS = $(WX_CXXFLAGS)
wxmacmolplt_LDADD = $(WX_LIBS) $(MING_LIBS) $(GLU_LIBS) $(GLEW_LIBS)
wxmacmolplt_LDFLAGS = $(WX_CXXFLAGS)
EXTRA_DIST = wxMacMolPlt.pjd xpms/arrow.xpm \
	xpms/hand.xpm xpms/rect_lasso.xpm xpms/view.xpm
