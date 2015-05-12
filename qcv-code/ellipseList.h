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

#ifndef __ELLIPSELIST_H
#define __ELLIPSELIST_H

/**
 *******************************************************************************
 *
 * @file ellipseList.h
 *
 * \class CEllipseList
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Handles a list of ellipses for displaying in screen or writing in file.
 *
 * The class is derived from CDrawingElementList implementing a list of ellipses.
 * Ellipses are composed of: 
 *  - row and column of center point,
 *  - radius in columns and rows,
 *  - a rotation in degrees around the center,
 *  - an outline color, 
 *  - a fill color, and 
 *  - a line width.
 *
 *******************************************************************************/

/* INCLUDES */
#include "drawingElementList.h"
#include "colors.h"

#include <vector>

/* CONSTANTS */


namespace QCV
{
    class CEllipseList: public CDrawingElementList
    {
        /// Constructor, Destructor
    public:
        /// Constructor
        CEllipseList( int f_bufferSize_i = -1);

        /// Destructor.
        virtual ~CEllipseList();

        /// Operations.
    public:

        // Add drawing from other list.
        virtual bool add (  const CEllipseList & f_otherList );

        // Add empty ellipses.
        virtual bool add (  float   f_u_f, 
                            float   f_v_f,
                            float   f_radiusU_f,
                            float   f_radiusV_f,
                            float   f_rotAngle_f,
                            SRgba   f_color,
                            float   f_lineWidth_i );

        // Add filled ellipses.
        virtual bool add (  float   f_u_f, 
                            float   f_v_f,
                            float   f_radiusU_f,
                            float   f_radiusV_f,
                            float   f_rotAngle_f,
                            SRgba   f_color,
                            SRgba   f_fillColor,
                            float   f_lineWidth_i );

        // Is blendable.
        virtual bool isBlendable () { return true; }

        // Clear all ellipses.
        virtual bool clear ();

        // Draw all ellipses.
        virtual bool show () const;

        // Write ellipse in a SVG file.
        virtual bool write ( FILE*                f_file_p,
                             const float          f_offsetU_f = 0.0,
                             const float          f_offsetV_f = 0.0,
                             const std::string    f_parameters_str = "") const;

        // Return number of elements.
        virtual int  getSize () const;

        // Element names.
        virtual std::string  getGroupName() const { return "Ellipses"; };

    protected:
        typedef struct
        {
            /// Color information
            SRgba          outlineColor;

            /// Color information
            SRgba          fillColor;

            /// Center position.
            float          u_f, v_f;

            /// Radius in u and v
            float          radiusU_f, radiusV_f;

            /// Rotation angle [rad].
            float          rotation_f;

            /// Line width
            float          lineWidth_f;
        } SEllipse;

        /// Private Members
    private:
        std::vector<SEllipse>    m_ellipse_v;
    };
} // Namespace QCV


#endif // __ELLIPSELIST_H
