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

#ifndef __POLYGONLIST_H
#define __POLYGONLIST_H

/**
 *******************************************************************************
 *
 * @file polygonList.h
 *
 * \class CPolygonList
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Handles a list of polygons for displaying in screen or writing in file.
 *
 * The class is derived from CDrawingElementList implementing a list of polygons.
 * Polygons are composed of: 
 *  - rows and columns of the top-left and bottom-right corners.
 *  - an outline color, 
 *  - a fill color, and 
 *  - a line width.
 *
 ******************************************************************************/

/* INCLUDES */
#include "drawingElementList.h"
#include "colors.h"
#include "standardTypes.h"

#include <vector>

/* CONSTANTS */


namespace QCV
{
    class CPolygonList: public CDrawingElementList
    {
        /// Constructor, Destructor
    public:
        /// Constructor
        CPolygonList( int f_bufferSize_i = -1);

        /// Destructor.
        virtual ~CPolygonList();

        /// Operations.
    public:

        // Add drawing from other list.
        virtual bool add (  const CPolygonList &f_otherList );

        // Add empty polygons.
        virtual bool add (  const std::vector< S2D<float> > & f_vector,
                            SRgba   f_color,
                            float   f_lineWidth_i );

        // Add filled polygons.
        virtual bool add (  const std::vector< S2D<float> > & f_vector,
                            SRgba   f_outLineColor,
                            SRgba   f_fillColor,
                            float   f_lineWidth_i );

        // Is blendable.
        virtual bool isBlendable () { return true; }

        // Clear all polygons.
        virtual bool clear ();

        // Draw all polygons.
        virtual bool show () const;

        // Write polygon in a SVG file.
        virtual bool write ( FILE*                f_file_p,
                             const float          f_offsetU_f = 0.0,
                             const float          f_offsetV_f = 0.0,
                             const std::string    f_parameters_str = "") const;

        // Return number of elements.
        virtual int  getSize () const;

        // Element names.
        virtual std::string  getGroupName() const { return "Polygons"; };

    protected:
        typedef struct
        {
            /// Color information
            SRgba                     outlineColor;

            /// Color information
            SRgba                     fillColor;

            /// Start position.
            std::vector< S2D<float> > vertex_v;

            /// Line width
            float                     lineWidth_f;
        } SPolygon;

        /// Private Members
    private:
        std::vector<SPolygon>    m_polygon_v;
    };
} // Namespace QCV


#endif // __POLYGONLIST_H

