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

#ifndef __MONOTRACKER_H
#define __MONOTRACKER_H

/**
*******************************************************************************
*
* @file monoTrackerOp.h
*
* \class CMonoTracker
* \author Hernan Badino (hernan.badino@gmail.com)
* \brief Class for computing feature tracking information.
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
#include "camera.h"

#include "feature.h"
/* PROTOTYPES */

/* CONSTANTS */

namespace QCV
{
    class CGfttFreakOp;
    class CKltTrackerOp;
   
    class CMonoTrackerOp: public COperator
    {
    /// Constructor, Desctructors
    public:    

    /// Parameter access
    public:
        ADD_PARAM_ACCESS         (bool,        m_compute_b,          Compute );
        ADD_PARAM_ACCESS         (S2D<float>,  m_centralPointOffset, CentralPointOffset );
        ADD_PARAM_ACCESS_BOUNDED (float,       m_fovScale_f,         FovScale, 0.001f, 1.f );
        ADD_PARAM_ACCESS         (bool,        m_scaleHorOnly_b,     ScaleHorizontallyOnly );
        ADD_PARAM_ACCESS         (bool,        m_resizeToOrigSize_b, ResizeToOrigSize );

        ADD_PARAM_ACCESS         (S2D<int>,    m_cropTopLeft,        CropTopLeft );
        ADD_PARAM_ACCESS         (S2D<int>,    m_cropBottomRight,    CropBottomRight );

        ADD_PARAM_ACCESS         (bool,        m_registerDL_b,       RegisterDrawingLists );
       
    /// Constructor, Desctructors
    public:    
        
        /// Constructors.
        CMonoTrackerOp ( COperator * const f_parent_p = NULL,
                   const std::string f_name_str = "Mono Tracker" );
        
        /// Virtual destructor.
        virtual ~CMonoTrackerOp ();

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

        /// Good feature to track operator
        CGfttFreakOp *              m_gftt_p;
       
        /// Good feature to track operator
        CKltTrackerOp *             m_kltTracker_p;       

        /// Compute?
        bool                        m_compute_b;

        /// Mono Camera
        CCamera                     m_origCamera;

        /// Mono Camera
        CCamera                     m_camera;

        /// Central point offset
        S2D<float>                  m_centralPointOffset;

        //// Resize to original image size
        bool                        m_resizeToOrigSize_b;

        //// FOV scale factor
        float                       m_fovScale_f;

        //// Scale horizontally only
        bool                        m_scaleHorOnly_b;

        /// Scaled images
        cv::Mat                     m_grayImage0;

        /// Scaled images
        cv::Mat                     m_grayImage1;         

        /// Scaled images
        cv::Mat                     m_scaledImage0;

        /// Scaled images
        cv::Mat                     m_scaledImage1;         

       /// Scaled images
        cv::Mat                     m_scaledImage2;

        /// Scaled images
        cv::Mat                     m_scaledImage3;         

        /// Unified feature vector
        CFeatureVector              m_unifiedFeatureVector;
 
        /// Top-left crop coordinate.
        S2D<int>                    m_cropTopLeft;

        /// Bottom-Right crop coordinate.
        S2D<int>                    m_cropBottomRight;

        /// Register drawing lists?
        bool                        m_registerDL_b;
    };
}
#endif // __MONOTRACKER_H
 
