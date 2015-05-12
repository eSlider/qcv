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
 * \file  ellipseList.cpp
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include "ellipseList.h"

#include "glheader.h"
#include <math.h>
#include <stdio.h>


using namespace QCV;


CEllipseList::CEllipseList( int /* f_bufferSize_i */ )
{}

/// Destructor.
CEllipseList::~CEllipseList()
{}

// Add ellipses from other list.
bool 
CEllipseList::add (  const CEllipseList & f_otherList )
{    
    m_ellipse_v.insert( m_ellipse_v.begin(), 
                        f_otherList.m_ellipse_v.begin(),
                        f_otherList.m_ellipse_v.end() );
    return true;
}

// Add empty ellipses.
bool
CEllipseList::add ( float   f_u_f, 
                    float   f_v_f,
                    float   f_radiusU_f,
                    float   f_radiusV_f,
                    float   f_rotAngle_f,
                    SRgba   f_color,
                    float   f_lineWidth_i )
{
    SEllipse newEllipse;

    newEllipse.outlineColor = f_color;

    /// Make transparent filling.
    newEllipse.fillColor.a  = 0;

    newEllipse.u_f         = f_u_f;
    newEllipse.v_f         = f_v_f;
    newEllipse.radiusU_f   = f_radiusU_f;
    newEllipse.radiusV_f   = f_radiusV_f;
    newEllipse.rotation_f  = f_rotAngle_f*180.f/M_PI;
    newEllipse.lineWidth_f = f_lineWidth_i;

    m_ellipse_v.push_back(newEllipse);

    return true;
}

// Add a filled ellipse.
bool
CEllipseList::add (  float   f_u_f, 
                     float   f_v_f,
                     float   f_radiusU_f,
                     float   f_radiusV_f,
                     float   f_rotAngle_f,
                     SRgba   f_color,
                     SRgba   f_fillColor,
                     float   f_lineWidth_i )
{
    SEllipse newEllipse;

    newEllipse.outlineColor = f_color;

    /// Make transparent filling.
    newEllipse.fillColor   = f_fillColor;

    newEllipse.u_f         = f_u_f;
    newEllipse.v_f         = f_v_f;
    newEllipse.radiusU_f   = f_radiusU_f;
    newEllipse.radiusV_f   = f_radiusV_f;
    newEllipse.rotation_f  = f_rotAngle_f*180.f/M_PI;
    newEllipse.lineWidth_f = f_lineWidth_i;

    m_ellipse_v.push_back(newEllipse);

    return true;
}

// Clear ellipse list.
bool CEllipseList::clear ()
{
    m_ellipse_v.clear();
    return m_ellipse_v.size();
}

// Draw all ellipses.
bool CEllipseList::show () const
{
    std::vector< SEllipse >::const_iterator last = m_ellipse_v.end();

    for (std::vector< SEllipse >::const_iterator i = m_ellipse_v.begin(); 
         i != last; ++i )
    {
        const int   segments_i  = 36;
        const float increment_f = M_PI * 2. / segments_i;

        float u_p[segments_i];
        float v_p[segments_i];

        float t = 0;

        for( int s = 0; s < segments_i; ++s )
        {
            u_p[s] = i->radiusU_f * cos(t);
            v_p[s] = i->radiusV_f * sin(t);
            t += increment_f;
        }

        glPushMatrix();
        glTranslatef ( i->u_f, i->v_f, 0.f );
        glRotatef    ( i->rotation_f, 0.f, 0.f, 1.f );
        //glTranslatef ( -i->u_f, -i->v_f, 0.f );

        glLineWidth( i->lineWidth_f );

        /// If not complete transparent.
        if ( i->fillColor.a != 0 )
        {
            glColor4ub( i->fillColor.r, 
                        i->fillColor.g, 
                        i->fillColor.b, 
                        i->fillColor.a );
            
            glBegin(GL_POLYGON);

            for( int s = 0; s < segments_i; ++s )
            {
                glVertex2f( u_p[s], v_p[s] );
            }

            glEnd();
        }       

        glColor4ub( i->outlineColor.r, 
                    i->outlineColor.g, 
                    i->outlineColor.b, 
                    i->outlineColor.a );
        
        glBegin ( GL_LINE_LOOP ) ;

        for( int s = 0; s < segments_i; ++s )
        {
            glVertex2f( u_p[s], v_p[s] );
        }
        
        glEnd();

        glPopMatrix();
    }

    /// Todo: Check GL status and return value.
    return true;
}

bool CEllipseList::write ( FILE*                f_file_p,
                           const float          f_offsetU_f /* = 0.0 */,
                           const float          f_offsetV_f /* = 0.0 */,
                           const std::string    f_prefix_str /* = "" */) const
{
    std::vector< SEllipse >::const_iterator last = m_ellipse_v.end();

    int count_i = 0;
    
    for (std::vector< SEllipse >::const_iterator i = m_ellipse_v.begin(); 
         i != last; ++i, ++count_i )
    {
        if (i->fillColor.a == 0)
        {
            fprintf( f_file_p,
                     "        <ellipse id=\"ellipse_%s_%i\"\n"
                     "                 cx=\"%f\" cy=\"%f\" rx=\"%f\" ry=\"%f\"\n"
                     "                 stroke-width=\"%f\"\n"
                     "                 stroke=\"#%02x%02x%02x\"\n"
                     "                 stroke-opacity=\"%f\"\n"
                     "                 fill=\"none\"\n"
                     "                 transform=\"rotate( %f, %f, %f )\" />\n",
                     f_prefix_str.c_str(), count_i,
                     i->u_f+f_offsetU_f, i->v_f+f_offsetV_f,
                     i->radiusU_f, i->radiusV_f, 
                     1+i->lineWidth_f,
                     i->outlineColor.r, i->outlineColor.g, i->outlineColor.b, i->outlineColor.a/255.,
                     i->rotation_f, i->u_f+f_offsetU_f, i->v_f+f_offsetV_f );
        }
        else
        {
            fprintf( f_file_p,
                     "        <ellipse id=\"ellipse_%s_%i\"\n"
                     "                 cx=\"%f\" cy=\"%f\" rx=\"%f\" ry=\"%f\"\n"
                     "                 stroke-width=\"%f\"\n"
                     "                 stroke=\"#%02x%02x%02x\"\n"
                     "                 stroke-opacity=\"%f\"\n"
                     "                 fill=\"#%02x%02x%02x\"\n"
                     "                 fill-opacity=\"%f\"\n"
                     "                 transform=\"rotate( %f, %f, %f )\" />\n",
                     f_prefix_str.c_str(), count_i,
                     i->u_f+f_offsetU_f, i->v_f+f_offsetV_f,
                     i->radiusU_f, i->radiusV_f, 
                     1+i->lineWidth_f,
                     i->outlineColor.r, i->outlineColor.g, i->outlineColor.b, i->outlineColor.a/255.,
                     i->fillColor.r, i->fillColor.g, i->fillColor.b, i->fillColor.a/255.,
                     i->rotation_f, i->u_f, i->v_f );
        }
    }
    return true;
}

/// Return number of elements.
int
CEllipseList::getSize () const
{
    return m_ellipse_v.size();    
}
