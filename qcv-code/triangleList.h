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

#ifndef __TRIANGLELIST_H
#define __TRIANGLELIST_H

/**
 *******************************************************************************
 *
 * @file triangleList.h
 *
 * \class CTriangleList
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Handles a list of triangles for displaying in screen or writing in file.
 *
 * The class is derived from CDrawingElementList implementing a list of triangles.
 * Triangles are composed of: 
 *  - rows and columns of the 3 corresponding vertices,
 *  - an outline color, 
 *  - a fill color, and 
 *  - a line width.
 *
 *******************************************************************************/

/* INCLUDES */
#include "drawingElementList.h"
#include "standardTypes.h"
#include "colors.h"

#include <vector>

/* CONSTANTS */


namespace QCV
{
    class CTriangleList: public CDrawingElementList
    {
        /// Constructor, Destructor
    public:
        /// Constructor
        CTriangleList( int f_bufferSize_i = -1);

        /// Destructor.
        virtual ~CTriangleList();

        /// Operations.
    public:

        // Add drawing from other list.
        virtual bool add (  const CTriangleList & f_otherList );

        // Add empty triangle.
        virtual bool add (  float   f_u1_f, 
                            float   f_v1_f,
                            float   f_u2_f,
                            float   f_v2_f,
                            float   f_u3_f, 
                            float   f_v3_f,
                            const SRgba   &f_color,
                            float   f_lineWidth_i );

        // Add empty triangle.
        virtual bool add (  const S2D<float> &f_vertex1, 
                            const S2D<float> &f_vertex2, 
                            const S2D<float> &f_vertex3, 
                            const SRgba   &f_color,
                            float   f_lineWidth_i );

        // Add filled triangle.
        virtual bool add (  float   f_u1_f, 
                            float   f_v1_f,
                            float   f_u2_f,
                            float   f_v2_f,
                            float   f_u3_f,
                            float   f_v3_f,
                            const SRgba   &f_outLineColor,
                            const SRgba   &f_fillColor,
                            float   f_lineWidth_i );

        // Add filled triangle.
        virtual bool add (  const S2D<float> &f_vertex1, 
                            const S2D<float> &f_vertex2, 
                            const S2D<float> &f_vertex3, 
                            const SRgba   &f_outLineColor,
                            const SRgba   &f_fillColor,
                            float   f_lineWidth_i );

        // Is blendable.
        virtual bool isBlendable () { return true; }

        // Clear all triangles.
        virtual bool clear ();

        // Draw all triangles.
        virtual bool show () const;

        // Write triangle in a SVG file.
        virtual bool write ( FILE*                f_file_p,
                             const float          f_offsetU_f = 0.0,
                             const float          f_offsetV_f = 0.0,
                             const std::string    f_parameters_str = "") const;

        // Return number of elements.
        virtual int  getSize () const;

        // Element names.
        virtual std::string  getGroupName() const { return "Triangles"; };

    protected:
        typedef struct
        {
            /// Color information
            SRgba          outlineColor;

            /// Color information
            SRgba          fillColor;

            S2D<float>     vertices[3];

            /// Line width
            float          lineWidth_f;
        } STriangle;

        /// Private Members
    private:
        std::vector<STriangle>    m_triangle_v;
    };
} // Namespace QCV


#endif // __TRIANGLELIST_H
