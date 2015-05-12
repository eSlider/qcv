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

#ifndef __DISPLAYIMAGELIST_H
#define __DISPLAYIMAGELIST_H

/**
 * @file displayImageList.h
 *
 * \class CDisplayImageList
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Handles a list of images for displaying in screen or writing in file.
 *
 * The class is derived from CDrawingElementList implementing a list of images.
 * Images are composed of: 
 *  - the image itself
 *  - row and column of top-left corner where to display it,
 *  - display width and height,
 *  - an alpha component for transparency, and 
 *  - scale factor and bias to apply on the data for displaying, 
 *
 */

/* INCLUDES */
#include "drawingElementList.h"
#include "colors.h"

#include <opencv/cv.h>

#include <vector>

namespace QCV
{
    class CDisplayImageList: public CDrawingElementList
    {
    /// Constructor, Destructor
    public:
        /// Constructor
        CDisplayImageList();

        /// Destructor.
        virtual ~CDisplayImageList();

    /// Operations.
    public:

        // Add images from other list.
        virtual bool add (  const CDisplayImageList & f_otherList );

        // Add an image.
        virtual bool add (  cv::Mat             f_image, 
                            float               f_u_f, 
                            float               f_v_f,
                            float               f_dispWidth_f,
                            float               f_dispHeight_f,
                            float               f_scale_f = 1.f,
                            float               f_bias_f  = 0.f,
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

        // Element names.
        virtual std::string  getGroupName() const { return "Images"; };

    protected:
        typedef struct
        {
            /// Color information
            cv::Mat        image;

            /// Start position.
            float          u_f, v_f;

            /// Width and height.
            float          width_f, height_f;

            /// Alpha value.
            float          alpha_f;

            /// Scale
            float          scale_f;

            /// Bias
            float          bias_f;

            /// Texture id
            unsigned int   textureId_ui;

            /// Own resources?
	    bool           ownResources_b;
        } SDisplayImage;

    /// Private Members
    private:
        
        typedef std::vector<SDisplayImage>  DisplayImageList_t;
        
        /// Vector of images.
        DisplayImageList_t     m_image_v;        
    };
} // Namespace QCV


#endif // __DISPLAYIMAGELIST_H
