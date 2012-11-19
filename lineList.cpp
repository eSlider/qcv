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
 * \file  lineList.cpp
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */
#include <QGLContext>
#include "lineList.h"
#include "glheader.h"
#include <stdio.h>

using namespace QCV;

extern QGLContext * g_QGLContext_p;

CLineList::CLineList( int /* f_bufferSize_i */ )
{}

/// Destructor.
CLineList::~CLineList()
{}

// Add lines from other list.
bool
CLineList::add ( const CLineList &f_otherList )
{
    m_line_v.insert( m_line_v.begin(), 
                     f_otherList.m_line_v.begin(),
                     f_otherList.m_line_v.end() );

    return true;
}

// Draw all lines.
bool
CLineList::add ( float   f_u1_f, 
                 float   f_v1_f,
                 float   f_u2_f,
                 float   f_v2_f,
                 SRgba   f_color,
                 float   f_lineWidth_i )
{
    SLine newLine;

    newLine.color       = f_color;
    newLine.u1_f        = f_u1_f;
    newLine.u2_f        = f_u2_f;
    newLine.v1_f        = f_v1_f;
    newLine.v2_f        = f_v2_f;
    newLine.lineWidth_f = f_lineWidth_i;

    m_line_v.push_back(newLine);

    return true;
}

// Clear all lines.
bool
CLineList::clear ()
{
    m_line_v.clear();
    return m_line_v.size();
}

// Draw all lines.
bool
CLineList::show () const
{
    //printf("g_QGLContext_p  = %p\n",g_QGLContext_p);
    std::vector< SLine >::const_iterator last = m_line_v.end();

    for (std::vector< SLine >::const_iterator i = m_line_v.begin(); 
         i != last; ++i )
    {   
        glLineWidth( i->lineWidth_f );
        glColor4ub(i->color.r, i->color.g, i->color.b, i->color.a);

        glBegin(GL_LINES);
        glVertex2f(i->u1_f, i->v1_f);
        glVertex2f(i->u2_f, i->v2_f);
        glEnd();
    }

    /// Todo: Check GL status and return value.
    return true;
}

bool
CLineList::write ( FILE*                f_file_p,
                   const float          f_offsetU_f /* = 0.0 */,
                   const float          f_offsetV_f /* = 0.0 */,
                   const std::string    f_prefix_str /* = "" */) const
{
    std::vector< SLine >::const_iterator last = m_line_v.end();
    
    int count_i = 0;
    
    for (std::vector< SLine >::const_iterator i = m_line_v.begin(); 
         i != last; ++i, ++count_i )
    {
        fprintf( f_file_p,
                 "        <line id=\"line_%s_%i\"\n"
                 "              x1=\"%f\" y1=\"%f\"\n"
                 "              x2=\"%f\" y2=\"%f\"\n"
                 "              stroke-width=\"%f\"\n"
                 "              stroke=\"#%02x%02x%02x\"\n"
                 "              opacity=\"%f\" />\n",
                 f_prefix_str.c_str(), count_i,
                 i->u1_f+f_offsetU_f, i->v1_f+f_offsetV_f,
                 i->u2_f+f_offsetU_f, i->v2_f+f_offsetV_f,
                 1+i->lineWidth_f,
                 i->color.r, i->color.g, i->color.b, i->color.a/255.f );
    }
    
    return true;
}

/// Return number of elements.
int
CLineList::getSize () const
{
    return m_line_v.size();    
}
