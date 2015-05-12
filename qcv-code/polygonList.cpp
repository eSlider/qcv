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
 * \file  polygonList.cpp
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include "polygonList.h"
#include "glheader.h"
#include <stdio.h>


using namespace QCV;


CPolygonList::CPolygonList( int /* f_bufferSize_i */ )
{}

/// Destructor.
CPolygonList::~CPolygonList()
{}


// Add polygons from other list.
bool
CPolygonList::add ( const CPolygonList &f_otherList )
{
    m_polygon_v.insert( m_polygon_v.begin(), 
                        f_otherList.m_polygon_v.begin(),
                        f_otherList.m_polygon_v.end() );
    return true;
}

// Add empty polygons.
bool
CPolygonList::add ( const std::vector< S2D<float> > & f_vector,
                    SRgba                             f_color,
                    float                             f_lineWidth_f )
{    
    m_polygon_v.push_back(SPolygon ());

    SPolygon &newPolygon = m_polygon_v[m_polygon_v.size()-1];

    newPolygon.outlineColor = f_color;
    newPolygon.lineWidth_f  = f_lineWidth_f;
    /// Make transparent filling.
    newPolygon.fillColor.a  = 0;

    newPolygon.vertex_v.insert( newPolygon.vertex_v.end(), 
                                f_vector.begin(), 
                                f_vector.end());

    return true;
}

// Add a filled polygon.
bool
CPolygonList::add (  const std::vector< S2D<float> > & f_vector,
                     SRgba                             f_outLineColor,
                     SRgba                             f_fillColor,
                     float                             f_lineWidth_f )
{
    m_polygon_v.push_back(SPolygon ());

    SPolygon &newPolygon = m_polygon_v[m_polygon_v.size()-1];

    newPolygon.outlineColor = f_outLineColor;
    newPolygon.fillColor    = f_fillColor;
    newPolygon.lineWidth_f  = f_lineWidth_f;

    newPolygon.vertex_v.insert( newPolygon.vertex_v.end(), 
                                f_vector.begin(), 
                                f_vector.end());
    
    return true;
}

// Clear polygon list.
bool
CPolygonList::clear ()
{
    m_polygon_v.clear();
    return m_polygon_v.size();
}

// Draw all polygons.
bool
CPolygonList::show () const
{
    std::vector< SPolygon >::const_iterator last = m_polygon_v.end();

    for (std::vector< SPolygon >::const_iterator i = m_polygon_v.begin(); 
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

            for (unsigned int v = 0; v < i->vertex_v.size(); ++v )
                glVertex2f ( i->vertex_v[v].x, 
                             i->vertex_v[v].y );
            glEnd();
        }       

        glColor4ub( i->outlineColor.r, 
                    i->outlineColor.g, 
                    i->outlineColor.b, 
                    i->outlineColor.a );
        
        glBegin ( GL_LINE_LOOP ) ;

        for (unsigned int v = 0; v < i->vertex_v.size(); ++v )
            glVertex2f ( i->vertex_v[v].x, 
                         i->vertex_v[v].y );
        glEnd();

    }

    /// Todo: Check GL status and return value.
    return true;
}

bool
CPolygonList::write ( FILE*                f_file_p,
                      const float          f_offsetU_f /* = 0.0 */,
                      const float          f_offsetV_f /* = 0.0 */,
                      const std::string    f_prefix_str /* = "" */) const
{
    std::vector< SPolygon >::const_iterator last = m_polygon_v.end();

    int count_i = 0;
    
    for (std::vector< SPolygon >::const_iterator i = m_polygon_v.begin(); 
         i != last; ++i )
    {
        fprintf( f_file_p,
                 "        <polygon id=\"polygon_%s_%i\"\n"
                 "                 points=\"",
                 f_prefix_str.c_str(), count_i );
        
        for (int v = 0; i->vertex_v.size(); ++v )
        {
            fprintf( f_file_p,
                     "%f,%f ",
                     i->vertex_v[v].x + f_offsetU_f,
                     i->vertex_v[v].y + f_offsetV_f );
        }

        if (i->fillColor.a == 0)
        {
            fprintf( f_file_p,
                     "                 stroke-width=\"%f\"\n"
                     "                 stroke=\"#%02x%02x%02x\"\n"
                     "                 opacity=\"%f\"\n"
                     "                 fill=\"none\" />\n",
                     1.f+i->lineWidth_f,
                     i->outlineColor.r, i->outlineColor.g, i->outlineColor.b, i->outlineColor.a/255.f);
        }
        else
        {
            fprintf( f_file_p,
                     "\"\n"
                     "                 stroke-width=\"%f\"\n"
                     "                 stroke=\"#%02x%02x%02x\"\n"
                     "                 opacity=\"%f\"\n"
                     "                 fill=\"#%02x%02x%02x\""
                     "                 fill-opacity=\"%f\" />\n",
                     1.f+i->lineWidth_f,
                     i->outlineColor.r, i->outlineColor.g, i->outlineColor.b, i->outlineColor.a/255.,
                     i->fillColor.r, i->fillColor.g, i->fillColor.b, i->fillColor.a/255.f );
        }
    }
    
    return true;
}

/// Return number of elements.
int
CPolygonList::getSize () const
{
    return m_polygon_v.size();    
}
