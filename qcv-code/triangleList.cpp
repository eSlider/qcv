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
 * \file  triangleList.cpp
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include "triangleList.h"
#include "glheader.h"
#include <stdio.h>

using namespace QCV;


CTriangleList::CTriangleList( int /* f_bufferSize_i */ )
{}

/// Destructor.
CTriangleList::~CTriangleList()
{}


// Add triangles from other list.
bool
CTriangleList::add ( const CTriangleList &f_otherList )
{
    m_triangle_v.insert( m_triangle_v.begin(), 
                         f_otherList.m_triangle_v.begin(),
                         f_otherList.m_triangle_v.end() );

    return true;
}

// Add empty triangles.
bool 
CTriangleList::add (  const S2D<float> &f_vertex1, 
                      const S2D<float> &f_vertex2, 
                      const S2D<float> &f_vertex3, 
                      const SRgba      &f_color,
                      float             f_lineWidth_i )
{
    return add ( f_vertex1.x,
                 f_vertex1.y,
                 f_vertex2.x,
                 f_vertex2.y,
                 f_vertex3.x,
                 f_vertex3.y,
                 f_color,
                 f_lineWidth_i );
}


// Add empty triangles.
bool 
CTriangleList::add ( float          f_u1_f, 
                     float          f_v1_f,
                     float          f_u2_f,
                     float          f_v2_f,
                     float          f_u3_f, 
                     float          f_v3_f,
                     const SRgba   &f_color,
                     float          f_lineWidth_i )
{
    STriangle newTriangle;

    newTriangle.outlineColor = f_color;

    /// Make transparent filling.
    newTriangle.fillColor.a  = 255;

    newTriangle.vertices[0].x = f_u1_f;
    newTriangle.vertices[0].y = f_v1_f;

    newTriangle.vertices[1].x = f_u2_f;
    newTriangle.vertices[1].y = f_v2_f;

    newTriangle.vertices[2].x = f_u3_f;
    newTriangle.vertices[2].y = f_v3_f;

    newTriangle.lineWidth_f  = f_lineWidth_i;

    m_triangle_v.push_back(newTriangle);
    
    return true;
}

// Add empty triangles.
bool 
CTriangleList::add (  const S2D<float> &f_vertex1, 
                      const S2D<float> &f_vertex2, 
                      const S2D<float> &f_vertex3, 
                      const SRgba      &f_outLineColor,
                      const SRgba      &f_fillColor,
                      float             f_lineWidth_i )
{
    return add ( f_vertex1.x,
                 f_vertex1.y,
                 f_vertex2.x,
                 f_vertex2.y,
                 f_vertex3.x,
                 f_vertex3.y,
                 f_outLineColor,
                 f_fillColor,
                 f_lineWidth_i );
}

// Add a filled triangle.
bool
CTriangleList::add (  float          f_u1_f, 
                      float          f_v1_f,
                      float          f_u2_f,
                      float          f_v2_f,
                      float          f_u3_f, 
                      float          f_v3_f,
                      const SRgba   &f_outLineColor,
                      const SRgba   &f_fillColor,
                      float          f_lineWidth_i )
{
    STriangle newTriangle;

    newTriangle.outlineColor = f_outLineColor;
    newTriangle.fillColor    = f_fillColor;

    newTriangle.vertices[0].x = f_u1_f;
    newTriangle.vertices[0].y = f_v1_f;

    newTriangle.vertices[1].x = f_u2_f;
    newTriangle.vertices[1].y = f_v2_f;

    newTriangle.vertices[2].x = f_u3_f;
    newTriangle.vertices[2].y = f_v3_f;

    newTriangle.lineWidth_f  = f_lineWidth_i;

    m_triangle_v.push_back(newTriangle);

    return true;
}

// Clear triangle list.
bool 
CTriangleList::clear ()
{
    m_triangle_v.clear();
    return m_triangle_v.size();
}

// Draw all triangles.
bool 
CTriangleList::show () const
{
    std::vector< STriangle >::const_iterator last = m_triangle_v.end();

    for (std::vector< STriangle >::const_iterator i = m_triangle_v.begin(); 
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
            for (int v = 0; v < 3; ++v)
                glVertex2f ( i->vertices[v].x, 
                             i->vertices[v].y );
            glEnd();
        }       

        glColor4ub( i->outlineColor.r, 
                    i->outlineColor.g, 
                    i->outlineColor.b, 
                    i->outlineColor.a );
        
        glBegin ( GL_LINE_LOOP ) ;
        for (int v = 0; v < 3; ++v)
            glVertex2f ( i->vertices[v].x, 
                         i->vertices[v].y );
        glEnd();

    }

    /// Todo: Check GL status and return value.
    return true;
}

bool 
CTriangleList::write ( FILE*                f_file_p,
                       const float          f_offsetU_f /* = 0.0 */,
                       const float          f_offsetV_f /* = 0.0 */,
                       const std::string    f_prefix_str /* = "" */) const
{
    std::vector< STriangle >::const_iterator last = m_triangle_v.end();

    int count_i = 0;
    
    for (std::vector< STriangle >::const_iterator i = m_triangle_v.begin(); 
         i != last; ++i, ++count_i )
    {
        if (i->fillColor.a == 0)
        {
            fprintf(f_file_p,
                    "        <polygon id=\"triangle_%s_%i\"\n"
                    "                 points=\"%f,%f %f,%f %f,%f\"\n"
                    "                 stroke-width=\"%f\"\n"
                    "                 stroke=\"#%02x%02x%02x\"\n"
                    "                 opacity=\"%f\"\n"
                    "                 fill=\"none\" />\n",
                    f_prefix_str.c_str(), count_i,
                    i->vertices[0].x + f_offsetU_f,
                    i->vertices[0].y + f_offsetV_f,
                    i->vertices[1].x + f_offsetU_f,
                    i->vertices[1].y + f_offsetV_f,
                    i->vertices[2].x + f_offsetU_f,
                    i->vertices[2].y + f_offsetV_f,
                    1.f+i->lineWidth_f,
                    i->outlineColor.r, 
                    i->outlineColor.g, 
                    i->outlineColor.b, 
                    i->outlineColor.a/255.f );
        }
        else
        {
            fprintf(f_file_p,
                    "        <polygon id=\"triangle_%s_%i\"\n"
                    "                 points=\"%f,%f %f,%f %f,%f\"\n"
                    "                 stroke-width=\"%f\"\n"
                    "                 stroke=\"#%02x%02x%02x\"\n"
                    "                 opacity=\"%f\"\n"
                    "                 fill=\"#%02x%02x%02x\"\n"
                    "                 fill-opacity=\"%f\" />\n",
                    f_prefix_str.c_str(), count_i,
                    i->vertices[0].x + f_offsetU_f,
                    i->vertices[0].y + f_offsetV_f,
                    i->vertices[1].x + f_offsetU_f,
                    i->vertices[1].y + f_offsetV_f,
                    i->vertices[2].x + f_offsetU_f,
                    i->vertices[2].y + f_offsetV_f,
                    1.f+i->lineWidth_f,
                    i->outlineColor.r, 
                    i->outlineColor.g, 
                    i->outlineColor.b, 
                    i->outlineColor.a/255.f,
                    i->fillColor.r, 
                    i->fillColor.g, 
                    i->fillColor.b, 
                    i->fillColor.a/255.f );
        }
    }

    return true;
    
}

/// Return number of elements.
int
CTriangleList::getSize () const
{
    return m_triangle_v.size();    
}
