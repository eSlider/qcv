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
 * \file  rectList.cpp
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include "rectList.h"
#include "glheader.h"
#include <stdio.h>


using namespace QCV;


CRectangleList::CRectangleList( int /* f_bufferSize_i */ )
{}

/// Destructor.
CRectangleList::~CRectangleList()
{}


// Add rectangles from other list.
bool
CRectangleList::add ( const CRectangleList &f_otherList )
{
    m_rect_v.insert( m_rect_v.begin(), 
                     f_otherList.m_rect_v.begin(),
                     f_otherList.m_rect_v.end() );
    return true;
}

// Add empty rectangles.
bool
CRectangleList::add ( float   f_u1_f, 
                           float   f_v1_f,
                           float   f_u2_f,
                           float   f_v2_f,
                           SRgba   f_color,
                           float   f_lineWidth_i )
{
    SRectangle newRect;

    newRect.outlineColor = f_color;

    /// Make transparent filling.
    newRect.fillColor.a  = 0;

    newRect.u1_f         = f_u1_f;
    newRect.u2_f         = f_u2_f;
    newRect.v1_f         = f_v1_f;
    newRect.v2_f         = f_v2_f;
    newRect.lineWidth_f  = f_lineWidth_i;

    m_rect_v.push_back(newRect);

    return true;
}

// Add a filled rectangle.
bool
CRectangleList::add (  float   f_u1_f, 
                            float   f_v1_f,
                            float   f_u2_f,
                            float   f_v2_f,
                            SRgba   f_outLineColor,
                            SRgba   f_fillColor,
                            float   f_lineWidth_i )
{
    SRectangle newRect;

    newRect.outlineColor = f_outLineColor;
    newRect.fillColor    = f_fillColor;
    newRect.u1_f         = f_u1_f;
    newRect.u2_f         = f_u2_f;
    newRect.v1_f         = f_v1_f;
    newRect.v2_f         = f_v2_f;
    newRect.lineWidth_f  = f_lineWidth_i;

    m_rect_v.push_back(newRect);

    return true;
}

// Clear rectangle list.
bool
CRectangleList::clear ()
{
    m_rect_v.clear();
    return m_rect_v.size();
}

// Draw all rectangles.
bool
CRectangleList::show () const
{
    std::vector< SRectangle >::const_iterator last = m_rect_v.end();

    for (std::vector< SRectangle >::const_iterator i = m_rect_v.begin(); 
         i != last; ++i )
    {
        glLineWidth( i->lineWidth_f );
        
        /// If not complete transparent.
        if ( i->fillColor.a != 0 )
        {
            glColor4ub( i->fillColor.r, 
                        i->fillColor.g, 
                        i->fillColor.b, 
                        i->fillColor.a );
            
            glBegin(GL_POLYGON);
            glVertex2f ( i->u1_f, i->v1_f );
            glVertex2f ( i->u2_f, i->v1_f );
            glVertex2f ( i->u2_f, i->v2_f );
            glVertex2f ( i->u1_f, i->v2_f );
            glEnd();
        }       

        glColor4ub( i->outlineColor.r, 
                    i->outlineColor.g, 
                    i->outlineColor.b, 
                    i->outlineColor.a );
        
        glBegin ( GL_LINE_LOOP ) ;
        glVertex2f ( i->u1_f, i->v1_f );
        glVertex2f ( i->u2_f, i->v1_f );
        glVertex2f ( i->u2_f, i->v2_f );
        glVertex2f ( i->u1_f, i->v2_f );
        glEnd();

    }

    /// Todo: Check GL status and return value.
    return true;
}

bool
CRectangleList::write ( FILE*                f_file_p,
                        const float          f_offsetU_f /* = 0.0 */,
                        const float          f_offsetV_f /* = 0.0 */,
                        const std::string    f_prefix_str /* = "" */) const
{
    std::vector< SRectangle >::const_iterator last = m_rect_v.end();

    int count_i = 0;
    
    for (std::vector< SRectangle >::const_iterator i = m_rect_v.begin(); 
         i != last; ++i )
    {
        if (i->fillColor.a == 0)
        {
            fprintf( f_file_p,
                     "        <rect id=\"rect_%s_%i\"\n"
                     "                 x=\"%f\" y=\"%f\"\n"
                     "                 width=\"%f\" height=\"%f\"\n"
                     "                 stroke-width=\"%f\"\n"
                     "                 stroke=\"#%02x%02x%02x\"\n"
                     "                 opacity=\"%f\"\n"
                     "                 fill=\"none\" />\n",
                     f_prefix_str.c_str(), count_i,
                     i->u1_f + f_offsetU_f,  i->v1_f + f_offsetV_f,
                     i->u2_f-i->u1_f, i->v2_f-i->v1_f,
                     1.f+i->lineWidth_f,
                     i->outlineColor.r, i->outlineColor.g, i->outlineColor.b, i->outlineColor.a/255.f);
        }
        else
        {
            fprintf( f_file_p,
                     "        <rect id=\"rect_%s_%i\"\n"
                     "                 x=\"%f\" y=\"%f\"\n"
                     "                 width=\"%f\" height=\"%f\"\n"
                     "                 stroke-width=\"%f\"\n"
                     "                 stroke=\"#%02x%02x%02x\"\n"
                     "                 opacity=\"%f\"\n"
                     "                 fill=\"#%02x%02x%02x\""
                     "                 fill-opacity=\"%f\" />\n",
                     f_prefix_str.c_str(), count_i,
                     i->u1_f + f_offsetU_f,  i->v1_f + f_offsetV_f,
                     i->u2_f-i->u1_f, i->v2_f-i->v1_f,
                     1.f+i->lineWidth_f,
                     i->outlineColor.r, i->outlineColor.g, i->outlineColor.b, i->outlineColor.a/255.,
                     i->fillColor.r, i->fillColor.g, i->fillColor.b, i->fillColor.a/255.f );
            
        }
    }
    
    return true;
}

/// Return number of elements.
int
CRectangleList::getSize () const
{
    return m_rect_v.size();    
}

