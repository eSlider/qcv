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

#ifndef __LINELIST_H
#define __LINELIST_H

/**
 *******************************************************************************
 *
 * @file lineList.h
 *
 * \class CLineList
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Handles a list of lines for displaying in screen or writing in file.
 *
 * The class is derived from CDrawingElementList implementing a list of lines.
 * Lines are composed of: 
 *  - rows and columns of the two points defining the line,
 *  - a color, and
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
    class CLineList: public CDrawingElementList
    {
    /// Constructor, Destructor
    public:
        /// Constructor
        CLineList( int f_bufferSize_i = -1);

        /// Destructor.
        virtual ~CLineList();

    /// Operations.
    public:

        // Add drawing lines from other list.
        virtual bool add (  const CLineList & f_otherList );

        // Draw all lines.
        virtual bool add (  float   f_u1_f, 
                            float   f_v1_f,
                            float   f_u2_f,
                            float   f_v2_f,
                            SRgba   f_color,
                            float   f_lineWidth_i );

        // Is blendable.
        virtual bool isBlendable () { return true; }

        // Clear all lines.
        virtual bool clear ();

        // Draw all lines.
        virtual bool show () const;

        // Write line in a SVG file.
        virtual bool write ( FILE*                f_file_p,
                             const float          f_offsetU_f = 0.0,
                             const float          f_offsetV_f = 0.0,
                             const std::string    f_parameters_str = "") const;

        // Return number of elements.
        virtual int  getSize () const;

        // Element names.
        virtual std::string  getGroupName() const { return "Lines"; };

    protected:
        typedef struct
        {
            /// Color information
            SRgba          color;

            /// Start position.
            float          u1_f, v1_f;

            /// End position.
            float          u2_f, v2_f;

            /// Line width
            float          lineWidth_f;

        } SLine;

    /// Private Members
    private:
        std::vector<SLine>    m_line_v;
    };
} // Namespace QCV


#endif // __LINELIST_H
