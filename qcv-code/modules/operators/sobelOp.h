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

#ifndef __SOBELOP_H
#define __SOBELOP_H

/**
*******************************************************************************
*
* @file sobelOp.h
*
* \class CSobelOp
* \author Hernan Badino (hernan.badino@gmail.com)
* \brief Class for resizing images.
*
*******************************************************************************/

/* INCLUDES */
#include <math.h>
#include <limits>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "imageFromFile.h"
#include "operator.h"
#include "colorEncoding.h"
#include "matVector.h"

/* PROTOTYPES */

/* CONSTANTS */

namespace QCV
{
    class CSobelOp: public COperator
    {
    public:
        typedef enum 
        {
            ID_GRADX = 0,
            ID_GRADY = 1
        } EOutputImageIndex;

        /// Constructor, Desctructors
    public:    

        /// Parameter access
    public:    
        ADD_PARAM_ACCESS         (bool,        m_compute_b,          Compute );
        ADD_PARAM_ACCESS         (int,         m_ksize_i,            KernelSize );
        ADD_PARAM_ACCESS         (int,         m_interpolMode_i,     InterpolationMode );
        ADD_PARAM_ACCESS         (bool,        m_applyGauss_b,       ApplyGauss );

        /// Constructor, Desctructors
    public:    
        
        /// Constructors.
        CSobelOp ( COperator * const f_parent_p = NULL,
                   const std::string f_name_str = "Sobel Operator" );
        
        /// Virtual destructor.
        virtual ~CSobelOp ();

        /// Cycle event.
        virtual bool cycle( );
    
        /// Show event.
        virtual bool show();
    
        /// Init event.
        virtual bool initialize();
    
        /// Reset event.
        virtual bool reset();
    
        /// Exit event.
        virtual bool exit();

    protected:

        void registerDrawingLists( );

        void registerParameters( );

    private:

        /// Compute?
        bool                        m_compute_b;

        /// Kernel size to use
        int                         m_ksize_i;

        /// Apply gauss?
        bool                        m_applyGauss_b;

        /// Input image
        cv::Mat                     m_img;

        /// Scaled image
        CMatVector                  m_gradImgs_v;

        /// Interpolation mode
        int                         m_interpolMode_i;

        /// Result of gauss
        cv::Mat                     m_gaussImg;
    };
}
#endif // __SOBELOP_H
 
