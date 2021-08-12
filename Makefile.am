SUBDIRS = src
dist_doc_DATA = LICENSE
dist_man_MANS = resources/wxmacmolplt.1 
shared = resources/MacMolPlt.Prefs.xml \
    resources/fonts/arial1.bmf resources/fonts/arial1.glf \
    resources/fonts/BsVeraSans.fdb resources/organics.cml \
    resources/solvents.cml resources/amino_acids.cml \
    resources/shaders/perpixel_dirlight_v.glsl \
    resources/shaders/perpixel_dirlight_f.glsl \
    resources/shaders/shadows_v.glsl \
    resources/shaders/shadows_f.glsl \
    resources/splash.jpg \
	resources/view.png
if HOST_IS_MSW
bindata_DATA = $(shared)
bindatadir = $(bindir)
else
dist_pkgdata_DATA = $(shared)
endif
nobase_dist_pkgdata_DATA = MacMolPlt_Manual.html \
    Manual_pages/WalkThrough.html Manual_pages/2D_General_Dialog.gif \
    Manual_pages/2D_MEP.gif Manual_pages/2D_MEP_Dialog.gif \
    Manual_pages/2D_Orbital.gif Manual_pages/2D_Orbital_Dialog.gif \
    Manual_pages/2D_TED.gif Manual_pages/2D_TED_Dialog.gif \
    Manual_pages/3D_General_Dialog.gif Manual_pages/3D_Orbital.jpg \
    Manual_pages/3D_Orbital_Dialog.gif Manual_pages/3D_TED.jpg \
    Manual_pages/3D_TED_Dialog.gif Manual_pages/Bonds_Win.gif \
    Manual_pages/Coords_Win.gif Manual_pages/Create_Surface.gif \
    Manual_pages/General_Description.html Manual_pages/IB_Win.gif \
    Manual_pages/LLM.html Manual_pages/MEP_3D.jpg \
    Manual_pages/MEP_3D_Dialog.gif Manual_pages/MacMolPlt_Files.html \
    Manual_pages/MacMolPlt_Surfaces.html Manual_pages/MacMolPlt_Windows.html \
    Manual_pages/Version_History.html Manual_pages/water.jpg \
    Manual_pages/sample-mol.jpg Manual_pages/builder.html \
    Manual_pages/bond_sites.jpg samples/water.log samples/sample.irc
EXTRA_DIST = wxMacMolPlt.kdevelop LICENSE  \
    wxMacMolPlt.vcproj wxMacMolPlt.rc wxMacMolPlt.xcodeproj/project.pbxproj \
    debian/changelog debian/compat debian/control debian/copyright \
    debian/dirs debian/docs debian/menu debian/rules \
    resources/wxmacmolplt.png resources/wxmacmolplt.desktop \
	resources/wxmacmolplt.appdata.xml README \
    resources/fonts/COPYRIGHT.BITSTREAM
