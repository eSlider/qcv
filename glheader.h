/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU General Public License, version 2.
 * See the GNU General Public License, version 2 for details.
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
#ifndef QTVISU  
  #include "glh/glh_extensions.h"
#endif  
#else
  #define __USE_GLX_SGIX_EXTENSION__
  #define GLX_GLXEXT_PROTOTYPES 1
  #define GL_GLEXT_PROTOTYPES   1
#include <GL/gl.h>
#include <GL/glext.h>

#endif


#ifdef WIN32
  #include <GL/wglext.h>
#else
  #include <GL/glx.h>
#endif

#endif
