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

#ifndef __DISPLAYCOLORENCIMAGELIST_H
#define __DISPLAYCOLORENCIMAGELIST_H

/**
 * @file displayCEImageList.h
 *
 * \class CDisplayCEImageList
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Handles a list of images for displaying in screen or writing in file
 * with a given color encoding.
 *
 * The class is derived from CDrawingElementList implementing a list of images.
 * Images are composed of: 
 *  - the image itself
 *  - row and column of top-left corner where to display it,
 *  - display width and height,
 *  - the color encoding object (of type CColorEncoding).
 *
 */

/* INCLUDES */
#include <opencv/cv.h>
#include "drawingElementList.h"
#include "colors.h"
#include "colorEncoding.h"

#include <vector>

namespace QCV
{
    class CDisplayColorEncImageList: public CDrawingElementList
    {
    /// Constructor, Destructor
    public:
        /// Constructor
        CDisplayColorEncImageList();

        /// Destructor.
        virtual ~CDisplayColorEncImageList();

    /// Operations.
    public:
        // Add images from other list.
        virtual bool add (  const CDisplayColorEncImageList & f_otherList );

        // Add an image.
        virtual bool add (  cv::Mat *           f_image_p,
                            CColorEncoding      f_encoder_f,
                            float               f_u_f, 
                            float               f_v_f,
                            float               f_dispWidth_f,
                            float               f_dispHeight_f,
                            float               f_alpha_f = 1.f,
                            bool                f_makeCopy_b = false );

        // Is blendable.
        virtual bool isBlendable () { return false; }

        // Clear all lines.
        virtual bool clear ();

        // Draw all lines.
        virtual bool show () const;

        // Write line in a SVG file.
        virtual bool write ( FILE*              f_file_p,
                             const float        f_offsetU_f = 0.0,
                             const float        f_offsetV_f = 0.0,
                             const std::string  f_parameters_str = "") const;

        // Return number of elements.
        virtual int  getSize () const;

    protected:
        struct SDisplayColorEncImage
        {
            /// Color information
            cv::Mat *           image_p;

            /// Copied
            bool                copied_b;
            
            /// Start position.
            float               u_f, v_f;

            /// Width and height.
            float               width_f, height_f;

            /// Alpha value.
            float               alpha_f;

            /// Color encoding
            CColorEncoding      encoder;

            void freeImage()
            {
                if ( copied_b )
                    delete image_p;
            }
            
        };

    /// Private Methods
    private:
        template <class Type_>
        bool showWithType ( const SDisplayColorEncImage & f_elem ) const;
        
    /// Private Members
    private:
        
        typedef std::vector<SDisplayColorEncImage>  DisplayColorEncImageList_t;
        
        /// Vector of images.
        DisplayColorEncImageList_t     m_image_v;        
    };
} // Namespace QCV


#endif // __DISPLAYCOLORENCIMAGELIST_H
