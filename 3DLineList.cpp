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
 * \file  3DLineList.cpp
 * \author Hernan Badino
 * \notes 
 *******************************************************************************
 *****             (C) Hernan Badino 2012 - All Rights Reserved            *****
 ******************************************************************************/

/* INCLUDES */

#include "3DLineList.h"
#include "glheader.h"

using namespace QCV;

C3DLineList::C3DLineList( int /* f_bufferSize_i */ )
{}

/// Destructor.
C3DLineList::~C3DLineList()
{}

// Add 3DLines from other list.
bool
C3DLineList::add ( const C3DLineList &f_otherList )
{
   m_3DLine_v.insert( m_3DLine_v.begin(), 
                      f_otherList.m_3DLine_v.begin(),
                      f_otherList.m_3DLine_v.end() );

   return true;
}

// Draw all 3DLines.
bool
C3DLineList::add ( C3DVector f_point1,
                   C3DVector f_point2,
                   SRgba     f_color,
                   float     f_lineWidth_f )
{
   m_3DLine_v.push_back( S3DLine() );

   S3DLine & new3DLine = m_3DLine_v.back();
    
   new3DLine.point1       = f_point1;
   new3DLine.point2       = f_point2;
   new3DLine.color        = f_color;
   new3DLine.lineWidth_f  = f_lineWidth_f;

   return true;
}

// Clear all 3DLines.
bool
C3DLineList::clear ()
{
   m_3DLine_v.clear();
   return m_3DLine_v.empty();
}

// Draw all 3DLines.
bool
C3DLineList::show ()
{
   std::vector< S3DLine >::const_iterator last = m_3DLine_v.end();

   glBegin(GL_LINES);
   for (std::vector< S3DLine >::const_iterator i = m_3DLine_v.begin(); 
        i != last; ++i )
   {   
      glLineWidth( i->lineWidth_f );

      glColor4ub( i->color.r, 
                  i->color.g, 
                  i->color.b, 
                  i->color.a );
        
      glVertex3f( i->point1.x(), 
                  i->point1.y(), 
                  i->point1.z() );

      glVertex3f( i->point2.x(), 
                  i->point2.y(), 
                  i->point2.z() );

   }
   glEnd();

   /// Todo: Check GL status and return value.
   return true;
}

/// Return number of elements.
int
C3DLineList::getSize () const
{
   return m_3DLine_v.size();    
}
