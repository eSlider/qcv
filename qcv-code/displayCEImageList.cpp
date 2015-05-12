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
* \file  displayCEImageList.cpp
* \author Hernan Badino
* \notes 
*******************************************************************************
*****             (C) Hernan Badino 2012 - All Rights Reserved            *****
******************************************************************************/

/* INCLUDES */
#include "displayCEImageList.h"

#include <opencv/highgui.h>

#include "glheader.h"


using namespace QCV;


CDisplayColorEncImageList::CDisplayColorEncImageList() 
{
}

/// Destructor.
CDisplayColorEncImageList::~CDisplayColorEncImageList()
{
    clear();
}

// Add images from other list.
bool 
CDisplayColorEncImageList::add ( const CDisplayColorEncImageList & f_otherList )
{
    m_image_v.insert( m_image_v.end(), 
                      f_otherList.m_image_v.begin(),
                      f_otherList.m_image_v.end() );
    return true;
    
}

// Add line.
bool
CDisplayColorEncImageList::add (  cv::Mat *           f_image_p,
                                  CColorEncoding      f_encoder_f,
                                  float               f_u_f, 
                                  float               f_v_f,
                                  float               f_dispWidth_f,
                                  float               f_dispHeight_f,
                                  float               f_alpha_f,
                                  bool                f_makeCopy_b )
{
    bool res_b = true;
    SDisplayColorEncImage newImage;

    newImage.copied_b = f_makeCopy_b;
    
    if ( !f_makeCopy_b )
        newImage.image_p = f_image_p;
    else
    {
        newImage.image_p = new cv::Mat;
        f_image_p->copyTo(*newImage.image_p);
    }

    //if (res_b)
    {
        newImage.u_f         = f_u_f;
        newImage.v_f         = f_v_f;
        newImage.width_f     = f_dispWidth_f;
        newImage.height_f    = f_dispHeight_f;
        newImage.alpha_f     = f_alpha_f;
        newImage.encoder     = f_encoder_f;

        m_image_v.push_back( newImage );
    }

    return res_b;
}

// Clear all lines.
bool 
CDisplayColorEncImageList::clear ()
{
    DisplayColorEncImageList_t::iterator it = m_image_v.begin();
    
    for (; it != m_image_v.end(); ++it)
    {
        it -> freeImage();
    }
    
    m_image_v.clear();
    return true;
}

// Draw all lines.
bool 
CDisplayColorEncImageList::show () const
{

    DisplayColorEncImageList_t::const_iterator last = m_image_v.end();
    
    glLineWidth( 0 );
    for (DisplayColorEncImageList_t::const_iterator i = m_image_v.begin(); 
         i != last; ++i )
    {
        switch( (*i).image_p->type() )
        {
            case CV_8S:
                return showWithType<char>((*i));

            case CV_8U:
                return showWithType<unsigned char>((*i));

            case CV_16S:
                return showWithType<short int>((*i));

            case CV_16U:
                return showWithType<unsigned short int>((*i));
            
            case CV_32S:
                return showWithType<int>((*i));
            
            //case CV_32U:
            //    return showWithType<unsigned int>((*i));
            
            case CV_32F:
                return showWithType<float>((*i));

            case CV_64F:
                return showWithType<double>((*i));

            default:
                return false;
        }
    }
    return false;
}

// Draw all lines.
template <class Type_>
bool 
CDisplayColorEncImageList::showWithType ( const SDisplayColorEncImage & f_elem ) const
{

    const unsigned int w_ui = f_elem.image_p->size().width;
    const int          h_i  = f_elem.image_p->size().height;
    
    const float dx_f = f_elem.width_f  / w_ui;
    const float dy_f = f_elem.height_f / h_i;

    glLineWidth( 0 );    

    // OPENMP HERE?
    for (int v = 0; v < h_i; ++v)
    {
        Type_ *ptr_p = ((Type_ *)(f_elem.image_p->data)) + v * w_ui;
        SRgba colorAlpha;
        SRgb  color;

        float offsetX_f    = f_elem.u_f;
        float offsetY_f    = f_elem.v_f + v * dy_f;
        float offsetYpdy_f = offsetY_f + dy_f;

        for ( unsigned u = 0; u < w_ui; ++u, ++ptr_p )
        {
            float offsetXpdx_f = offsetX_f + dx_f;
            
            f_elem.encoder.colorFromValue( (float) (*ptr_p), 
                                           color );

            colorAlpha = color;
            colorAlpha.a = f_elem.alpha_f;
            

            glColor4ub( colorAlpha.r, 
                        colorAlpha.g,
                        colorAlpha.b,
                        colorAlpha.a );
            
            glBegin(GL_POLYGON);
            glVertex2f ( offsetX_f,    offsetY_f );
            glVertex2f ( offsetXpdx_f, offsetY_f );
            glVertex2f ( offsetXpdx_f, offsetYpdy_f );
            glVertex2f ( offsetX_f,    offsetYpdy_f );
            glEnd();

            offsetX_f = offsetXpdx_f;
        }
    }

    return true;
}

bool
CDisplayColorEncImageList::write ( FILE*                f_file_p,
                                   const float          f_offsetU_f /* = 0.0 */,
                                   const float          f_offsetV_f /* = 0.0 */,
                                   const std::string    f_parameters_str /* = "" */) const
{
    return CDrawingElementList::write( f_file_p, 
                                       f_offsetU_f, 
                                       f_offsetV_f, 
                                       f_parameters_str );
}

/// Return number of elements.
int
CDisplayColorEncImageList::getSize () const
{
    return m_image_v.size();    
}

/*

  Possible parameters: interpolation type, show scroll bars, maintain aspect ratio

  Functionality: full screen (all and individual) with zoom posibility.
 */
