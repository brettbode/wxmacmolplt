#ifndef MMP_GL_H
#define MMP_GL_H

#ifdef __WXMAC__
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#ifndef GL_RESCALE_NORMAL
#warning "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
#define GL_RESCALE_NORMAL 0x803A
#endif

#ifndef GL_SEPARATE_SPECULAR_COLOR
#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
#endif

#endif
