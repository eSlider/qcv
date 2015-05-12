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

#ifndef __3DLINELIST_H
#define __3DLINELIST_H

/**
 *******************************************************************************
 *
 * @file 3DLineList.h
 *
 * \class C3DLineList
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Handles a list of 3DLines for displaying in QGLViewer
 *
 * The class is derived from C3DDrawingElementList implementing a list of 3DLines.
 * 3DLines are composed of: 
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
    class C3DLineList: public C3DDrawingElementList
    {
    /// Constructor, Destructor
    public:
        /// Constructor
        C3DLineList( int f_bufferSize_i = -1);

        /// Destructor.
        virtual ~C3DLineList();

    /// Operations.
    public:

        // Add drawing 3DLines from other list.
        virtual bool add (  const C3DLineList & f_otherList );

        // Draw all 3DLines.
        virtual bool add (  C3DVector f_point1,
                            C3DVector f_point2,
                            SRgba     f_color,
                            float     f_lineWidth_f );

        // Clear all 3DLines.
        virtual bool clear ();

        // Draw all 3DLines.
        virtual bool show ();

        // Return number of elements.
        virtual int  getSize () const;

        // Element names.
        virtual std::string  getGroupName() const { return "3DLines"; };

    protected:
        typedef struct
        {
            /// 3d point
            C3DVector      point1;

            /// 3d point
            C3DVector      point2;

            /// Color information
            SRgba          color;

            /// 3DLine width
            float          lineWidth_f;

        } S3DLine;

    /// Private Members
    private:
        std::vector<S3DLine>    m_3DLine_v;
    };
} // Namespace QCV


#endif // __3DLINELIST_H
