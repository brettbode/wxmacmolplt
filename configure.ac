AC_INIT(src/main.cpp)
AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE(wxMacMolPlt, 0.0.1)
dnl AC_CONFIG_HEADER
dnl AC_REVISION


WXCONFIG=wx-config
AC_ARG_WITH(wx-config,
[[  --with-wx-config=FILE   Use the given path to wx-config when determining
                            wxWidgets configuration; defaults to "wx-config"]],
[
    if test "$withval" != "yes" -a "$withval" != ""; then
        WXCONFIG=$withval
    fi
])

dnl C-compiler checks
AC_PROG_CC

AC_PROG_CXX
dnl AX_CHECK_GL


dnl if test x"$no_gl" = xyes; then
dnl     AC_MSG_ERROR([OpenGL is required.])
dnl fi


dnl I need a way to test for wxWidgets GL support, or else I need to ditch it
dnl in favor of some kind of decent off-screen rendering.

AC_MSG_CHECKING([wxWidgets version])
if wxversion=`$WXCONFIG --version`; then
    AC_MSG_RESULT([$wxversion])
else
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([wxWidgets is required. Try --with-wx-config.])
fi
WX_CXXFLAGS="`$WXCONFIG --cxxflags`"
WX_LIBS="`$WXCONFIG --libs base,core,gl,adv`"
AC_SUBST([WX_CXXFLAGS])
AC_SUBST([WX_LIBS])


AC_SUBST(ac_aux_dir)
AC_OUTPUT(Makefile src/Makefile)
