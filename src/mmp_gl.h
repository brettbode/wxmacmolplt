#ifndef MMP_GL_H
#define MMP_GL_H

#ifdef SYSTEM_GLEW
#include <GL/glew.h>
#else
#include "glew.h"
#endif
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <string>

#if 0
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif
#ifdef __WXMAC__
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif
#endif

/* #ifndef GL_RESCALE_NORMAL */
/* #define GL_RESCALE_NORMAL 0x803A */
/* #endif */

/* #ifdef GL_VERSION_2_0 */
GLuint GetShaderProgramFromFiles(const std::string& vert_filename,
                                 const std::string& frag_filename);

GLuint GetShaderProgram(const std::string& vert_src,
                        const std::string& frag_src);
/* #endif */

#endif
