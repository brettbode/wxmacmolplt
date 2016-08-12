AC_INIT(wxmacmolplt, 7.7, MMP.Guy@gmail.com)
AC_CONFIG_SRCDIR(src/main.cpp)
AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE([1.9 foreign])
dnl AC_CONFIG_HEADER
dnl AC_REVISION
AM_MAINTAINER_MODE

AC_CANONICAL_HOST
GLU_LIBS=
HOST=
case "${host}" in
   *msvc*)
      HOST=MSW 
      GLU_LIBS="-lopengl32 -lglu32"
      ;;
   *darwin*)
      HOST=OSX 
      GLU_LIBS="-framework OpenGL"
      ;;
   *)
      HOST=LINUX
      PKG_CHECK_MODULES([GLU], [glu], [], [GLU_LIBS="-lGL -lGLU"])
      ;;
esac
AM_CONDITIONAL(HOST_IS_MSW, [test "x$HOST" == xMSW])
AC_SUBST([GLU_LIBS])

#
# There isn't a reliable way to know we should use the Apple OpenGL framework
# without a configure option.  A Mac OS X user may have installed an
# alternative GL implementation (e.g., Mesa), which may or may not depend on X.
#
# AC_ARG_WITH([apple-opengl-framework], 
            # [AC_HELP_STRING([--with-apple-opengl-framework], 
                            # [use Apple OpenGL framework (Mac OS X only; default=no)])], 
            # [], 
            # [with_apple_opengl_framework=no]) 
# LIBGL= 
# AS_IF([test "x$with_apple_opengl_framework" != xno], 
      # [AC_SUBST([LIBGL], ["-framework OpenGL"])]) 

AC_ARG_WITH([quicktime],
            [AC_HELP_STRING([--with-quicktime],
                            [use Quicktime (Mac OS X only; default=no)])],
            [],
            [with_quicktime=no])
AS_IF([test "x$with_quicktime" != xno],
      [CPPFLAGS="$CPPFLAGS -D__MAC_USE_QUICKTIME__ -FQuickTime"])

AC_ARG_WITH(ming,
[[  --with-ming             Include support for exporting flash animations
                          using the ming library; (default=yes)]],
[],
[with_ming=yes])

MING_LIBS=

AS_IF([test "x$with_ming" != xno],
[AC_CHECK_LIB([ming], [Ming_init],
    [PKG_CHECK_MODULES([MING], [libming], [], [MING_LIBS="-lming"])
     AC_DEFINE([HAVE_LIBMING])],
    [if test "x$with_ming" != xyes ; then
        AC_MSG_FAILURE([--with-ming was given, but test for ming failed])
    fi], -lpng)])

AC_SUBST([MING_LIBS])

AC_ARG_WITH(glew,
[[  --with-glew             Use the system provided glew library instead of 
                          the built-in version; (default=yes if present)]],
[],
[with_glew=check])

GLEW_LIBS=

AS_IF([test "x$with_glew" != xno],
      [AC_CHECK_LIB([GLEW], [glewInit],
    [PKG_CHECK_MODULES([GLEW], [glew], [], [GLEW_LIBS="-lGLEW"])
     AC_DEFINE([SYSTEM_GLEW])],
    [if test "x$with_glew" = xyes; then
        AC_MSG_FAILURE([--with-glew was given, but test for glew failed])
    fi])])

AM_CONDITIONAL([SYSTEM_GLEW], [test "x$ac_cv_lib_GLEW_glewInit" = xyes])
AC_SUBST([GLEW_LIBS])

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
WX_LIBS="`$WXCONFIG --libs base,core,gl,adv,html`"
AC_SUBST([WX_CXXFLAGS])
AC_SUBST([WX_LIBS])

# The purpose here is to test for a working wx install
# before the GL test. This should give the user a better error
# when the install is broken rather than saying wx lacks GL support
AC_MSG_CHECKING([for wxWidgets libraries])
AC_LANG_SAVE()
AC_LANG_CPLUSPLUS()
TEMP_CXXFLAGS=$CXXFLAGS
CXXFLAGS="$CXXFLAGS `$WXCONFIG --cxxflags`"
TEMP_LIBS=$LIBS
LIBS="$LIBS `$WXCONFIG --libs`"
AC_TRY_RUN([
      #include <cstdlib>
      #include "wx/wx.h"
      using namespace std;
      int main() {
          wxWindow *win;
          exit(0);
          return 0;
      }
],
AC_MSG_RESULT([yes]),
AC_MSG_RESULT([not found])
AC_MSG_ERROR([have you set LD_LIBRARY_PATH correctly?]))
CXXFLAGS=$TEMP_CXXFLAGS
LIBS=$TEMP_LIBS
AC_LANG_RESTORE()

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

case "${host}" in
    *msvc*)
        datarootdir="${bindir}"
        ;;
esac

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
