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
 * \file  3DPointList.cpp
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */

#include "3DPointList.h"
#include "glheader.h"

using namespace QCV;

C3DPointList::C3DPointList( int /* f_bufferSize_i */ )
{}

/// Destructor.
C3DPointList::~C3DPointList()
{}

// Add 3DPoints from other list.
bool
C3DPointList::add ( const C3DPointList &f_otherList )
{
    m_3DPoint_v.insert( m_3DPoint_v.begin(), 
                        f_otherList.m_3DPoint_v.begin(),
                        f_otherList.m_3DPoint_v.end() );

    return true;
}

// Draw all 3DPoints.
bool
C3DPointList::add ( C3DVector f_point,
                    SRgba     f_color,
                    float     f_pointSize_i,
                    C3DVector f_normal  )
{
    m_3DPoint_v.push_back( S3DPoint() );

    S3DPoint & new3DPoint = m_3DPoint_v.back();
    
    new3DPoint.point       = f_point;
    new3DPoint.normal      = f_normal;
    new3DPoint.color       = f_color;
    new3DPoint.pointSize_f = f_pointSize_i;

    return true;
}

// Clear all 3DPoints.
bool
C3DPointList::clear ()
{
    m_3DPoint_v.clear();
    return m_3DPoint_v.empty();
}

// Draw all 3DPoints.
bool
C3DPointList::show ()
{
    std::vector< S3DPoint >::const_iterator last = m_3DPoint_v.end();

    for (std::vector< S3DPoint >::const_iterator i = m_3DPoint_v.begin(); 
         i != last; ++i )
    {   
        glPointSize ( i->pointSize_f );
        glBegin(GL_POINTS);

        glColor4ub( i->color.r, 
                    i->color.g, 
                    i->color.b, 
                    i->color.a );
        
        glNormal3f( i->normal.x(), 
                    i->normal.y(), 
                    i->normal.z() );

        glVertex3f( i->point.x(), 
                    i->point.y(), 
                    i->point.z() );

        glEnd();
    }

    /// Todo: Check GL status and return value.
    return true;
}

/// Return number of elements.
int
C3DPointList::getSize () const
{
    return m_3DPoint_v.size();    
}
