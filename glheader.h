/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 2.1. See the GNU LGPL version 2.1 for details.
 * QCV is distributed "AS IS" without ANY WARRANTY, without even the
 * implied warranty of merchantability or fitness for a particular
 * purpose. 
 *
 * In no event shall the authors or contributors be liable
 * for any direct, indirect, incidental, special, exemplary, or
 * consequential damages arising in any way out of the use of this
 * software.
 *
 * By downloading, copying, installing or using the software you agree
 * to this license. Do not download, install, copy or use the
 * software, if you do not agree to this license.
 */

/**
 *******************************************************************************
 * @file glheader.h
 *
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Some definitions for using the opengl library.
 *
 ******************************************************************************/

#ifndef GLHEADER
#define GLHEADER 1


#ifdef WIN32
  #include <windows.h>
  #include <GL/gl.h>

  #include <GL/wglext.h>
#elif __APPLE__

  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <OpenGL/glext.h> 
  #include <OpenGL/glext.h>
  #include <GLUT/glut.h> 

#else // not WIN32 and not __APPLE__

  #define __USE_GLX_SGIX_EXTENSION__
  #define GLX_GLXEXT_PROTOTYPES 1
  #define GL_GLEXT_PROTOTYPES   1
#include <GL/gl.h>
#include <GL/glext.h>
  #include <GL/glx.h>
  #include <GL/glut.h>

#endif // WIN32 __APPLE__


// TODO: fix instances of GL_TEXTURE_RECTANGLE_ARB: this is a workaround for when it is not available
#ifndef  GL_TEXTURE_RECTANGLE_NV
#ifdef GL_TEXTURE_RECTANGLE_ARB

  #define GL_TEXTURE_RECTANGLE_NV GL_TEXTURE_RECTANGLE_ARB

#elif GL_TEXTURE_RECTANGLE_EXT

  #define GL_TEXTURE_RECTANGLE_NV GL_TEXTURE_RECTANGLE_EXT

#endif // GL_TEXTURE_RECTANGLE_ARB or GL_TEXTURE_RECTANGLE_EXT
#endif // GL_TEXTURE_RECTANGLE_NV 

#endif // GLHEADER
