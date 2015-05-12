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

#ifndef __IMGSCALEROP_H
#define __IMGSCALEROP_H

/**
*******************************************************************************
*
* @file imgScalerOp.h
*
* \class CImageScalerOp
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
    class CImageScalerOp: public COperator
    {
    public:

        /// Constructor, Desctructors
    public:    
        typedef enum {
            SM_FACTOR,
            SM_SIZE
        } EScaleMode;            

        /// Parameter access
    public:    
        ADD_PARAM_ACCESS (std::string, m_inputId_str,        InputId );
        ADD_PARAM_ACCESS (std::string, m_outputId_str,       OutputId );

        ADD_PARAM_ACCESS (bool,        m_compute_b,          Compute );
        ADD_PARAM_ACCESS (EScaleMode,  m_scaleMode_e,        ScaleMode );

        ADD_PARAM_ACCESS (int,         m_interpolMode_i,     InterpolationMode );

        bool              setScaleFactor ( S2D<float> f_factors );
        S2D<float>        getScaleFactor ( ) const;

        bool              setScaleSize ( S2D<unsigned int> f_size );
        S2D<unsigned int> getScaleSize ( ) const;

        /// Constructor, Desctructors
    public:    
        
        /// Constructors.
        CImageScalerOp ( COperator * const f_parent_p = NULL,
                         const std::string f_name_str = "Image Scaler",
                         const int f_preferedNumImgs_i = 1 );
        
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

        /// User Operation Events
    public:
        /// Compute
        virtual bool compute ( const CMatVector & f_input, 
                               CMatVector       & fr_output );

    protected:

    protected:

        void registerDrawingLists( int f_numReg_i );

        void registerParameters( int f_numReg_i );

        void resize();
        
	bool getInputs();

    private:

        /// Input image id
        std::string                 m_inputId_str;
        
        /// Output image id
        std::string                 m_outputId_str;
        
        /// Compute?
        bool                        m_compute_b;

        //// Stereo algorithm to use
        EScaleMode                  m_scaleMode_e;
        
        /// Fixed size
        S2D<float>                  m_scaleFactor;
        
        /// Fixed size
        S2D<unsigned int>           m_scaleSize;

        /// Input image
        CMatVector                  m_img_v;

        /// Scaled image
        CMatVector                  m_scaledImgs_v;

        /// Interpolation mode
        int                         m_interpolMode_i;
    };
}
#endif // __IMGSCALEROP_H
 
