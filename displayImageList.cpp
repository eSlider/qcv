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

/*@@@**************************************************************************
* \file  displayImageList
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include <QGLContext>

#include "displayImageList.h"

#include <opencv/highgui.h>

#include <stdio.h>

#include "glheader.h"

extern QGLContext * g_QGLContext_p;

using namespace QCV;

static int cv2GLDType ( int f_cvFormat_i)
{    
    switch (f_cvFormat_i)
    {
       case CV_8S: 
       //case CV_8SC1:
       case CV_8SC2:
       case CV_8SC3:
       case CV_8SC4:
           return GL_BYTE;
       case CV_8U:
       //case CV_8UC1:
       case CV_8UC2:
       case CV_8UC3:
       case CV_8UC4:
           return GL_UNSIGNED_BYTE;
       case CV_16S:
       //case CV_16SC1:
       case CV_16SC2:
       case CV_16SC3:
       case CV_16SC4:
           return GL_SHORT;
       case CV_16U:
       //case CV_16UC1:
       case CV_16UC2:
       case CV_16UC3:
       case CV_16UC4:
           return GL_UNSIGNED_SHORT;
       case CV_32S:
       //case CV_32SC1:
       case CV_32SC2:
       case CV_32SC3:
       case CV_32SC4:
           return GL_INT;
       case CV_32F:
       //case CV_32FC1:
       case CV_32FC2:
       case CV_32FC3:
       case CV_32FC4:
           return GL_FLOAT;
       case CV_64F:
       //case CV_64FC1:
       case CV_64FC2:
       case CV_64FC3:
       case CV_64FC4:
          return GL_DOUBLE;
    }


    return GL_UNSIGNED_BYTE;    
}

static int cv2GLFormat ( const cv::Mat &f_mat )
{
    if ( f_mat.size().width == 0 || f_mat.size().width == 0 )
        return GL_LUMINANCE;

    int bytesPerElem_i = 1;
    
    switch (f_mat.type())
    {
       case CV_8S: 
       //case CV_8SC1:
       case CV_8SC2:
       case CV_8SC3:
       case CV_8SC4:
       case CV_8U:
       //case CV_8UC1:
       case CV_8UC2:
       case CV_8UC3:
       case CV_8UC4:
           {
               bytesPerElem_i = 1; break;
           }
       case CV_16S:
       //case CV_16SC1:
       case CV_16SC2:
       case CV_16SC3:
       case CV_16SC4:
       case CV_16U:
       //case CV_16UC1:
       case CV_16UC2:
       case CV_16UC3:
       case CV_16UC4:
           {
               bytesPerElem_i = 2; break;
           }
       case CV_32S:
       //case CV_32SC1:
       case CV_32SC2:
       case CV_32SC3:
       case CV_32SC4:
       case CV_32F:
       //case CV_32FC1:
       case CV_32FC2:
       case CV_32FC3:
       case CV_32FC4:
           {
               bytesPerElem_i = 4; break;
           }
       case CV_64F:
       //case CV_64FC1:
       case CV_64FC2:
       case CV_64FC3:
       case CV_64FC4:
           {
               bytesPerElem_i = 8; break;
           }
    }

    int oneChannelWidth_i = (f_mat.size().width*bytesPerElem_i);
    int elemsPerPixel     = f_mat.step / oneChannelWidth_i;

    if (elemsPerPixel <= 1)
        return GL_LUMINANCE;
    else if (elemsPerPixel <= 2)
        return GL_LUMINANCE_ALPHA;
    else if (elemsPerPixel <= 3)
        return GL_RGB;
    else if (elemsPerPixel <= 4)
        return GL_RGBA;

    return GL_LUMINANCE;
}

static int cv2GLFormat2 ( const cv::Mat &f_mat )
{
    int val = cv2GLFormat ( f_mat );
    
    if (val == GL_RGB)
        return GL_BGR;
    else if (val == GL_RGBA)
        return GL_BGRA;
    
    return val;
}

CDisplayImageList::CDisplayImageList() 
{
}

/// Destructor.
CDisplayImageList::~CDisplayImageList()
{
    clear();
}

// Add images from other list.
bool 
CDisplayImageList::add ( const CDisplayImageList & f_otherList )
{
    if ( g_QGLContext_p )
        g_QGLContext_p->makeCurrent();

    m_image_v.insert( m_image_v.begin(), 
                      f_otherList.m_image_v.begin(),
                      f_otherList.m_image_v.end() );

    for (size_t i = 0; i < f_otherList.m_image_v.size(); ++i)
      m_image_v[i].ownResources_b = false;

    return true;    
}

// Draw all lines.
bool CDisplayImageList::add ( cv::Mat             f_image,
                              const float         f_u_f, 
                              const float         f_v_f,
                              const float         f_dispWidth_f,
                              const float         f_dispHeight_f,
                              const float         f_scale_f,
                              const float         f_bias_f,
                              const float         f_alpha_f,
                              const bool          f_makeCopy_b )
{
    if ( g_QGLContext_p )
        g_QGLContext_p->makeCurrent();

    //bool res_b;
    SDisplayImage newImage;

    if ( f_makeCopy_b )
       f_image.copyTo(newImage.image);
    else
        newImage.image = f_image;

    //if (res_b)
    {
        newImage.u_f         = f_u_f;
        newImage.v_f         = f_v_f;
        newImage.width_f     = f_dispWidth_f;
        newImage.height_f    = f_dispHeight_f;
        newImage.alpha_f     = f_alpha_f;
        newImage.scale_f     = f_scale_f;
        newImage.bias_f      = f_bias_f;

        // Generate texture.
        glGenTextures(1, &newImage.textureId_ui);
        
	newImage.ownResources_b = true;	

        m_image_v.push_back(newImage);        

        //first call transferes the texture to hw
        glBindTexture( GL_TEXTURE_RECTANGLE_NV, newImage.textureId_ui );
        
        glPixelStorei( GL_UNPACK_ALIGNMENT, 1);    

        // Interpolation settings ().
        glTexParameteri( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_WRAP_T, GL_CLAMP);
        
        // Interpolation settings (make as parameter).
        glTexParameteri( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR
        glTexParameteri( GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

        glPixelTransferf ( GL_RED_SCALE,   newImage.scale_f );
        glPixelTransferf ( GL_RED_BIAS,    newImage.bias_f  );        
        glPixelTransferf ( GL_GREEN_SCALE, newImage.scale_f );
        glPixelTransferf ( GL_GREEN_BIAS,  newImage.bias_f  );        
        glPixelTransferf ( GL_BLUE_SCALE,  newImage.scale_f );
        glPixelTransferf ( GL_BLUE_BIAS,   newImage.bias_f  );

        int realWidth_i  = newImage.image.step/newImage.image.elemSize();
        int realHeight_i = newImage.image.step/newImage.image.elemSize();
        int offX_i       = realWidth_i - newImage.image.cols;
        int offY_i       = realHeight_i - newImage.image.rows;

        glTexImage2D( GL_TEXTURE_RECTANGLE_NV,
                      0,                                        // base level (only 0 allowed for GL_TEXTURE_RECTANGLE_NV).
                      cv2GLFormat(newImage.image),           // internal format.
                      realWidth_i,
                      newImage.image.size().height,
                      0,                                        // border.
                      cv2GLFormat2(newImage.image),          // 
                      cv2GLDType(newImage.image.type()),     // 
                      newImage.image.ptr<unsigned char>(0) );  // data. 

        glBindTexture(GL_TEXTURE_2D, newImage.textureId_ui);
        glTexSubImage2D( GL_TEXTURE_RECTANGLE_NV,
                         0,
                         offX_i,
                         offY_i,
                         realWidth_i,
                         newImage.image.rows,
                         cv2GLFormat2(newImage.image),
                         cv2GLDType(newImage.image.type()),
                         newImage.image.ptr<unsigned char>(0) );
   }

    return true; //res_b;
}

// Clear all lines.
bool CDisplayImageList::clear ()
{
    for (unsigned int i = 0; i < m_image_v.size(); ++i)
    {
      if (m_image_v[i].ownResources_b)
      {
         glDeleteTextures( 1, &m_image_v[i].textureId_ui );
      }
    }
    
    m_image_v.clear();
    return true;
}

// Draw all lines.
bool CDisplayImageList::show () const
{
    DisplayImageList_t::const_iterator last = m_image_v.end();

    glEnable( GL_TEXTURE_RECTANGLE_NV);

    for (DisplayImageList_t::const_iterator i = m_image_v.begin(); 
         i != last; ++i )
    {
        glBindTexture( GL_TEXTURE_RECTANGLE_NV,
                       i->textureId_ui );
        
        float endX_f = i->u_f + i->width_f;
        float endY_f = i->v_f + i->height_f;
        
        glBegin(GL_QUADS);
        
        glTexCoord2f(0, 0);
        glVertex2f(i->u_f, i->v_f);
        
        glTexCoord2f(i->image.size().width, 0);
        glVertex2f(endX_f, i->v_f);

        glTexCoord2f(i->image.size().width, i->image.size().height);
        glVertex2f(endX_f, endY_f);

        glTexCoord2f(0, i->image.size().height);
        glVertex2f(i->u_f, endY_f);

        glEnd();        
    }

    glDisable(GL_TEXTURE_RECTANGLE_NV); //  This takes a very long
                                        //  time. It sould be optimized!

    /// Todo: Check GL status and return value.
    return true;
}

bool CDisplayImageList::write ( FILE*                f_file_p,
                                const float          f_offsetU_f /* = 0.0 */,
                                const float          f_offsetV_f /* = 0.0 */,
                                const std::string    f_parameters_str /* = "" */) const
{
    /*
    // Insert image link
    fprintf( file_p,
             "    <image x=\"0\" y=\"0\" width=\"%d\" height=\"%d\" xlink:href=\"%s\" />\n",
             m_screenWidth_i, m_screenHeight_i, relImagePath.ascii() );
    
    */
    return CDrawingElementList::write( f_file_p, 
                                       f_offsetU_f, 
                                       f_offsetV_f, 
                                       f_parameters_str );
}

/// Return number of elements.
int
CDisplayImageList::getSize () const
{
    return m_image_v.size();    
}

/*

  Possible parameters: interpolation type, show scroll bars, maintain aspect ratio

  Functionality: full screen (all and individual) with zoom posibility.
 */
