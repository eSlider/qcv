/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 3. See the GNU LGPL version 3 for details.
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

#ifndef __IMGSCALEROP_H
#define __IMGSCALEROP_H

/**
*******************************************************************************
*
* @file opencvstereoOp.cpp
*
* \class COpencvstereoOp
* \author Hernan Badino (hernan.badino@gmail.com)
*
* \brief Class for computing stereo.
*
*******************************************************************************/

/* INCLUDES */
#include <math.h>
#include <limits>

#include <cv.h>
#include <highgui.h>

#include "imageFromFile.h"
#include "operator.h"
#include "colorEncoding.h"

/* PROTOTYPES */

/* CONSTANTS */

namespace QCV
{
    class CImageScalerOp: public COperator<cv::Mat, cv::Mat>
    {
    /// Constructor, Desctructors
    public:    
        typedef enum {
            SM_FACTOR,
            SM_SIZE
        } EScaleMode;            

    /// Parameter access
    public:    
        ADD_PARAM_ACCESS         (bool,        m_compute_b,          Compute );
        ADD_PARAM_ACCESS         (EScaleMode,  m_scaleMode_e,        ScaleMode );

        bool              setScaleFactor ( S2D<float> f_factors );
        S2D<float>        getScaleFactor ( ) const;

        bool              setScaleSize ( S2D<unsigned int> f_size );
        S2D<unsigned int> getScaleSize ( ) const;

    /// Constructor, Desctructors
    public:    
        
        /// Constructors.
        CImageScalerOp ( COperatorBase * const f_parent_p = NULL,
                       const std::string f_name_str = "Image Scaler" );
        
        /// Virtual destructor.
        virtual ~CImageScalerOp ();

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

    public:
       
    /// I/O registration.
    public:
        /// Set the input of this operator
        bool setInput  ( const cv::Mat & f_input );

        /// Gets the output of this operator
        bool getOutput ( cv::Mat & f_output ) const;

    protected:

    protected:

        void registerDrawingLists();

        void registerParameters();

    private:

        /// Compute?
        bool                        m_compute_b;

        //// Stereo algorithm to use
        EScaleMode                  m_scaleMode_e;
        
        /// Fixed size
        S2D<float>                  m_scaleFactor;
        
        /// Fixed size
        S2D<unsigned int>           m_scaleSize;

        /// Input image
        cv::Mat                     m_img;

        /// Scaled image
        cv::Mat                     m_scaledImg;

    };
}
#endif // __IMGSCALEROP_H
 
