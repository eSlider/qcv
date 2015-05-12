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

#ifndef __3DPOINTLIST_H
#define __3DPOINTLIST_H

/**
 *******************************************************************************
 *
 * @file 3DPointList.h
 *
 * \class C3DPointList
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Handles a list of 3DPoints for displaying in QGLViewer
 *
 * The class is derived from C3DDrawingElementList implementing a list of 3DPoints.
 * 3DPoints are composed of: 
 *  - 3D point
 *  - 3D normal
 *  - a color, and
 *  - a point size.
 *
 *******************************************************************************/

/* INCLUDES */
#include "3DDrawingElementList.h"
#include "colors.h"

#include <vector>

/* CONSTANTS */


namespace QCV
{
    class C3DPointList: public C3DDrawingElementList
    {
    /// Constructor, Destructor
    public:
        /// Constructor
        C3DPointList( int f_bufferSize_i = -1);

        /// Destructor.
        virtual ~C3DPointList();

    /// Operations.
    public:

        // Add drawing 3DPoints from other list.
        virtual bool add (  const C3DPointList & f_otherList );

        // Draw all 3DPoints.
        virtual bool add (  C3DVector f_point,
                            SRgba     f_color,
                            float     f_pointSize_i,
                            C3DVector f_normal = C3DVector(0,0,0) );

        // Clear all 3DPoints.
        virtual bool clear ();

        // Draw all 3DPoints.
        virtual bool show ();

        // Return number of elements.
        virtual int  getSize () const;

        // Element names.
        virtual std::string  getGroupName() const { return "3DPoints"; };

    protected:
        typedef struct
        {
            /// 3d point
            C3DVector      point;

            /// 3d point
            C3DVector      normal;

            /// Color information
            SRgba          color;

            /// 3DPoint width
            float          pointSize_f;

        } S3DPoint;

    /// Private Members
    private:
        std::vector<S3DPoint>    m_3DPoint_v;
    };
} // Namespace QCV


#endif // __3DPOINTLIST_H
