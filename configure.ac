AC_INIT(wxmacmolplt, 6.3.0, brett@scl.ameslab.gov)
AC_CONFIG_SRCDIR(src/main.cpp)
AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE([1.9 foreign])
dnl AC_CONFIG_HEADER
dnl AC_REVISION
AM_MAINTAINER_MODE


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


AC_MSG_CHECKING([wxWidgets version])
if wxversion=`$WXCONFIG --version`; then
    AC_MSG_RESULT([$wxversion])
else
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([wxWidgets is required. Try --with-wx-config.])
fi
WX_CXXFLAGS="`$WXCONFIG --cxxflags` -DINSTALL_PREFIX=\\\"$prefix\\\""
WX_LIBS="`$WXCONFIG --libs base,core,gl,adv,core,base`"
AC_SUBST([WX_CXXFLAGS])
AC_SUBST([WX_LIBS])

AC_MSG_CHECKING([for OpenGL support in wxWidgets])
AC_LANG_SAVE()
AC_LANG_CPLUSPLUS()
TEMP_CXXFLAGS=$CXXFLAGS
CXXFLAGS="$CXXFLAGS `$WXCONFIG --cxxflags`"
TEMP_LIBS=$LIBS
LIBS="$LIBS `$WXCONFIG --libs`"
AC_TRY_RUN([
            #include <cstdlib>
            #include "wx/wx.h"
            #include "wx/glcanvas.h"
            using namespace std;
            int main() {
            #if wxUSE_GLCANVAS
                exit(0);
                return 0;
            #else
                exit(-1);
                return -1;
            #endif
            }
           ],
           AC_MSG_RESULT([yes]),
           AC_MSG_RESULT([not found])
           AC_MSG_ERROR([wxMacMolPlt requires wxWidgets to be compiled with OpenGL support.]))
CXXFLAGS=$TEMP_CXXFLAGS
LIBS=$TEMP_LIBS
AC_LANG_RESTORE()

#Begin Mike Ekstrand's cleverness
if test -z "${datarootdir}"; then
    datarootdir="${datadir}"
    AC_SUBST([datarootdir])
fi

if test -z "${docdir}"; then
    # be compatible with old make's
    docdir='${datarootdir}/doc/${PACKAGE}'
    dvidir='${docdir}'
    pdfdir='${docdir}'
    psdir='${docdir}'
    htmldir='${docdir}'
    AC_SUBST([docdir])
    AC_SUBST([dvidir])
    AC_SUBST([pdfdir])
    AC_SUBST([psdir])
    AC_SUBST([htmldir])
fi
#End cleverness

AC_SUBST(ac_aux_dir)
AC_OUTPUT(Makefile src/Makefile)
