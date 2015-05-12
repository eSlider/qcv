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

#ifndef __IMAGEPYRAMID_H
#define __IMAGEPYRAMID_H

/**
 *******************************************************************************
 *
 * @file imagePyramid.h
 *
 * \class CImagePyramid
 * \author Hernan Badino (hernan.badino@gmail.com)
 *
 * \brief Implements for handling image pyramid.
 *
 * The class implements an operator for handling image pyramids. The base
 * level of the pyramid (original image size) might be self allocated or
 * copied from the input source image.
 *
 *******************************************************************************/


/* CONSTANTS */

/* INCLUDES */
#include <opencv/cv.h>

namespace QCV
{
    class CImagePyramid
    {
    /// Public data types.
    public:

    /// Constructors/Destructor
    public: 
        CImagePyramid ( );
        CImagePyramid ( unsigned int f_levels_ui );
        virtual ~CImagePyramid ( );

    /// Computation
    public:
        virtual void clear (  );

    /// Gets and Sets
    public:
        virtual 
        cv::Mat &                getLevelImage ( unsigned int f_level_ui );

        virtual 
        cv::Mat                  getLevelImage ( unsigned int f_level_ui ) const;
        
        virtual 
        bool                     setLevels ( unsigned int f_levels_ui );

        virtual
        unsigned int             getLevels ( ) const;

        virtual 
        unsigned int             getWidth ( ) const;
        virtual 
        unsigned int             getHeight ( ) const;

    /// Computation
    public:
        bool        compute ( const cv::Mat &f_img );
        
    /// Protected help methods.
    protected:

    /// Protected members.
    protected:

        /// Pyramid Images
        std::vector<cv::Mat>           m_images_v;

        /// Width of the original level image.
        unsigned int                   m_width_ui;

        /// Heightr of the original level image.
        unsigned int                   m_height_ui;
    };
}



#endif // __IMAGEPYRAMID_H
