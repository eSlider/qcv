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

#ifndef __TEXTLIST_H
#define __TEXTLIST_H

/**
 *******************************************************************************
 *
 * @file textList.h
 *
 * \class CTextList
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Handles a list of texts for displaying in screen or writing in file.
 *
 * The class is derived from CDrawingElementList implementing a list of texts.
 * Texts are composed of: 
 *  - rows and columns of the two points defining the text,
 *  - a color, and
 *  - a text width.
 *
 *******************************************************************************/

/* INCLUDES */
#include "drawingElementList.h"
#include "colors.h"

#include <vector>

/* CONSTANTS */


namespace QCV
{
    class CTextList: public CDrawingElementList
    {
    /// Constructor, Destructor
    public:
        /// Constructor
        CTextList( int f_bufferSize_i = -1);

        /// Destructor.
        virtual ~CTextList();

    /// Operations.
    public:

        // Add drawing texts from other list.
        virtual bool add (  const CTextList & f_otherList );

        // Draw all texts.
        virtual bool add (  std::string f_text_str,
                            float   f_u_f, 
                            float   f_v_f,
                            SRgba   f_color,
                            float   f_lineWidth_f,
                            float   f_fontSize_f,
                            bool    f_fixSize_b = false );

        // Is blendable.
        virtual bool isBlendable () { return true; }

        // Clear all texts.
        virtual bool clear ();

        // Draw all texts.
        virtual bool show () const;

        // Write text in a SVG file.
        virtual bool write ( FILE*                f_file_p,
                             const float          f_offsetU_f = 0.0,
                             const float          f_offsetV_f = 0.0,
                             const std::string    f_parameters_str = "") const;

        // Return number of elements.
        virtual int  getSize () const;

        // Element names.
        virtual std::string  getGroupName() const { return "Text Strings"; };

    protected:
        typedef struct
        {
            /// Color information
            SRgba          color;

            /// String
            char           text_str[1024];

            /// Lateral position
            float          u_f;
            
            /// Vertical position
            float          v_f;
            
            /// Width of line
            float          lineWidth_f;
            
            /// Font size
            float          fontSize_f;

            /// Fix font size?
            bool           fixSize_b;
        } SText;

    /// Private Members
    private:
        std::vector<SText>    m_text_v;
    };
} // Namespace QCV


#endif // __TEXTLIST_H
